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

#include "viewer.h"
#include "webviewer.h"
#include "page.h"
#include "xhtmldbg.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "cookieacceptdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QHelpEvent>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QToolTip>

/* QtWebKit */
#include <QtWebKit/QWebHitTestResult>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

Viewer::Viewer ( QWidget * parent )
    : QWebView ( parent )
    , cookieAlreadyAdd ( false )
{
  if ( objectName().isEmpty() )
    setObjectName ( "webviewer" );

  setContentsMargins ( 0, 0, 0, 0 );
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  NetworkAccessManager* netManager = xhtmldbg::instance()->networkAccessManager();
  cookieManager = xhtmldbg::instance()->cookieManager();

  m_page = new Page ( netManager, this );
  setPage ( m_page );

  /* signal cookie nachfrage dialog */
  connect ( cookieManager, SIGNAL ( cookiesRequest ( const QUrl & ) ),
            this, SLOT ( cookiesRequest ( const QUrl & ) ) );
  /* bei jeder verknüpfungs anfrage dem netzwerk manager
  * mitteilen welches die orignal anfrage adresse */
  connect ( this, SIGNAL ( linkClicked ( const QUrl & ) ),
            netManager, SLOT ( setUrl ( const QUrl & ) ) );
  /* start maus-sanduhr */
  connect ( this, SIGNAL ( loadStarted () ), this, SLOT ( cursorwait () ) );
  /* stop maus-sanduhr */
  connect ( this, SIGNAL ( loadFinished ( bool ) ), this, SLOT ( cursorFinished ( bool ) ) );
  /* bei überfahren mir der maus informationen der verknüpfung ausgeben. */
  connect ( m_page, SIGNAL ( linkHovered ( const QString &, const QString &, const QString & ) ),
            this, SLOT ( linkInfos ( const QString &, const QString &, const QString & ) ) );
}

/**
* Öffnet den Keks Dialog und sendet danach das
* Signal @ref CookieManager::reload
*/
void Viewer::openCookieRequestDialog ( const QUrl &cookieUrl )
{
  if ( ! cookieManager )
    return;

  CookieAcceptDialog cookiediag ( cookieUrl, this );
  if ( cookiediag.exec() )
    cookieManager->reload();
}

/**
* Mit dieser Methode wird das Signal für die
* Klasse @class DomTree aufgearbeitet.
* Mit hitTestContent wird vom aktuellen Rahmen
* die Position des WebElementes abgegriffen.
*/
void Viewer::findChildNode ( const QPoint &p )
{
  if ( ! p.isNull() )
    emit hitTestResult ( page()->currentFrame()->hitTestContent ( p ).element() );
}

/**
* Setze die Maus Sanduhr
* Wird immer dann aufgerufen wenn ein
* Signal loadStarted abgegeben wird.
*/
void Viewer::cursorwait ()
{
  setCursor ( Qt::WaitCursor );
}

/**
* Entferne die Maus Sanduhr
* Wird immer dann aufgerufen wenn ein
* Signal loadFinished abgegeben wird.
*/
void Viewer::cursorFinished ( bool )
{
  setCursor ( Qt::ArrowCursor );
  if ( page()->bytesReceived() >= 100 )
    emit totalBytes ( page()->bytesReceived() );
}

/**
* Konvertiere Lesezeichen Signal @ref addBookmark
* von @ref contextMenuEvent Menu
*/
void Viewer::bookmark()
{
  emit addBookmark ( url(), title() );
}

/**
* Füge Lesezeichen Aktion mit ein.
*/
void Viewer::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* menu = m_page->createStandardContextMenu();
  QAction* add = menu->addAction ( trUtf8 ( "Bookmark" ) );
  add->setObjectName ( QLatin1String ( "addbookmarkaction" ) );
  add->setIcon ( QIcon::fromTheme ( QLatin1String ( "bookmark-new" ) ) );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( bookmark() ) );
  menu->exec ( e->globalPos() );
  delete menu;
}

