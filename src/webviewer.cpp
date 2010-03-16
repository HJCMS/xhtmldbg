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

#include "webviewer.h"
#include "viewer.h"
#include "historymanager.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QDesktopServices>
#include <QtGui/QTabBar>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebSettings>

WebViewer::WebViewer ( QWidget * parent )
    : QTabWidget ( parent )
    , url ( QUrl ( QLatin1String ( "http://www.hjcms.de" ) ) )
{
  if ( objectName().isEmpty() )
    setObjectName ( "webviewer" );

  setContentsMargins ( 0, 0, 0, 0 );
  setTabsClosable ( true );

  m_viewer = new Viewer ( this );
  updateWebSettings();
  addViewerTab ( m_viewer );

  connect ( this, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( pretended ( int ) ) );

  connect ( this, SIGNAL ( tabCloseRequested ( int ) ),
            this, SLOT ( closeViewerTab ( int ) ) );

}

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
  wcfg->setAttribute ( QWebSettings::DeveloperExtrasEnabled,
                       cfg->value ( QLatin1String ( "DeveloperExtrasEnabled" ), false ).toBool() );

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

void WebViewer::pretended ( int index )
{
  QUrl url = activeView()->url();
  if ( url.isValid() )
    emit urlChanged ( url );

  QIcon icon = QWebSettings::iconForUrl ( url );
  if ( ! icon.isNull() )
    setTabIcon ( index, icon );
}

void WebViewer::setFavicon()
{
  QIcon icon = activeView()->icon();
  if ( ! icon.isNull() )
    setTabIcon ( currentIndex(), icon );
}

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

  QUrl uri ( view->url() );
  QString title = uri.host().isEmpty() ? trUtf8 ( "Startpage" ) : uri.host();
  int index = addTab ( view, title );
  setCurrentIndex ( index );
}

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

void WebViewer::keywords ( const QStringList &words )
{
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

void WebViewer::setUrl ( const QUrl &u )
{
  activeView()->openUrl ( u );
  url = u;
}

const QUrl WebViewer::getUrl()
{
  return activeView()->url();
}

const QString WebViewer::toHtml()
{
  QString source = activeView()->source ();
  if ( source.isEmpty() )
    return activeView()->page()->currentFrame()->toHtml ();
  else
    return source;
}

const QWebElement WebViewer::toWebElement()
{
  return activeView()->page()->currentFrame()->documentElement ();
}

const QString WebViewer::blank()
{
  return QString::fromUtf8 ( "<html>\n<head>\n<title>QTidy Startpage</title>\n</head>\n<body>\n<div>New Empty Page</div>\n</body>\n</html>" );
}

WebViewer::~WebViewer()
{
}
