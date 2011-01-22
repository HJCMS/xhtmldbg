/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

/* QtNetwork */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

Page::Page ( NetworkAccessManager * manager, QObject * parent )
    : QWebPage ( parent )
    , m_netManager ( manager )
    , xhtml ( QString::null )
{
  setObjectName ( "page" );
  // Nicht bekannte Datentypen an SIGNAL unsupportedContent übergeben.
  setForwardUnsupportedContent ( true );

  setNetworkAccessManager ( m_netManager );

#ifdef HAVE_NPPLOADER
  // NPP Plugin Factory
  setPluginFactory ( new NPPLoader ( this ) );
#endif

  reply = 0x00;

  // merge ShortCuts with Application Window
  action ( QWebPage::Reload )->setShortcut ( QKeySequence::Refresh );
  action ( QWebPage::Back )->setShortcut ( QKeySequence::Back );
  action ( QWebPage::Forward )->setShortcut ( QKeySequence::Forward );
  action ( QWebPage::Copy )->setShortcut ( QKeySequence::Copy );
  action ( QWebPage::Copy )->setIcon ( QIcon::fromTheme ( "edit-copy" ) );

  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );
  connect ( this, SIGNAL ( unsupportedContent ( QNetworkReply * ) ),
            this, SLOT ( unsupportedContentRequest ( QNetworkReply * ) ) );
  connect ( this, SIGNAL ( downloadRequested ( const QNetworkRequest & ) ),
            this, SLOT ( downloadContentRequest ( const QNetworkRequest & ) ) );
}

/**
* Sende alle von Webkit generierten JavaScript Meldungen an das Hauptfenster
*/
void Page::javaScriptConsoleMessage ( const QString & m, int l, const QString & id )
{
  if ( m.isEmpty() )
    return;

  QByteArray cmd = QByteArray::fromPercentEncoding ( m.toAscii() );
  QString message ( cmd );
  message.remove ( QRegExp ( "[\\/\n]+" ) );

  if ( l > 0 )
    message.append ( trUtf8 ( " Line: %1" ).arg ( QString::number ( l ) ) );

  if ( ! id.isEmpty() )
    message.append ( trUtf8 ( " Document: %1" ).arg ( id ) );

  xhtmldbgmain::instance()->mainWindow()->setJavaScriptMessage ( Qt::escape ( message ) );
}

/**
* Zeigt ein Dialogfenster mit einem Eingabefeld, einem OK-Button und einem Abbrechen-Button an.
* Der Anwender kann in diesem Fenster beim Eingabefeld Werte setzen.
* Die JavaScript Methode prompt() gibt diesen eingegebenen Wert zurück.
* So lassen sich Anwendereingaben im Script weiterverarbeiten.
* Es werden mindestens zwei Parameter erwartet:
*   @li Aufforderungstext = Text, der beschreibt, was der Anwender eingeben soll.
*   @li Feldvorbelegung = val, mit dem das Eingabefeld vorbelegt wird.
*
* @param frame Aktueller Frame
* @param text  Aufforderungstext
* @param val   Feldvorbelegung (Kann null sein)
* @param inp   Referenz auf den Rückgabewert!
*/
bool Page::javaScriptPrompt ( QWebFrame * frame, const QString &text, const QString &val, QString * inp )
{
  bool b = false;
  QString status = trUtf8 ( "JavaScript Prompt: " );
  QString path = Qt::escape ( frame->requestedUrl().path() );
  status.append ( Qt::escape ( text ) );

#ifndef QT_NO_INPUTDIALOG
  QString x = QInputDialog::getText ( view(), path, Qt::escape ( text ), QLineEdit::Normal, val, &b );
  if ( b && inp )
  {
    status.append ( " ("+ Qt::escape ( x ) + ")" );
    *inp = x;
  }
#endif

  javaScriptConsoleMessage ( status, 0, path );

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
  xhtmldbgmain::instance()->mainWindow()->setSource ( xhtml );
  return true;
}

/**
* Wenn der unbekannte Datentype einen "Content-Type" enthält wird
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
* Und zusätzlich eine Meldung an das Nachrichten Fenster senden.
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
  // und Gültigkeit der Parameter und ihrer Werte geprüft!
  if ( loader->setConfiguration ( params, values ) )
  {
    QString message = trUtf8 ( "(x-qt-plugin) ClassID: %1 %2" ).arg ( id, url.toString() );
    internalMessanger ( message );
  }
  return loader->getUiComponent ( view() );
}
#endif

/**
* Nachrichten Übermittler an das Hauptfenster
*/
void Page::internalMessanger ( const QString &m )
{
  xhtmldbgmain::instance()->mainWindow()->setApplicationMessage ( m );
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
