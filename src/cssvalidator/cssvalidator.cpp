/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#include "cssvalidator.h"
#include "validator.h"
#include "soupreader.h"
#include "validatormenu.h"
#include "cssconfigure.h"

#include <climits>

/* QtCore */
#include <QtCore/QByteArrayMatcher>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCursor>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>
#include <QtGui/QSizePolicy>

CSSValidator::CSSValidator ( QWidget * parent, Settings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconCritical ( QString::fromUtf8 ( ":/icons/critical.png" ) )
{
  setObjectName ( QLatin1String ( "cssvalidator" ) );
  setWindowTitle ( i18n ( "CSS Validation" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 0, 0, 0, 0 );

  m_soupReader = new SoupReader ( this );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( true );
  m_listWidget->setWordWrap ( false );
  m_listWidget->sortItems ( Qt::AscendingOrder );
  m_listWidget->setSelectionMode ( QAbstractItemView::SingleSelection );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  m_listWidget->setBackgroundRole ( QPalette::AlternateBase );
  m_listWidget->setAlternatingRowColors ( true );

  setWidget ( m_listWidget );

  // Prozess
  m_validator = new Validator ( this );
  m_validator->setEnviromentVariable ( cfg );

  // Kontext Menü
  m_menu = new ValidatorMenu ( this );

  // Listen Signale
  connect ( m_listWidget, SIGNAL ( itemPressed ( QListWidgetItem * ) ),
            this, SLOT ( markItem ( QListWidgetItem * ) ) );

  connect ( m_listWidget, SIGNAL ( itemDoubleClicked ( QListWidgetItem * ) ),
            this, SLOT ( doubleClicked ( QListWidgetItem * ) ) );

  // Signale der Überprüfung
  connect ( m_validator, SIGNAL ( error ( QProcess::ProcessError ) ),
            this, SLOT ( errors ( QProcess::ProcessError ) ) );

  connect ( m_validator, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );

  connect ( m_validator, SIGNAL ( readyReadStandardOutput() ),
            this, SLOT ( readStandardReply() ) );

  connect ( m_validator, SIGNAL ( criticalError ( const QString & ) ),
            this, SLOT ( criticalItem ( const QString & ) ) );

  connect ( m_validator, SIGNAL ( started() ),
            this, SLOT ( processTriggered() ) );

  // Signale der XML Analyse
  connect ( m_soupReader, SIGNAL ( beginParsed () ),
            this, SLOT ( clearItems () ) );

  connect ( m_soupReader, SIGNAL ( parserError ( const QString & ) ),
            this, SLOT ( criticalItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( warnings ( const QString & ) ),
            this, SLOT ( warningItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( congratulation ( const QString & ) ),
            this, SLOT ( noticeItem ( const QString & ) ) );

  // Kontext Menü Aktionen
  connect ( m_menu, SIGNAL ( check() ), m_validator, SLOT ( validate() ) );
  connect ( m_menu, SIGNAL ( dropout() ), this, SLOT ( shutdownProcess() ) );
  connect ( m_menu, SIGNAL ( ascending() ), this, SLOT ( sortAscending() ) );
  connect ( m_menu, SIGNAL ( descending() ), this, SLOT ( sortDescending() ) );
  connect ( m_menu, SIGNAL ( clearlist() ), this, SLOT ( clearItems() ) );
  connect ( m_menu, SIGNAL ( configure() ), this, SLOT ( openConfig() ) );
}

/**
* Nachsehen ob diese Adresse nicht für die
* Prüfung existiert.
* Wenn ja dann true zurück geben.
*/
bool CSSValidator::isUnique ( const QUrl &url )
{
  for ( int i = 0 ; i < m_listWidget->count(); i++ )
  {
    if ( m_listWidget->item ( i )->data ( Qt::UserRole ).toUrl() == url )
      return false;
  }
  return true;
}

/**
* Die eingereichte Adresse prüfen und nachsehen ob bei
* @class Validator schon ein Prozess am laufen ist.
* Wenn nicht und @ref Validator::setValidation kein
* false zurück gibt, dann die Adresse in @ref usedUrl
* schreiben und true zurück geben.
*/
bool CSSValidator::prepareToExecute ( const QUrl &url )
{
  if ( url.isValid() && ! url.toString().isEmpty() )
  {
    if ( m_validator->isRunning() )
    {
      warningItem ( i18n ( "Rejected - CSS Validator is busy ..." ) );
      return false;
    }

    if ( m_validator->setValidation ( url.toString() ) )
    {
      usedUrl = url.toString();
      return true;
    }
  }
  return false;
}

/**
* Bei einem auswählen mit itemPressed diesen
* Eintrag bei @ref prepareToExecute gegenprüfen
* und im Menü die Aktion aCheck (de)aktivieren.
*/
void CSSValidator::markItem ( QListWidgetItem * item )
{
  if ( ! item->data ( Qt::UserRole ).isValid() )
  {
    m_menu->enableCheckUrlAction ( false );
    return;
  }

  QUrl url = item->data ( Qt::UserRole ).toUrl();
  if ( prepareToExecute ( url ) )
    m_menu->enableCheckUrlAction ( true );
}

/**
* Wenn in diesem Eintrag eine URL Definiert ist.
* Wird diese Adresse bei einem Doppel Klick von
* @ref prepareToExecute geprüft und direkt ein
* Prozess sofort gestartet.
*/
void CSSValidator::doubleClicked ( QListWidgetItem * item )
{
  if ( ! item->data ( Qt::UserRole ).isValid() )
    return;

  QUrl url = item->data ( Qt::UserRole ).toUrl();
  if ( prepareToExecute ( url ) )
    m_validator->validate();
  else
    warningItem ( i18n ( "CSS Validator says: Invalid Url and denies this request." ) );
}

/**
* Dieser slot wird aufgerufen wenn ein
* Prozess gestartet wurde.
*/
void CSSValidator::processTriggered()
{
  usedUrl = m_validator->getValidation();
  soupData.clear();
  setCursor ( Qt::WaitCursor );
  clearItems();
  QString message = i18n ( "Checking (%1). Please wait a little!" ).arg ( usedUrl );
  placeUrlItem ( message, QUrl ( usedUrl ) );
  m_menu->enableKillProcessAction ( m_validator->isRunning() );
}

/**
* Aufsteigend sortieren
*/
void CSSValidator::sortAscending()
{
  m_listWidget->sortItems ( Qt::AscendingOrder );
}

/**
* Absteigend sortieren
*/
void CSSValidator::sortDescending()
{
  m_listWidget->sortItems ( Qt::DescendingOrder );
}

/**
* Konfiguration öffnen
*/
void CSSValidator::openConfig()
{
  if ( cfg )
    openConfigurationDialog ();
}

/**
* Einen Eintrag erstellen in dem die URL als
* Benutzer Definierten datensatz mit eingefügt wird.
* Nur diese Einträge erlauben eine Benutzerdefinierte
* URL Überprüfung mit einem Maus Doppel Klick!
*/
void CSSValidator::placeUrlItem ( const QString &message, const QUrl &url )
{
  QListWidgetItem* item = new QListWidgetItem ( iconNotice, message );
  item->setData ( Qt::UserRole, url );
  item->setToolTip ( url.toString() );
  m_listWidget->addItem ( item );
}

/**
* Versucht eine übergeben Adresse sofort zu Überprüfen.
* Soll aber keinen laufenden Prozess beenden oder behindern!
* Falls ein Prozess schon am laufen ist wird er hier Ignoriert!
* Dieser slot ist Reserviert für Zugriffe der IDE!
*/
void CSSValidator::forceValidation ( const QUrl &url )
{
  if ( ! url.scheme().contains ( QRegExp ( "^(http|file)" ) ) )
    return;

  if ( prepareToExecute ( url ) )
    m_validator->validate();
}

/**
* Eintrag mit Schwerer- Fehlermeldung einfügen.
*/
void CSSValidator::criticalItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconCritical, message ) );
}

/**
* Eintrag mit Warnmeldung einfügen.
*/
void CSSValidator::warningItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconWarning, message ) );
}

