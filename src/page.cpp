/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2013
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

#include "page.h"
#include "viewer.h"
#include "xhtmldbgmain.h"
#include "networkaccessmanager.h"
#include "downloadmanager.h"
#include "jsmessanger.h"
#include "javascriptpopup.h"
/* QtUiTools */
#ifdef HAVE_QTUITOOLS
# include "uitoolsloader.h"
#endif
/* NPPLoader */
#ifdef HAVE_NPPLOADER
# include "npploader.h"
#endif

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QClipboard>
#include <QtGui/QIcon>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QTextDocument>

/* QtWebKit */
#include <QtWebKit/QWebDatabase>
#include <QtWebKit/QWebHistoryItem>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>

/* QtNetwork */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

Page::Page ( NetworkAccessManager * manager, QObject * parent )
    : KWebPage ( parent, ( KWebPage::KPartsIntegration | KWebPage::KWalletIntegration ) )
    , m_netManager ( manager )
    , reply ( 0 )
{
  setObjectName ( "page" );
  // Nicht bekannte Datentypen an SIGNAL unsupportedContent ??bergeben.
  setForwardUnsupportedContent ( true );
  setAllowExternalContent ( true );
  setLinkDelegationPolicy ( QWebPage::DontDelegateLinks );
  setNetworkAccessManager ( m_netManager );

#ifdef HAVE_NPPLOADER
  // NPP Plugin Factory
  NPPLoader* factory = new NPPLoader ( this );
  setPluginFactory ( factory );
  factory->refreshPlugins();
#endif

  // merge ShortCuts with Application Window
  action ( QWebPage::Reload )->setShortcut ( QKeySequence::Refresh );
  action ( QWebPage::Back )->setShortcut ( QKeySequence::Back );
  action ( QWebPage::Forward )->setShortcut ( QKeySequence::Forward );
  action ( QWebPage::Copy )->setShortcut ( QKeySequence::Copy );
  action ( QWebPage::Copy )->setIcon ( QIcon::fromTheme ( "edit-copy" ) );

  // NOTE localReplySource muss auch in @class Window gesetzt werden!
  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );

  connect ( this, SIGNAL ( unsupportedContent ( QNetworkReply * ) ),
            this, SLOT ( unsupportedContentRequest ( QNetworkReply * ) ) );

  connect ( this, SIGNAL ( downloadRequested ( const QNetworkRequest & ) ),
            this, SLOT ( downloadContentRequest ( const QNetworkRequest & ) ) );

  connect ( this, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( onReadyLoadPage ( bool ) ) );
}

/**
* Versuche mit dem Datenkopf Content-Type den Zeichensatz zu ermitteln.
*/
QTextCodec* Page::fetchHeaderEncoding ( QNetworkReply * reply ) const
{
  QString encoding ( "UTF-8" );
  if ( reply )
  {
    QByteArray cType = reply->rawHeader ( QByteArray ( "Content-Type" ) );
    if ( ! cType.isEmpty() )
    {
      QString Charset ( cType );
      foreach ( QString param, Charset.split ( QRegExp ( "[\\s ]?;[\\s ]?" ) ) )
      {
        param.trimmed();
        if ( param.contains ( "charset", Qt::CaseInsensitive ) )
        {
          QString buf = param.split ( QRegExp ( "[\\s ]?=[\\s ]?" ) ).last();
          encoding = buf.toUpper();
          break;
        }
      }
    }
  }
  return QTextCodec::codecForName ( encoding.toAscii() );
}

/**
* Hier wird der Datenstrom ausgelesen und in den passenden Zeichnsatz gesetzt.
* Danach geht es weiter an @ref Window::setSource
* @note Diese Methode reagiert nur bei einem onClick oder laden innerhalb dieser Seite.
*/
bool Page::prepareContent ( QNetworkReply * dev )
{
  QByteArray data = dev->readAll();
  if ( data.isEmpty() )
    return false;

#ifdef DEBUG_VERBOSE
  QTextCodec* codec = QTextCodec::codecForHtml ( data, fetchHeaderEncoding ( dev ) );
  qDebug() << "\n" << codec->toUnicode ( data ) << "\n";
#endif

  return true;
}

/** Sende alle von Webkit generierten JavaScript Meldungen an das Hauptfenster */
void Page::javaScriptConsoleMessage ( const QString &m, int l, const QString &id )
{
  if ( m.isEmpty() || id.isEmpty() )
    return;

  QString message ( m.trimmed() );
  message.replace ( QRegExp ( "[\r\n]+" ), QString(" ") );

  if ( l > 0 )
    message.append ( i18n ( " Line: %1" ).arg ( QString::number ( l ) ) );

  if ( ! id.isEmpty() )
    message.append ( i18n ( " Document: %1" ).arg ( id ) );

  xhtmldbgmain::instance()->mainWindow()->setJavaScriptMessage ( Qt::escape ( message ) );
}

