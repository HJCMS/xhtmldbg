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

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QTabBar>
#include <QtGui/QToolButton>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebSettings>

WebViewer::WebViewer ( QWidget * parent )
    : QTabWidget ( parent )
    , url ( QUrl ( QLatin1String ( "http://www.selfhtml.org" ) ) )
{
  setObjectName ( "webviewer" );
  setContentsMargins ( 0, 0, 0, 0 );
  setFocusPolicy ( Qt::StrongFocus );
  setTabsClosable ( true );
  setTabCornerButton();

  m_viewer = new Viewer ( this );
  m_viewer->setObjectName ( QLatin1String ( "startpage" ) );
  updateWebSettings();
  addViewerTab ( m_viewer );

  connect ( this, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( pretended ( int ) ) );

  connect ( this, SIGNAL ( tabCloseRequested ( int ) ),
            this, SLOT ( closeViewerTab ( int ) ) );
}

/**
* Eckenknöpfe für neue Seite etc.
*/
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

/**
* Das lesen aller Web Einstellungen muss
* vor dem ersten erstellen ein tabs erfolgen.
*/
void WebViewer::updateWebSettings()
{
  // Settings
  QSettings* cfg = new QSettings ( QSettings::NativeFormat,
                                   QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  QWebSettings* wcfg = QWebSettings::globalSettings();

  QString dbPath = QDesktopServices::storageLocation ( QDesktopServices::CacheLocation );
  QDir dir ( dbPath );
  dir.mkpath ( QLatin1String ( "icons" ) );
  dir.mkpath ( QLatin1String ( "storage" ) );
  wcfg->setIconDatabasePath ( dbPath + dir.separator() + QLatin1String ( "icons" ) );
  wcfg->setLocalStoragePath ( dbPath + dir.separator() + QLatin1String ( "storage" ) );

  wcfg->setDefaultTextEncoding ( QLatin1String ( "utf-8" ) );

  wcfg->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled,
                       cfg->value ( QLatin1String ( "OfflineStorageDatabaseEnabled" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled,
                       cfg->value ( QLatin1String ( "OfflineWebApplicationCacheEnabled" ), false ).toBool() );

  // Until QtWebkit defaults to 16
  wcfg->setFontSize ( QWebSettings::DefaultFontSize,
                      cfg->value ( QLatin1String ( "DefaultFontSize" ), 16 ).toUInt() );

  wcfg->setFontSize ( QWebSettings::DefaultFixedFontSize,
                      cfg->value ( QLatin1String ( "DefaultFixedFontSize" ), 16 ).toUInt() );

  // Page Settings
  wcfg->setAttribute ( QWebSettings::ZoomTextOnly,
                       cfg->value ( QLatin1String ( "ZoomTextOnly" ), true ).toBool() );

  // Jetzt immer deaktiviert. Siehe Webinspector plugin!
  wcfg->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );

  wcfg->setAttribute ( QWebSettings::AutoLoadImages,
                       cfg->value ( QLatin1String ( "AutoLoadImages" ), true ).toBool() );

  wcfg->setAttribute ( QWebSettings::JavascriptEnabled,
                       cfg->value ( QLatin1String ( "JavascriptEnabled" ), true ).toBool() );

  wcfg->setAttribute ( QWebSettings::PluginsEnabled,
                       cfg->value ( QLatin1String ( "PluginsEnabled" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::JavaEnabled,
                       cfg->value ( QLatin1String ( "JavaEnabled" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::PrivateBrowsingEnabled,
                       cfg->value ( QLatin1String ( "PrivateBrowsingEnabled" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::DnsPrefetchEnabled,
                       cfg->value ( QLatin1String ( "DnsPrefetchEnabled" ), true ).toBool() );

  wcfg->setAttribute ( QWebSettings::JavascriptCanOpenWindows,
                       cfg->value ( QLatin1String ( "JavascriptCanOpenWindows" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::JavascriptCanAccessClipboard,
                       cfg->value ( QLatin1String ( "JavascriptCanAccessClipboard" ), false ).toBool() );

  wcfg->setAttribute ( QWebSettings::PrintElementBackgrounds,
                       cfg->value ( QLatin1String ( "PrintElementBackgrounds" ), false ).toBool() );
}

Viewer* WebViewer::activeView()
{
  QObject* w = currentWidget();
  while ( w )
  {
    if ( Viewer* mw = qobject_cast<Viewer*> ( w ) )
      return mw;

    w = w->parent();
  }
  return m_viewer;
}

/**
* Wird von Signal @ref WebView::titleChanged aufgerufen.
* Hier den Tab Titel, Tip und WasIstDas setzen.
*/
void WebViewer::updateTabTitle ( const QString &title )
{
  int max = 20;
  int index = currentIndex();
  QString pr ( title );
  if ( pr.isEmpty() )
  {
    pr.append ( activeView()->url().host() );
  }
  else if ( title.size() >= max )
  {
    pr.resize ( max );
    pr.append ( "..." );
  }
  setTabText ( index, pr );
  setTabToolTip ( index, title );
  setTabWhatsThis ( index, title );
}

/**
* Bei einem Tabwechsel die Url der Seite
* ermitteln und weiter an @ref urlChanged geben.
*/
void WebViewer::pretended ( int index )
{
  QUrl url = activeView()->url();
  if ( url.isValid() )
    emit urlChanged ( url );

  QIcon icon = QWebSettings::iconForUrl ( url );
  setTabIcon ( index, icon );
}

/**
* Setze wenn vorhanden einen Icon in das Tab
* an sonsten mit leeren Icon wieder entfernen.
*/
void WebViewer::setFavicon()
{
  QIcon icon = activeView()->icon();
  setTabIcon ( currentIndex(), icon );
}

/**
* Öffne eine Neue Seite mit @param newUrl wenn @param oldUrl
* noch nicht geöffnet ist.
*/
bool WebViewer::setViewerTabByUrl ( const QUrl &oldUrl, const QUrl &newUrl )
{
  // Wenn die Aktuelle page diese URL enthält dann setUrl und fertig.
  if ( activeView()->url() == oldUrl )
  {
    setUrl ( newUrl );
    return true;
  }
  // Suche nach einer Page mit @param oldUrl und setze die URL
  for ( int i = 0; i < count(); i++ )
  {
    Viewer* mw = qobject_cast<Viewer*> ( widget ( i ) );
    if ( mw && mw->url() == oldUrl )
    {
      mw->setUrl ( newUrl );
      setCurrentIndex ( i );
      return true;
    }
  }
  // Wenn keine Page mit oldURL vorhanden dann abbrechen.
  // Siehe Window::setPageUrl
  return false;
}

/**
* Ein neues Tab erstellen und die Signale hierfür neu Initialsieren.
*/
void WebViewer::addViewerTab ( Viewer *view )
{
  if ( ! view )
    return;

  connect ( view, SIGNAL ( titleChanged ( const QString & ) ),
            this, SLOT ( updateTabTitle ( const QString & ) ) );

  connect ( view, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ),
            this, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ) );

  connect ( view, SIGNAL ( urlChanged ( const QUrl & ) ),
            this, SIGNAL ( urlChanged ( const QUrl & ) ) );

  connect ( view, SIGNAL ( loadFinished ( bool ) ),
            this, SIGNAL ( loadFinished ( bool ) ) );

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
  QString title = uri.host().isEmpty() ? trUtf8 ( "Startpage" ) : uri.host();
  int index = addTab ( view, title );
  setCurrentIndex ( index );
  if ( uri.toString().contains ( "about:" ) )
    setAboutPage ( uri.toString().split ( ":" ).last() );
}

/**
* Wird für eine Leere Seite erstellen benötigt.
*/
void WebViewer::addViewerTab ()
{
  Viewer* view = new Viewer ( this );
  view->setHtml ( blank() );
  addViewerTab ( view );
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

void WebViewer::refresh ()
{
  activeView()->reload();
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
void WebViewer::setUrl ( const QUrl &u )
{
  // qDebug() << Q_FUNC_INFO << u;
  activeView()->openUrl ( u );
  url = u;
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
* Aktuelle URL von @ref WebView::url holen.
*/
const QUrl WebViewer::getUrl()
{
  return activeView()->url();
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
  return activeView()->page()->currentFrame()->documentElement ();
}

/**
* Eine neue Leere Seite erstellen
*/
const QString WebViewer::blank()
{
  QFile fp ( QString::fromUtf8 ( ":/about/html/blank.html" ) );
  if ( fp.open ( QFile::ReadOnly ) )
  {
    QTextStream rc ( &fp );
    QByteArray html = rc.device()->readAll();
    fp.close();
    return QString ( html );
  }
  return QString::fromUtf8 ( "<html>\n<head>\n<title>QTidy Startpage</title>\n</head>\n<body>\n<div>New Empty Page</div>\n</body>\n</html>" );
}

/**
* Setze eine About Seite!
*/
void WebViewer::setAboutPage ( const QString &type )
{
  QString about = QString ( ":/about/html/%1.html" ).arg ( type );
  QFile fp ( about );
  if ( fp.open ( QFile::ReadOnly ) )
  {
    QTextStream rc ( &fp );
    QByteArray html = rc.device()->readAll();
    fp.close();
    if ( ! html.isEmpty() )
      activeView()->setContent ( html, "text/html", QString ( "about:%1" ).arg ( type ) );
  }
}

WebViewer::~WebViewer()
{}
