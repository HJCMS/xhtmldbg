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
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkRequest>

WebInspectorClient::WebInspectorClient ( QObject * parent )
    : QWebPage ( parent )
    , loading ( false )
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

  // Web Einstellungen ändern
  updateWebSettings();

  // Jetzt den Plattenspeicher setzen
  networkAccessManager()->setCache ( diskCache );

  // Signale
  connect ( this, SIGNAL ( loadStarted() ),
            this, SLOT ( setLoadStarted() ) );
  connect ( this, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( setLoadFinished ( bool ) ) );
}

/**
* Das lesen aller Web Einstellungen \b muss vor dem Erstellen erfolgen.
*/
void WebInspectorClient::updateWebSettings()
{
  QWebSettings* wcfg = settings();
  wcfg->setDefaultTextEncoding ( QLatin1String ( "utf-8" ) );
  wcfg->setAttribute ( QWebSettings::DeveloperExtrasEnabled, true );

  /* Speicher Verwaltung & Verhalten festlegen */
  wcfg->setIconDatabasePath ( storagePath );
  wcfg->setLocalStoragePath ( storagePath );
  wcfg->setAttribute ( QWebSettings::PrivateBrowsingEnabled, false );
  wcfg->setAttribute ( QWebSettings::LocalStorageEnabled, true );
  wcfg->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, false );
  wcfg->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, false );
  // Browser Verhalten
  wcfg->setAttribute ( QWebSettings::AutoLoadImages, true );
  wcfg->setAttribute ( QWebSettings::JavascriptEnabled, true );
  wcfg->setAttribute ( QWebSettings::PluginsEnabled, false );
  wcfg->setAttribute ( QWebSettings::JavaEnabled, false );
  wcfg->setAttribute ( QWebSettings::DnsPrefetchEnabled, true );
  wcfg->setAttribute ( QWebSettings::JavascriptCanOpenWindows, false );
  wcfg->setAttribute ( QWebSettings::JavascriptCanAccessClipboard, false );
#if QT_VERSION >= 0x040700
  wcfg->setAttribute ( QWebSettings::LocalContentCanAccessFileUrls, false );
  wcfg->setAttribute ( QWebSettings::XSSAuditingEnabled, true );
  wcfg->setAttribute ( QWebSettings::AcceleratedCompositingEnabled, false );
  wcfg->setAttribute ( QWebSettings::TiledBackingStoreEnabled, false );
#endif
}

/**
* Manipuliere die Netzwrk Einstellungen für die URL anfrage!
* Wir benötigen für das Auditing einen Platten Speicher.
* Weil aber XHTMLDBG selbst keinen Plattenspeicher verwendet.
* Setzen wir hier eine \b neue Url anfrage damit der Netzwerkmanager
* von @class WebInspectorClient nicht die Einstelleungen von XHTMLDBG verwendet!
*/
const QUrl WebInspectorClient::modifyRequest ( const QUrl &url )
{
  QNetworkRequest req ( url );
  req.setAttribute ( QNetworkRequest::CacheSaveControlAttribute, true );
  req.setAttribute ( QNetworkRequest::SourceIsFromCacheAttribute, true );
  req.setAttribute ( QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache );
  req.setAttribute ( QNetworkRequest::HttpPipeliningAllowedAttribute, false );
  req.setAttribute ( QNetworkRequest::HttpPipeliningWasUsedAttribute, false );
  return req.url();
}

/**
* Sperre weitere Anfragen wenn ein Ladevorgang gestartet wurde.
* Weil der Inspector @ref setHtml und @ref load gleichzeitig aufrufen
* kann, an dieser stelle verhindern das es zu Überläufen kommt.
*/
void WebInspectorClient::setLoadStarted ()
{
  loading = true;
}

/**
* Öffne die Sperre für weitere Anfragen
*/
void WebInspectorClient::setLoadFinished ( bool b )
{
  loading = ( b ) ? false : true;
}

/**
* Seiten Url laden!
*/
void WebInspectorClient::load ( const QUrl &url )
{
  if ( loading )
    return;

  if ( url.isValid() && url.scheme().contains ( "http" ) )
    mainFrame()->load ( modifyRequest ( url ) );
}

/**
* Seiten Inhalt laden und Url anhängen!
*/
void WebInspectorClient::setHtml ( const QString &html, const QUrl &url )
{
  if ( loading )
    return;

  if ( ! html.isEmpty() && url.isValid() )
    mainFrame()->setHtml ( html, modifyRequest ( url ) );
}

/**
* Beim entladen den Debugger wieder abshalten und den Plattenspeicher leeren!
*/
WebInspectorClient::~WebInspectorClient ()
{
  settings()->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
  networkAccessManager()->cache()->clear();
}
