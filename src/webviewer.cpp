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

#include "webviewer.h"
#include "viewer.h"
#include "historymanager.h"
#include "historyitem.h"
#include "websettings.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QTabBar>
#include <QtGui/QToolButton>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

WebViewer::WebViewer ( QWidget * parent )
    : QTabWidget ( parent )
    , m_wcfg ( new WebSettings ( this ) )
    , tabIcon ( QIcon::fromTheme ( "xhtmldbg" ) )
{
  setObjectName ( "webviewer" );
  setContentsMargins ( 0, 0, 0, 0 );
  setFocusPolicy ( Qt::StrongFocus );
  setTabsClosable ( true );
  setTabCornerButton();
  setBackgroundRole ( QPalette::NoRole );

  m_viewer = new Viewer ( this );
  m_viewer->setObjectName ( "webviewer_startpage" );
  addViewerTab ( m_viewer );

  connect ( this, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( pretended ( int ) ) );

  connect ( this, SIGNAL ( tabCloseRequested ( int ) ),
            this, SLOT ( closeViewerTab ( int ) ) );
}

/** Eckenknöpfe für neue Seite etc. */
void WebViewer::setTabCornerButton()
{
  QToolButton* btn = new QToolButton ( this );
  btn->setObjectName ( QLatin1String ( "newpagecorneraction" ) );
  btn->setText ( trUtf8 ( "New Page" ) );
  btn->setIcon ( QIcon::fromTheme ( QLatin1String ( "tab-new" ) ) );
  btn->setToolTip ( trUtf8 ( "Add a new empty Tab" ) );
  connect ( btn, SIGNAL ( clicked() ), this, SLOT ( addViewerTab() ) );
  setCornerWidget ( btn, Qt::TopRightCorner );
}

/** Wird für den Start von WebInspector benötigt */
QWebPage* WebViewer::startPage()
{
  return m_viewer->page();
}

/** Gibt den Aktuellen Tab Index zurück */
int WebViewer::realPageIndex ( int index )
{
  return ( index == -1 ) ? currentIndex() : index;
}

/** Zeiger auf das aktuelle Viewer an Hand des sichtbaren Tabs. */
Viewer* WebViewer::activeView ( int index )
{
  QObject* w = widget ( realPageIndex ( index ) );
  while ( w )
  {
    if ( Viewer* mw = qobject_cast<Viewer*> ( w ) )
      return mw;

    w = w->parent();
  }
  return m_viewer;
}

/**
* Suche Index von Viewer mit der Url
* Wenn nicht gefunden dann gebe -1 zurück.
*/
int WebViewer::getIndexWithUrl ( const QUrl &url )
{
  if ( ! url.isValid() )
    return -1;

  for ( int i = 0; i < count(); i++ )
  {
    Viewer* mw = qobject_cast<Viewer*> ( widget ( i ) );
    if ( mw && mw->url() == url )
    {
      return i;
    }
  }
  return -1;
}

/**
* Wird von Signal @ref WebView::titleChanged aufgerufen.
* Hier den Tab Titel, Tip und WasIstDas setzen.
*/
void WebViewer::updateTab ( const QUrl &url, const QString &title )
{
  if ( title.isEmpty() || ! url.isValid() )
    return;

  int max = 20;
  int index = getIndexWithUrl ( url );
  if ( index == -1 )
    return;

  QString pr ( title );
  if ( title.size() >= max )
  {
    pr.resize ( max );
    pr.append ( "..." );
  }

  setTabText ( index, pr );
  setTabToolTip ( index, title );
  setTabWhatsThis ( index, title );
  setFavicon ( index );
}

/**
* Bei einem Tabwechsel die Url der Seite
* ermitteln und weiter an @ref urlChanged
* und @ref pageChanged geben.
*/
void WebViewer::pretended ( int index )
{
  QUrl url = activeView ( index )->url();
  if ( ! url.isValid() )
    return;

  emit urlChanged ( url );
  pageChanged ( index );
}