/**
* Event für @ref findChildNode und dem Auffinden
* von WebElementen für @class DomTree
*/
void Viewer::mousePressEvent ( QMouseEvent * ev )
{
  findChildNode ( ev->pos() );
  QWebView::mousePressEvent ( ev );
}

/**
* Alle Fenster anfragen werden in einem neuen
* Tab gesetzt und nicht ein zweites Fenster geöffnet !
*/
Viewer* Viewer::createWindow ( QWebPage::WebWindowType t )
{
  if ( QWebPage::WebModalDialog == t )
    return this;

  QObject* w = this->parent();
  while ( w )
  {
    if ( WebViewer* mw = qobject_cast<WebViewer*> ( w ) )
    {
      Viewer *view = new Viewer ( mw );
      mw->addViewerTab ( view );
      return view;
    }
    w = w->parent();
  }
  return this;
}

/**
* Dieser Slot wird vom Signal cookiesRequest aufgerufen.
* Es wird zuerst Überprüft ob @ref cookieAlreadyAdd bereits
* existiert oder die URL Identisch mit Seiten Url ist!
* 3.Anbieter werden generell abgwewiesen!
*/
void Viewer::cookiesRequest ( const QUrl &u )
{
  QString pageHost ( url().host() );
  QString cookieHost ( u.host() );
  if ( pageHost.contains ( cookieHost ) && ! cookieAlreadyAdd )
  {
    cookieAlreadyAdd = true;
    openCookieRequestDialog ( u );
  }
}

/**
* Dieser SLOT wird von @class KeywordsToolBar benötigt
* um die gesuchten Schlüsselwörter hervor zu heben.
*/
void Viewer::findKeyword ( const QString &word )
{
  QString wA, wB, wC, wD, wE;
  QString message = trUtf8 ( "SEO Result for \"%1\": " ).arg ( word );

  findText ( word, QWebPage::HighlightAllOccurrences );
  QString body = bodyContent();
  wA = QString::number ( body.count ( word, Qt::CaseInsensitive ) );
  wB = QString::number ( body.count ( word, Qt::CaseSensitive ) );
  wC = QString::number ( body.count ( QRegExp ( "\\b"+word+"\\b", Qt::CaseInsensitive ) ) );

  QStringList meta = m_page->keywordMetaTagItems();
  wD = QString::number ( meta.first().count ( word, Qt::CaseInsensitive ) );
  wE = QString::number ( meta.last().count ( word, Qt::CaseInsensitive ) );
  message.append (
      trUtf8 ( "Simple: %1; Case Sensitive: %2; Word boundary: %3; Meta Keywords: %4; Meta Description: %5" )
      .arg ( wA, wB, wC, wD, wE )
  );

  xhtmldbg::instance()->mainWindow()->setApplicationMessage ( message );
}

/**
* Slot auf QWebView::setUrl
*/
void Viewer::openUrl ( const QUrl &url )
{
  setUrl ( url );
}

/**
* Such im aktuellen Frame mit QwebElement nach dem BODY
* Tag und gibt dieses als text Zurück.
*/
const QString Viewer::bodyContent()
{
  return m_page->currentFrame()->findFirstElement ( QLatin1String ( "body" ) ).toPlainText();
}

/**
* Gibt den Original von page() Quelltext zurück!
*/
const QString Viewer::source()
{
  return m_page->xhtmlSource();
}

/**
* Erstelle Extras für die Anzeige erweiterte Verknüpfungs Informationen.
*/
void Viewer::linkInfos ( const QString &link, const QString &title, const QString & )
{
  if ( ! link.isEmpty() )
    emit statusBarMessage ( link );

  if ( title.isEmpty() )
    return;

  QPoint point = cursor ().pos();
  QToolTip::showText ( point, title, this, QRect ( point, QSize ( 20, 20 ) ) );
}

Viewer::~Viewer()
{
}
