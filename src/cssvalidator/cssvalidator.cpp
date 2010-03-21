/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

/* QtCore */
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

CSSValidator::CSSValidator ( QWidget * parent, QSettings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconCritical ( QString::fromUtf8 ( ":/icons/critical.png" ) )
{
  setObjectName ( QLatin1String ( "cssvalidator" ) );
  setWindowTitle ( trUtf8 ( "CSS Validation" ) );
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

  connect ( m_validator, SIGNAL ( started() ), this, SLOT ( processTriggered() ) );

  // Signale der XML Analyse
  connect ( m_soupReader, SIGNAL ( parserError ( const QString & ) ),
            this, SLOT ( criticalItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( warnings ( const QString & ) ),
            this, SLOT ( warningItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( congratulation ( const QString & ) ),
            this, SLOT ( noticeItem ( const QString & ) ) );

  // Kontext Menü Aktionen
  connect ( m_menu, SIGNAL ( check() ), m_validator, SLOT ( validate() ) );
  connect ( m_menu, SIGNAL ( dropout() ), m_validator, SLOT ( kill() ) );
  connect ( m_menu, SIGNAL ( ascending() ), this, SLOT ( sortAscending() ) );
  connect ( m_menu, SIGNAL ( descending() ), this, SLOT ( sortDescending() ) );
  connect ( m_menu, SIGNAL ( clearlist() ), this, SLOT ( clearItems() ) );
  connect ( m_menu, SIGNAL ( configure() ), this, SLOT ( openConfig() ) );
}

/**
* Wenn in diesem Eintrag eine URL Definiert ist.
* Wird diese URL bei einem Doppel Klick geprüft.
*/
void CSSValidator::doubleClicked ( QListWidgetItem * item )
{
  if ( item->data ( Qt::UserRole ).isValid() )
  {
    QString remote = item->data ( Qt::UserRole ).toUrl().toString();
    if ( remote.isEmpty() )
      return;

    if ( m_validator->isRunning() )
    {
      warningItem ( trUtf8 ( "Rejected - CSS Validator is busy ..." ) );
      return;
    }
    if ( m_validator->setValidation ( remote ) )
      m_validator->validate();
    else
      warningItem ( trUtf8 ( "CSS Validator says: Invalid Url and denies this request." ) );
  }
}

/**
* Wird aufgerufen wenn ein Prozess gestartet wird.
*/
void CSSValidator::processTriggered()
{
  setCursor ( Qt::WaitCursor );
  clearItems();
  QString message = trUtf8 ( "Checking (%1). Please wait a little!" ).arg ( m_validator->getValidation() );
  noticeItem ( message );
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
  item->setData ( Qt::ToolTipRole, url.toString() );
  m_listWidget->addItem ( item );
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
* Wenn kein Prozess am laufen ist die Sanduhr entfernen.
*/
void CSSValidator::mouseMoveEvent ( QMouseEvent * e )
{
  Q_UNUSED ( e )
  if ( ! m_validator->isRunning() )
    setCursor ( Qt::ArrowCursor );
}

/**
* In diesem Slot wird die Url für die Überprüfung entgegen
* genommen. Dabei wird immer Passwort und Fragment entfernt.
* Gleichzeitig geht ein Hinweis in das Listenfeld.
*
*/
void CSSValidator::addForValidation ( const QUrl &url )
{
  QString remote = url.toString ( ( QUrl::RemovePassword | QUrl::RemoveFragment ) );
  if ( ! remote.isEmpty() )
  {
    QString display = url.toString ( QUrl::StripTrailingSlash | QUrl::RemoveFragment );
    placeUrlItem ( trUtf8 ( "To take in \"%1\" for Validation." ).arg ( display ), QUrl ( remote ) );
    m_validator->setValidation ( remote );
  }
}

void CSSValidator::clearItems()
{
  m_listWidget->clear();
}

void CSSValidator::errors ( QProcess::ProcessError err )
{
  QString errtxt = m_validator->errorString();
  switch ( err )
  {
    case QProcess::FailedToStart:
      criticalItem ( trUtf8 ( "FailedToStart (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Crashed:
      criticalItem ( trUtf8 ( "Crashed (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Timedout:
      warningItem ( trUtf8 ( "Timedout (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::WriteError:
      criticalItem ( trUtf8 ( "WriteError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::ReadError:
      criticalItem ( trUtf8 ( "ReadError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::UnknownError:
      criticalItem ( trUtf8 ( "UnknownError (%1) ..." ).arg ( errtxt ) );
      break;
  }
  m_validator->close();
}

void CSSValidator::exited ( int exitCode, QProcess::ExitStatus stat )
{
  Q_UNUSED ( exitCode )
  switch ( stat )
  {
    case QProcess::NormalExit:
      m_validator->waitForFinished ( 50 );
      break;

    case QProcess::CrashExit:
      criticalItem ( trUtf8 ( "Crashed see logfiles" ) );
      break;

    default:
      return;
  }
  m_validator->close();
}

void CSSValidator::openConfigurationDialog()
{
  CSSConfigure* dialog = new CSSConfigure ( this, cfg );
  if ( dialog->exec() )
    m_validator->setEnviromentVariable ( cfg );

  delete dialog;
}

void CSSValidator::readInputData ( const QByteArray &data )
{
  QString soup ( data );
  if ( soup.isEmpty() || soup.contains ( QRegExp ( "^[\\s]?\\{.+\\}[\\s\\n\\r]" ) ) )
    return;

  if ( m_soupReader->readReceivedXML ( soup.trimmed() ) )
    m_validator->closeReadChannel ( QProcess::StandardOutput );
}

/**
* Der css-validator
*/
void CSSValidator::readStandardReply ()
{
  QByteArray data = m_validator->readAllStandardOutput();
  if ( data.startsWith ( QByteArray ( "{" ) ) || data.size() < 50 )
    return;

  readInputData ( data );
  setCursor ( Qt::ArrowCursor );
}

CSSValidator::~CSSValidator()
{
  if ( m_validator->isRunning() )
    m_validator->kill();
}