/**
* Wenn eine neue Seite aufgemacht wird dann hier
* das Signal @ref pageEntered aufrufen!
* Es wird hiermit die aktuelle WebPage an die
* Plugins und DockWidgets gesendet!
*/
void WebViewer::pageChanged ( int index )
{
  if ( index == -1 )
    return;

  Viewer* m_view = activeView ( index );
  if ( m_view->url().isValid() )
    emit pageEntered ( m_view->page() );
}

/**
* Setze wenn vorhanden einen Icon in das Tab
* an sonsten mit leeren Icon wieder entfernen.
*/
void WebViewer::setFavicon ( int index )
{
  int id = realPageIndex ( index );
  QIcon icon = QWebSettings::iconForUrl ( activeView ( id )->url() );
  if ( icon.isNull() )
  {
    icon = activeView ( id )->icon();
    if ( icon.isNull() )
    {
      setTabIcon ( id, tabIcon );
      return;
    }
  }
  setTabIcon ( id, icon );
}

/** Wenn ein Tab eingefügt oder geändert wird! */
void WebViewer::tabInserted ( int index )
{
  // WebPage mitteilen
  pageChanged ( index );
  // Favicon Setzen
  setFavicon ( index );
}

/**
* Öffne eine Neue Seite mit @param newUrl wenn @param oldUrl
* noch nicht geöffnet ist.
*/
bool WebViewer::setViewerTabByUrl ( const QUrl &oldUrl, const QUrl &newUrl )
{
  qDebug() << Q_FUNC_INFO << oldUrl << newUrl;
  // Suche nach einer Page mit @param oldUrl
  int index = getIndexWithUrl ( oldUrl );
  // Wenn die Aktuelle page diese URL enthält dann setUrl und fertig.
  if ( activeView ( index )->url() == oldUrl )
  {
    setUrl ( newUrl );
    return true;
  }

  return addViewerUrlTab ( newUrl );
}

/**
* Ein neues Tab erstellen und die Signale hierfür neu Initialsieren.
*/
void WebViewer::addViewerTab ( Viewer *view, bool move )
{
  if ( ! view )
    return;

  connect ( view, SIGNAL ( titleChanged ( const QUrl &, const QString & ) ),
            this, SLOT ( updateTab ( const QUrl &, const QString & ) ) );

  connect ( view, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ),
            this, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ) );

  connect ( view, SIGNAL ( urlChanged ( const QUrl & ) ),
            this, SIGNAL ( urlChanged ( const QUrl & ) ) );

  /* in 10. schritten 0% -> 100% */
  connect ( view, SIGNAL ( loadProgress ( int ) ),
            this, SIGNAL ( loadProgress ( int ) ) );

  connect ( view, SIGNAL ( iconChanged() ),
            this, SLOT ( setFavicon() ) );

  connect ( view, SIGNAL ( statusBarMessage ( const QString & ) ),
            this, SIGNAL ( statusBarMessage ( const QString & ) ) );

  connect ( view, SIGNAL ( hitTestResult ( const QWebElement & ) ),
            this, SIGNAL ( hitTestResult ( const QWebElement & ) ) );

  connect ( view, SIGNAL ( totalBytes ( qint64 ) ),
            this, SIGNAL ( bytesLoaded ( qint64 ) ) );

  connect ( view, SIGNAL ( loadStarted () ),
            this, SIGNAL ( loadStarted () ) );

  QUrl uri ( view->url() );
  if ( uri.isValid() && move )
  {
    QString title = uri.host().isEmpty() ? trUtf8 ( "file" ) : uri.host();
    int index = addTab ( view, title );
    setCurrentIndex ( index );
  }
  else
    addTab ( view, tabIcon, trUtf8 ( "Unknown" ) );
}

/**
* Wird für eine Leere Seite erstellen benötigt.
*/
void WebViewer::addViewerTab ()
{
  Viewer* view = new Viewer ( this );
  view->setHtml ( blank() );
  // nach vorne holen
  addViewerTab ( view, true );
}

/**
* Ein neues Tab erstellen und die Url setzen Initialsieren.
*/
bool WebViewer::addViewerUrlTab ( const QUrl &url )
{
  if ( ! url.scheme().contains ( QRegExp ( "(http[s]?|file)" ) ) )
    return false;

  Viewer* view = new Viewer ( this );
  view->openUrl ( url );
  addViewerTab ( view, false );
  return true;
}