/**
* Wenn die Seite fertig geladen ist...
* \li Suche nach Formularen f??r Formular Manager
* \li Suche \b Testweise nach HTML5 Datenbank restriktionen
*/
void Page::onReadyLoadPage ( bool b )
{
  if ( ! b )
    return;

  QWebFrame* frame = mainFrame();
  QWebElement bodyElement = frame->documentElement().findFirst ( "body" );
  if ( bodyElement.isNull() )
    return;

  if ( bodyElement.findAll ( "FORM" ).count() > 0 )
  {
#ifdef DEBUG_VERBOSE
    qDebug() << Q_FUNC_INFO << "TODO  FormManager" << bodyElement.findAll ( "FORM" ).count();
#endif
  }
}

/**
* Diese Virtuelle Methode m??ssen wir verwenden weil QWebPage seltsamer Weise
* bei @ref acceptNavigationRequest kein @em NavigationTypeBackOrForward verarbeitet.
* Hier werden also nur (Back|Forward|Reload) aktionen abgefangen!
* @todo FIXME acceptNavigationRequest => NavigationTypeBackOrForward
*/
void Page::triggerAction ( QWebPage::WebAction action, bool checked )
{
  QWebHistoryItem hist = history()->currentItem();
  switch ( action )
  {
    case QWebPage::Back:
      hist = history()->backItem();
      break;

    case QWebPage::Forward:
      hist = history()->forwardItem();
      break;

    case QWebPage::Reload:
      hist = history()->currentItem();
      break;

    case QWebPage::ReloadAndBypassCache:
      hist = history()->currentItem();
      break;

    default:
      qWarning ( "Unknown Action triggered (%d)", action );
      QWebPage::triggerAction ( action, checked );
      return;
  }

  if ( hist.isValid() )
  {
    history()->goToItem ( hist );
    emit urlAction ( hist.url() );
  }
}

/**
* Zeigt ein Dialogfenster mit einem Eingabefeld, einem OK-Button und einem Abbrechen-Button an.
* Der Anwender kann in diesem Fenster beim Eingabefeld Werte setzen.
* Die JavaScript Methode prompt() gibt diesen eingegebenen Wert zur??ck.
* So lassen sich Anwendereingaben im Script weiterverarbeiten.
* Es werden mindestens zwei Parameter erwartet:
*   @li Aufforderungstext = Text, der beschreibt, was der Anwender eingeben soll.
*   @li Feldvorbelegung = val, mit dem das Eingabefeld vorbelegt wird.
*
* @param frame Aktueller Frame
* @param text  Aufforderungstext
* @param val   Feldvorbelegung (Kann null sein)
* @param inp   Referenz auf den R??ckgabewert!
*/
bool Page::javaScriptPrompt ( QWebFrame * frame, const QString &text, const QString &val, QString * inp )
{
  bool b = false;
  QString status = i18n ( "JavaScript Prompt: " );
  QString path = Qt::escape ( frame->requestedUrl().path() );
  status.append ( Qt::escape ( text ) );

  QString x = QInputDialog::getText ( view(), path, Qt::escape ( text ), QLineEdit::Normal, val, &b );
  if ( b && inp )
  {
    status.append ( " ("+ Qt::escape ( x ) + ")" );
    *inp = x;
  }
  return b;
}

/**
* JavaScript Fehler Meldung ausgeben
*/
void Page::javaScriptAlert ( QWebFrame * frame, const QString &message )
{
  QString path = frame->requestedUrl().path();
  javaScriptConsoleMessage ( message, 0, path );
  QMessageBox::warning ( view(), path, Qt::escape ( message ) );
}

/**
* Wenn der unbekannte Datentype einen "Content-Type" enth??lt wird
* die anfrage an den DonwloadManager weitergeleitet.
* @note Diese Methode arbeitet nur wenn @ref QWebPage::setForwardUnsupportedContent true ist.
*/
void Page::unsupportedContentRequest ( QNetworkReply * nr )
{
  QUrl url ( nr->request().url().toString ( QUrl::RemovePassword ), QUrl::StrictMode );
  if ( nr->hasRawHeader ( "Content-Type" ) && url.isValid() )
    downloadContentRequest ( nr->request() );
}

/**
* Download anfragen an den DonwloadManager weiter leiten.
* Und zus??tzlich eine Meldung an das Nachrichten Fenster senden.
*/
void Page::downloadContentRequest ( const QNetworkRequest &request )
{
  xhtmldbgmain::instance()->mainWindow()->downloadRequest ( request );
}

