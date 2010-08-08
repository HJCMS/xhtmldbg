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

#include "webinspectorclient.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebSettings>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkAccessManager>

WebInspectorClient::WebInspectorClient ( QObject * parent )
    : QWebPage ( parent )
{
  setObjectName ( QLatin1String ( "webinspectorpage" ) );
  setForwardUnsupportedContent ( false );
  setLinkDelegationPolicy ( QWebPage::DontDelegateLinks );

  // Plattenspeicher für die Performanse steigerung
  QDir dir ( QDesktopServices::storageLocation ( QDesktopServices::CacheLocation ) );
  dir.mkpath ( QLatin1String ( "inspector" ) );
  storagePath = dir.path() + dir.separator() + QLatin1String ( "inspector" );

  QNetworkDiskCache* diskCache = new QNetworkDiskCache ( this );
  diskCache->setCacheDirectory ( storagePath );

  networkAccessManager()->setCache ( diskCache );
  // Einstellungen ändern
  updateWebSettings();
}

/**
* Das lesen aller Web Einstellungen muss
* vor dem ersten erstellen ein tabs erfolgen.
*/
void WebInspectorClient::updateWebSettings()
{
  QWebSettings* wcfg = settings();
  wcfg->setIconDatabasePath ( storagePath );
  wcfg->setLocalStoragePath ( storagePath );
  wcfg->setDefaultTextEncoding ( QLatin1String ( "utf-8" ) );
  wcfg->setAttribute ( QWebSettings::DeveloperExtrasEnabled, true );
  wcfg->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, false );
  wcfg->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, false );
  wcfg->setAttribute ( QWebSettings::AutoLoadImages, true );
  wcfg->setAttribute ( QWebSettings::JavascriptEnabled, true );
  wcfg->setAttribute ( QWebSettings::PluginsEnabled, false );
  wcfg->setAttribute ( QWebSettings::JavaEnabled, false );
  wcfg->setAttribute ( QWebSettings::PrivateBrowsingEnabled, false );
  wcfg->setAttribute ( QWebSettings::DnsPrefetchEnabled, true );
  wcfg->setAttribute ( QWebSettings::JavascriptCanOpenWindows, false );
  wcfg->setAttribute ( QWebSettings::JavascriptCanAccessClipboard, false );
}

void WebInspectorClient::load ( const QUrl &url )
{
  if ( url.isValid() && url.scheme().contains ( "http" ) )
    mainFrame()->load ( url );
}

WebInspectorClient::~WebInspectorClient ()
{
  settings()->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
  networkAccessManager()->cache()->clear();
}