/**
* Einfache Meldung einfügen.
*/
void CSSValidator::noticeItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconNotice, message ) );
}

/**
* Kontext Menü öffnen
*/
void CSSValidator::contextMenuEvent ( QContextMenuEvent *e )
{
  m_menu->exec ( e->globalPos() );
}

/**
* Dieser Listener sorgt dafür das - wenn kein Prozess
* am laufen ist die Sanduhr entfernt wird.
* Sollte Eigentlich nicht passieren ;)
*/
void CSSValidator::mouseMoveEvent ( QMouseEvent * e )
{
  Q_UNUSED ( e )
  if ( ! m_validator->isRunning() )
    setCursor ( Qt::ArrowCursor );
}

/**
* In diesem Slot wird die Url für die Überprüfung entgegen genommen.
* Dabei wird immer das Passwort und die Anker entfernt.
* Gleichzeitig geht ein Hinweis in das Listenfeld das eine Adresse
* zum Validieren zu vefügung steht.
* Zum abschluss wir die Adress an @ref Validator::setValidation weiter
* geleitet.
*/
void CSSValidator::addForValidation ( const QUrl &url )
{
  if ( ! url.scheme().contains ( QRegExp ( "^(http|file)" ) ) )
    return;

  QString remote = url.toString ( ( QUrl::RemovePassword | QUrl::RemoveFragment ) );
  if ( ! remote.isEmpty() )
  {
    QString display = url.toString ( QUrl::StripTrailingSlash | QUrl::RemoveFragment );
    QUrl remoteUrl ( remote );
    if ( isUnique ( remoteUrl ) )
      placeUrlItem ( i18n ( "To take in \"%1\" for Validation." ).arg ( display ), remoteUrl );
  }
}

