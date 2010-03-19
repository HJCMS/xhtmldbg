/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"
#include "viewer.h"
#include "xhtmldbg.h"
#include "networkaccessmanager.h"
#include "jsmessanger.h"

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

/* QtNetwork */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

Page::Page ( NetworkAccessManager * manager, QObject * parent )
    : QWebPage ( parent )
    , m_netManager ( manager )
    , xhtml ( QString::null )
{
  setObjectName ( "page" );

  setNetworkAccessManager ( m_netManager );

  reply = 0x00;

  // merge ShortCuts with Application Window
  action ( QWebPage::Reload )->setShortcut ( QKeySequence::Refresh );
  action ( QWebPage::Back )->setShortcut ( QKeySequence::Back );
  action ( QWebPage::Forward )->setShortcut ( QKeySequence::Forward );
  action ( QWebPage::Copy )->setShortcut ( QKeySequence::Copy );
  action ( QWebPage::Copy )->setIcon ( QIcon::fromTheme ( "edit-copy" ) );

  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );
  connect ( this, SIGNAL ( downloadRequested ( const QNetworkRequest & ) ),
            this, SLOT ( unsupportedContent ( const QNetworkRequest & ) ) );
}

/**
* Sende alle von Webkit generierten JavaScript Meldungen an das Hauptfenster
*/
void Page::javaScriptConsoleMessage ( const QString & m, int l, const QString & id )
{
  Q_UNUSED ( l )
  Q_UNUSED ( id )
  if ( m.isEmpty() )
    return;

  QByteArray cmd = QByteArray::fromPercentEncoding ( m.toAscii() );
  QString message ( cmd );
  message.remove ( QRegExp ( "[\\/\n]+" ) );

  xhtmldbg::instance()->mainWindow()->setJavaScriptMessage ( message );
}

/**
* Versuche mit dem Datenkopf Content-Type den Zeichensatz zu ermitteln.
*/
QTextCodec* Page::fetchHeaderEncoding ( QNetworkReply * reply )
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
* Hier wird der Datenstrom ausgelesen und in den
* passenden Zeichnsatz gesetzt. Danach geht es
* weiter an @ref Window::setSource
*/
bool Page::prepareContent ( QNetworkReply * dev )
{
  xhtml.clear();
  QByteArray data = dev->readAll();
  if ( data.isEmpty() )
    return false;

  QTextCodec* codec = QTextCodec::codecForHtml ( data, fetchHeaderEncoding ( dev ) );
  xhtml = codec->toUnicode ( data );
  xhtmldbg::instance()->mainWindow()->setSource ( xhtml );
  return true;
}

/**
* Wenn eine Anfrage nicht verabeitet werden kann,
* gebe eine Meldung an das Hauptfenster weiter.
*/
void Page::unsupportedContent ( QNetworkReply * reply )
{
  QUrl url ( reply->url() );
  QString message = trUtf8 ( "Unsupported Request: %1" ).arg ( url.toString() );
  xhtmldbg::instance()->mainWindow()->setApplicationMessage ( message );
}

/**
* In diesem Programm werden keine Plugins oder Donwloads
* durchgeführt, es ist ein Quelltext Debugger.
* Deshalb an dieser Stelle die entsprechende Meldung.
*/
void Page::unsupportedContent ( const QNetworkRequest &request )
{
  QString url = request.url().toString();
  QString message = trUtf8 ( "Unsupported Request: %1" ).arg ( url );
  xhtmldbg::instance()->mainWindow()->setApplicationMessage ( message );
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
    if ( mimeType.contains ( "text/html" ) &&  prepareContent ( reply ) )
      emit getUrl ( reply->url() );
  }
}

/**
* Lege eine Kopie der GET anfragen für die Quelltext darstellung an.
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
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeBackOrForward:
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeReload:
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeFormSubmitted:
      // The user activated a submit button for an HTML form.
      break;

    case QWebPage::NavigationTypeFormResubmitted:
      // An HTML form was submitted a second time.
      return false;

    case QWebPage::NavigationTypeOther:
      // typically a external reload or header location href request
      // after this a getRequest is used by WebKit
      break;

    default:
      break;
  }
  return b;
}

/**
* Alle ausgewählten Texte an das Unix Clipboard weiter geben.
*/
void Page::triggerSelections()
{
  QString txt = selectedText();
  if ( txt.isEmpty() )
    return;

  QApplication::clipboard()->setText ( txt );
}

/**
* Suche im Aktuellen Dokument nach META Tags mit dem Prädikat name="keywords|description"
* Und gebe eine Liste der Schlüsselwörter zurück.
* Damit es nicht wegen fehlerhaften Zeigern zu abstürtzen kommt werden immer 2 Inhalte erzeugt.
* Dabei ist first() die "keywords" und last() die "description".
* Fehlende Inhalte werden durch @e "NULL" ersetzt.
*/
const QStringList Page::keywordMetaTagItems()
{
  QStringList words;
  QString missed = QLatin1String ( "NULL" );
  QMultiMap<QString, QString> map = currentFrame()->metaData ();
  if ( map.values ( "keywords" ).isEmpty() )
    words << missed;
  else
    words << map.values ( "keywords" );

  if ( map.values ( "description" ).isEmpty() )
    words << missed;
  else
    words << map.values ( "description" );

  return words;
}

/**
* Temporärer Quelltext wird in @ref prepareContent erstellt.
*/
const QString Page::xhtmlSource()
{
  return xhtml;
}

Page::~Page()
{
  xhtml.clear();
}