/**
* Wenn eine GET Anfrage erfolgreich verlaufen ist suche in der
* Antwort den Datenkopf ContentTypeHeader und gebe diese weiter
* an die Methode @ref prepareContent
*/
void Page::replyFinished()
{
  if ( reply )
  {
    QString mimeType = reply->header ( QNetworkRequest::ContentTypeHeader ).toString();
    if ( mimeType.contains ( "text/html" ) )
      prepareContent ( reply );
  }
}

/**
* Lege eine Kopie der GET anfragen f??r die Quelltext darstellung an.
* @note POST kann nur vom Netzwerk Manager verarbeitet werden.
*/
bool Page::acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest &request,
                                     QWebPage::NavigationType type )
{
  bool b = QWebPage::acceptNavigationRequest ( frame, request, type );
  if ( ! b )
    return b;

  switch ( type )
  {
    case QWebPage::NavigationTypeLinkClicked:
    {
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      b = true;
    }
    break;

    case QWebPage::NavigationTypeBackOrForward:
    {
      // Navigation to a previously shown document in the back or forward history is requested.
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      b = true;
    }
    break;

    case QWebPage::NavigationTypeReload:
    {
      // The user activated the reload action.
      if ( request.url().toString().contains ( "about:" ) )
        return false;

      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      b = true;
    }
    break;

    case QWebPage::NavigationTypeFormSubmitted:
    {
      if ( mainFrame()->documentElement().findAll ( "FORM" ).count() > 0 )
        qDebug() << Q_FUNC_INFO << "TODO FormManager";

      b = true;
    }
    break;

    case QWebPage::NavigationTypeFormResubmitted:
    {
      internalMessanger ( i18n ( "Action rejected: HTML form was submitted a second time." ) );
#ifdef DEBUG_VERBOSE
      foreach ( QByteArray arr, request.rawHeaderList() )
      {
        qDebug() << Q_FUNC_INFO << arr << request.rawHeader ( arr );
      }
#endif
    }
    return false;

    case QWebPage::NavigationTypeOther:
      break;

    default:
      break;
  }
  return b;
}

#ifdef HAVE_QTUITOOLS
/**
* QT's Internes Blugin Interface!
*/
QObject* Page::createPlugin ( const QString &id, const QUrl &url,
                              const QStringList &params, const QStringList &values )
{
  UiToolsLoader* loader = new UiToolsLoader ( id, url, view() );
  connect ( loader, SIGNAL ( message ( const QString & ) ),
            this, SLOT ( internalMessanger ( const QString & ) ) );

  if ( ! params.contains ( "classid" ) || ! params.contains ( "name" ) )
    return loader->displayFailWidget ( view() );

  // Zuerst die Konfiguration setzen, denn es werden auch die Anzahl
  // und G??ltigkeit der Parameter und ihrer Werte gepr??ft!
  if ( loader->setConfiguration ( params, values ) )
  {
    QString message = i18n ( "(x-qt-plugin) ClassID: %1 %2" ).arg ( id, url.toString() );
    internalMessanger ( message );
  }
  return loader->getUiComponent ( view() );
}
#endif

/**
* Nachrichten ??bermittler an das Hauptfenster
*/
void Page::internalMessanger ( const QString &m )
{
  xhtmldbgmain::instance()->mainWindow()->setApplicationMessage ( m );
}

/**
* Alle ausgew??hlten Texte an das Unix Clipboard weiter geben.
*/
void Page::triggerSelections()
{
  QString txt = selectedText();
  if ( txt.isEmpty() )
    return;

  QApplication::clipboard()->setText ( txt );
}

/**
* Suche im Aktuellen Dokument nach META Tags mit dem Pr??dikat name="keywords|description"
* Und gebe eine Liste der Schl??sselw??rter zur??ck.
* Damit es nicht wegen fehlerhaften Zeigern zu abst??rtzen kommt werden immer 2 Inhalte erzeugt.
* Dabei ist first() die "keywords" und last() die "description".
* Fehlende Inhalte werden durch @e "0" ersetzt.
*/
const QStringList Page::keywordMetaTagItems()
{
  QStringList words;
  QMultiMap<QString, QString> map = currentFrame()->metaData ();
  if ( map.values ( "keywords" ).isEmpty() )
    words << "0";
  else
    words << map.values ( "keywords" );

  if ( map.values ( "description" ).isEmpty() )
    words << "0";
  else
    words << map.values ( "description" );

  return words;
}

Page::~Page()
{}