/**
* Öffentlicher Slot für das leeren des Listenfensters
*/
void CSSValidator::clearItems()
{
  m_listWidget->clear();
}

/**
* Im fehlerfall die Meldungen abfangen und das Liste Fenster
* senden. Danach den Schreib- Lesemodus beenden!
*/
void CSSValidator::errors ( QProcess::ProcessError err )
{
  QString errtxt = m_validator->errorString();
  switch ( err )
  {
    case QProcess::FailedToStart:
      criticalItem ( i18n ( "FailedToStart (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Crashed:
      criticalItem ( i18n ( "Crashed (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Timedout:
      warningItem ( i18n ( "Timedout (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::WriteError:
      criticalItem ( i18n ( "WriteError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::ReadError:
      criticalItem ( i18n ( "ReadError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::UnknownError:
      criticalItem ( i18n ( "UnknownError (%1) ..." ).arg ( errtxt ) );
      break;
  }
  m_validator->close();
}

/**
* Wenn der Prozess beendet wurde nachsehen welcher status
* dieser zurück gegeben hat. Ist das beenden suaber verlaufen
* schliesse den Schreibmodus und gebe dem LesenModus 100
* Millisekunden Zeit für eine Antwort.
* Zum abschluss Schließe die Verbindung!
*/
void CSSValidator::exited ( int exitCode, QProcess::ExitStatus stat )
{
#ifdef XHTMLDBG_DEBUG_VERBOSE

  qDebug ( "(XHTMLDBG) Validator closed process with exit code: %d status: %d (%d)"
           , exitCode, stat, __LINE__ );

#else

  Q_UNUSED ( exitCode )

#endif

  switch ( stat )
  {
    case QProcess::NormalExit:
      m_validator->closeWriteChannel ();
      break;

    case QProcess::CrashExit:
      criticalItem ( i18n ( "Crashed see logfiles" ) );
      shutdownProcess();
      break;

    default:
      return;
  }
  shutdownProcess();
}

/**
* Methode zum öffnen des CSS Konfigurations Dialoges.
* Bei einem gültigen beenden werden die Umgebungswerte
* mit @ref Validator::setEnviromentVariable neu eingelesen.
*/
void CSSValidator::openConfigurationDialog()
{
  CSSConfigure* dialog = new CSSConfigure ( this, cfg );
  if ( dialog->exec() )
    m_validator->setEnviromentVariable ( cfg );

  delete dialog;
}

/**
* Bei Antworten des CSS-Validierers nach sehen ob es
* sich um ein XML-Deklaration handelt! Wenn nicht,
* verwerfen und die weiteren DatenStröme einlesen.
* In der Regel wirft der Validator 3 Datenströme.
* @li {...} Hinweis über die übergebene Konfiguration
* @li       Zur Trennnung einen Leeren Datenstrom
* @li <?xml Das XML-Dokument
* Wenn der Datenstrom eingelesen werden konnte dann diesen
* an die Methode @ref SoupReader::readReceivedXML weiterleiten.
* Gibt diese Methode true zurück den Lesespeicher wieder
* freigeben und die Mausanzeiger zurück auf normal stellen!
*/
void CSSValidator::readStandardReply ()
{
  QByteArrayMatcher match ( "{output=soap12" );
  QByteArray data = m_validator->readAllStandardOutput();
  if ( match.indexIn ( data, 0 ) != -1 || data.size() < 50 )
    return;

  soupData.append ( data );

  if ( m_validator->waitForFinished ( 100 ) )
  {
    if ( m_soupReader->readReceivedXML ( soupData, usedUrl ) )
      m_validator->closeReadChannel ( QProcess::StandardOutput );

    setCursor ( Qt::ArrowCursor );
    m_menu->enableKillProcessAction ( false );
  }
}

/**
* Einen laufenden Prozess beenden und die Liste leeren.
* Danach das Abbrechen Menü deaktivieren.
*/
void CSSValidator::shutdownProcess()
{
  if ( m_validator->isRunning() )
  {
    m_validator->closeReadChannel ( QProcess::StandardOutput );
    clearItems();
  }
  m_menu->enableKillProcessAction ( false );
}

CSSValidator::~CSSValidator()
{
}
