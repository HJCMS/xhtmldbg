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
#include "application.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "cookieacceptdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QDesktopServices>
#include <QtGui/QColor>
#include <QtGui/QMenu>
#include <QtGui/QPalette>

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
  updateWebSettings();

  NetworkAccessManager* netManager = Application::networkAccessManager();

  m_page = new Page ( this );
  m_page->setNetworkAccessManager ( netManager );
  m_page->setLinkDelegationPolicy ( QWebPage::DelegateAllLinks );
  setPage ( m_page );

  connect ( Application::cookieManager(), SIGNAL ( cookiesRequest ( const QUrl & ) ),
            this, SLOT ( cookiesRequest ( const QUrl & ) ) );

  connect ( m_page, SIGNAL ( getUrl ( const QUrl & ) ),
            this, SLOT ( openUrl ( const QUrl & ) ) );

  connect ( this, SIGNAL ( linkClicked ( const QUrl & ) ),
            netManager, SLOT ( setUrl ( const QUrl & ) ) );

  connect ( this, SIGNAL ( loadStarted () ),
            this, SLOT ( cursorwait () ) );

  connect ( this, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( cursorFinished ( bool ) ) );
}

void Viewer::updateWebSettings()
{
  // Settings
  QSettings* cfg = new QSettings ( QSettings::NativeFormat,
                                   QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  QWebSettings* wcfg = settings ();

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

void Viewer::cursorwait ()
{
  setCursor ( Qt::WaitCursor );
}

void Viewer::cursorFinished ( bool )
{
  setCursor ( Qt::ArrowCursor );
}

void Viewer::bookmark()
{
  emit addBookmark ( url(), title() );
}

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

void Viewer::unsupportedContent ( QNetworkReply *reply )
{
  qDebug() << "Unsupported Content:" << reply->url();
  if ( reply->error() == QNetworkReply::NoError )
    return;
}

void Viewer::cookiesRequest ( const QUrl &u )
{
  QString pageHost ( url().host() );
  QString cookieHost ( u.host() );
  if ( pageHost.contains ( cookieHost ) && ! cookieAlreadyAdd )
  {
    CookieAcceptDialog diag ( u, this );
    if ( diag.exec() )
    {
      cookieAlreadyAdd = true;
      Application::cookieManager()->reload();
    }
  }
}

void Viewer::findKeyword ( const QString &word )
{
  findText ( word, QWebPage::HighlightAllOccurrences );
}

void Viewer::openUrl ( const QUrl &url )
{
  setUrl ( url );
  emit linkClicked ( url ); // @ref NetworkAccessManager
}

const QString Viewer::source()
{
  return m_page->xhtmlSource();
}

Viewer::~Viewer()
{
}