void WebViewer::closeViewerTab ( int index )
{
  if ( count() < 2 )
    return;

  removeTab ( index );
}

/**
* Liste auflösen und weiter an @ref WebView::findKeyword senden.
*/
void WebViewer::keywords ( const QStringList &words )
{
  activeView()->findKeyword ( QString::null );
  foreach ( QString w, words )
  {
    activeView()->findKeyword ( w );
  }
}

/**
* Dieser Slot wird nur von AutoRefresh aufgerufen!
* Verwende deshalb QWebPage::ReloadAndBypassCache um den Cache zu unterdrücken!
*/
void WebViewer::refresh ()
{
  activeView()->page()->triggerAction ( QWebPage::ReloadAndBypassCache );
}

void WebViewer::back ()
{
  activeView()->back();
}

void WebViewer::forward ()
{
  activeView()->forward();
}

/**
* Weiter auf @ref WebView::openUrl senden.
*/
void WebViewer::setUrl ( const QUrl &url )
{
  if ( ! url.isValid() || url.isRelative() )
    return;

  // Aktuelles Tab verwenden
  int index = currentIndex ();
  activeView ( index )->openUrl ( url );
}

/**
* Wird Normalerweise nach dem start aufgerufen und
* setzt den Focus auf das aktuelle Browser Fenster.
**/
void WebViewer::setWebFocus()
{
  activeView()->setFocus ( Qt::ActiveWindowFocusReason );
}

/**
* Webseite Zoomen siehe QWebView::zoomFactor
* @li Zoom Faktor In=1
* @li Zoom Faktor Out=2
* @li Zoom Faktor Original=0
*/
void WebViewer::zoomFactor ( int type )
{
  Viewer* view = activeView();
  if ( !view )
    return;

  switch ( type )
  {
    case 1:
      view->setZoomFactor ( view->zoomFactor() + 0.1 );
      break;

    case 2:
      view->setZoomFactor ( view->zoomFactor() - 0.1 );
      break;

    default:
      view->setZoomFactor ( 1.0 );
      break;
  }
}

/**
* Dieser SLOT setzt die Browser Fensterbreite auf ein fixes mass.
* Siehe @ref ResizePortButtons
* @note Im Moment in @ref Window::Window mit MAINTAINER_REPOSITORY deaktiviert!
*/
void WebViewer::setViewerWidth ( int w )
{
  QSize s = size();
  s.setWidth ( w );

  if ( w < 250 ) // Restore
    activeView()->page()->setPreferredContentsSize ( QSize ( 0, 0 ) );
  else
    activeView()->page()->setPreferredContentsSize ( s );

  updateGeometry ();
}

/**
* Aktuelle URL von @ref WebView::url holen.
*/
const QUrl WebViewer::getUrl()
{
  return activeView()->url();
}

/**
* Nehme URL von @ref WebView::url holen.
*/
const QList<QUrl> WebViewer::getPageUrls ()
{
  QList<QUrl> list;
  for ( int i = 0; i < count(); ++i )
  {
    Viewer* v = qobject_cast<Viewer*> ( widget ( i ) );
    if ( v && v->url().isValid() )
      list.append ( v->url() );
  }
  return list;
}

/**
* Wenn kein XHTML Quelltext verfügbar ist wird der
* von QWebKit generierte HTML Quelltext verwendet.
*/
const QString WebViewer::toHtml()
{
  QString source = activeView()->source ();
  if ( source.isEmpty() )
    return activeView()->page()->currentFrame()->toHtml ();
  else
    return source;
}

/**
* Oberster WebElement baum für @class DomTree
*/
const QWebElement WebViewer::toWebElement()
{
  return activeView()->page()->mainFrame()->documentElement ();
}

/**
* Eine neue Leere Seite erstellen
*/
const QString WebViewer::blank()
{
  return m_wcfg->templateSource ( "blank" );
}

/**
* Setze eine About Seite!
*/
void WebViewer::setAboutPage ( const QString &type )
{
  setUrl ( m_wcfg->templateSite ( type ) );
}

WebViewer::~WebViewer()
{}
