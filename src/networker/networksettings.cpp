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

#include "networksettings.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopServices>

NetworkSettings::NetworkSettings ( QObject * parent )
    : QSettings ( QSettings::NativeFormat,
                  QSettings::UserScope, "hjcms.de", "xhtmldbg", parent )
    , wcfg ( QWebSettings::globalSettings() )
{
  setObjectName ( QLatin1String ( "networksettings" ) );
}

const QNetworkRequest NetworkSettings::requestOptions ( const QNetworkRequest &req )
{
  QNetworkRequest request = req;
  request.setAttribute ( QNetworkRequest::CacheSaveControlAttribute,
                         value ( "CacheSaveControlAttribute", false ).toBool() );
  request.setAttribute ( QNetworkRequest::SourceIsFromCacheAttribute,
                         value ( "SourceIsFromCacheAttribute", false ).toBool() );
  request.setAttribute ( QNetworkRequest::DoNotBufferUploadDataAttribute,
                         value ( "DoNotBufferUploadDataAttribute", true ).toBool() );
  request.setAttribute ( QNetworkRequest::HttpPipeliningAllowedAttribute,
                         value ( "HttpPipeliningAllowedAttribute", true ).toBool() );
  request.setAttribute ( QNetworkRequest::HttpPipeliningWasUsedAttribute,
                         value ( "HttpPipeliningWasUsedAttribute", true ).toBool() );

  // Headers
  beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
  QStringList keys = allKeys();
  if ( keys.size() > 1 )
  {
    foreach ( QString key, keys )
    {
      QByteArray val = value ( key ).toByteArray();
      request.setRawHeader ( key.toAscii(), val );
    }
  }
  endGroup();

  return request;
}

const QString NetworkSettings::storageDirectory ()
{
  QString dbPath = QDesktopServices::storageLocation ( QDesktopServices::CacheLocation );
  QDir dir ( dbPath );
  dir.mkpath ( QLatin1String ( "storage" ) );
  wcfg->setLocalStoragePath ( dbPath + dir.separator() + QLatin1String ( "storage" ) );
  return wcfg->localStoragePath();
}

const QNetworkProxy NetworkSettings::getProxy()
{
  QNetworkProxy proxy;
  int type = value ( QLatin1String ( "proxyType" ), QNetworkProxy::HttpProxy ).toUInt();
  proxy.setType ( ( QNetworkProxy::ProxyType ) type );
  proxy.setHostName ( value ( QLatin1String ( "proxyHostName" ) ).toString() );
  proxy.setPort ( value ( QLatin1String ( "proxyPort" ), 8080 ).toUInt() );
  proxy.setUser ( value ( QLatin1String ( "proxyUser" ) ).toString() );
  proxy.setPassword ( value ( QLatin1String ( "proxyPassword" ) ).toString() );
  return proxy;
}

NetworkSettings::~NetworkSettings()
{}
