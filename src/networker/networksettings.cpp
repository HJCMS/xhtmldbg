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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "networksettings.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextIStream>
#include <QtCore/QVariant>

/* QtNetwork */
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

NetworkSettings::NetworkSettings ( QObject * parent )
    : Settings ( parent )
    , wcfg ( QWebSettings::globalSettings() )
{
  setObjectName ( QLatin1String ( "networksettings" ) );
  wcfg->setIconDatabasePath ( webIconDatabasePath() );
  wcfg->setLocalStoragePath ( webLocalStoragePath() );
  wcfg->setAttribute ( QWebSettings::PrivateBrowsingEnabled, false );
  wcfg->setAttribute ( QWebSettings::LocalStorageEnabled, false );
  wcfg->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, false );
  wcfg->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, false );
}

/**
* Suche in der Konfiguration nach dem aktuellen UserAgentString
* Wenn keiner vorhanden setze den XHTMLDBG User-Agent
*/
const QByteArray NetworkSettings::userAgentString()
{
  QString defaultAgent = value ( "UserAgentString" ).toString();
  if ( defaultAgent.isEmpty() )
  {
    QLocale locale = QLocale::system();
    QString iso639_1 = locale.name().remove ( QRegExp ( "([_\\-].+)$" ) );
    QString iso3166 = locale.name();
    defaultAgent.append ( "Mozilla/5.0 (compatible; XHTMLDBG/" );
    defaultAgent.append ( XHTMLDBG_VERSION_STRING );
    defaultAgent.append ( " " + iso639_1 + ", " + iso3166 );
    defaultAgent.append ( ") AppleWebKit (KHTML, like Gecko)" );
  }
  return defaultAgent.toAscii();
}

/**
* ProxyType aus der Konfiguration lesen
*/
QNetworkProxy::ProxyType NetworkSettings::proxyType()
{
  int type = value ( QLatin1String ( "proxyType" ), QNetworkProxy::HttpProxy ).toUInt();
  return static_cast<QNetworkProxy::ProxyType> ( type );
}

/**
* Diese Methode wird vor jeder Netzwerkanfrage gesetzt!
*/
const QNetworkRequest NetworkSettings::requestOptions ( const QNetworkRequest &req )
{
  QNetworkRequest request = req;
  request.setAttribute ( QNetworkRequest::CacheSaveControlAttribute, false );
  request.setAttribute ( QNetworkRequest::SourceIsFromCacheAttribute, false );
  request.setAttribute ( QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork );
#if QT_VERSION >= 0x040700
  request.setAttribute ( QNetworkRequest::CookieLoadControlAttribute, QNetworkRequest::Automatic );
#endif

  request.setAttribute ( QNetworkRequest::DoNotBufferUploadDataAttribute,
                         boolValue ( "DoNotBufferUploadDataAttribute", true ) );
  request.setAttribute ( QNetworkRequest::HttpPipeliningAllowedAttribute,
                         boolValue ( "HttpPipeliningAllowedAttribute", true ) );
  request.setAttribute ( QNetworkRequest::HttpPipeliningWasUsedAttribute,
                         boolValue ( "HttpPipeliningWasUsedAttribute", true ) );


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

  // User-Agent
  request.setRawHeader ( QByteArray ( "User-Agent" ), userAgentString() );

  return request;
}

const QString NetworkSettings::storageDirectory ()
{
  return wcfg->localStoragePath();
}

const QNetworkProxy NetworkSettings::getProxy()
{
  QNetworkProxy proxy;
  proxy.setType ( proxyType() );
  proxy.setHostName ( value ( QLatin1String ( "proxyHostName" ) ).toString() );
  proxy.setPort ( value ( QLatin1String ( "proxyPort" ), 3128 ).toUInt() );
  proxy.setUser ( value ( QLatin1String ( "proxyUser" ) ).toString() );
  QString pass ( QByteArray::fromBase64 ( value ( QLatin1String ( "proxyPassword" ) ).toByteArray() ) );
  proxy.setPassword ( pass );
  return proxy;
}

const QSslConfiguration NetworkSettings::sslConfiguration()
{
  QSslConfiguration ssl = QSslConfiguration::defaultConfiguration ();

  QString dbpath = value ( QLatin1String ( "sslCaCertsPath" ) ).toString();
  QFileInfo db ( dbpath );
  if ( db.exists() )
  {
    QList<QSslCertificate> caCerts = ssl.caCertificates();
    caCerts << QSslCertificate::fromPath ( db.absoluteFilePath(), QSsl::Pem, QRegExp::FixedString );
    ssl.setCaCertificates ( caCerts );
  }
  QSslConfiguration::setDefaultConfiguration ( ssl );

// TODO !!! Qt4 macht beim setzen eigener Zertifikate noch Probleme!!!
//   QFile fp0 ( value ( QLatin1String ( "sslPublicKey" ) ).toString() );
//   if ( fp0.open ( QIODevice::ReadOnly ) )
//   {
//     QSslCertificate cert ( &fp0, QSsl::Pem );
//     if ( cert.isValid() )
//       ssl.setLocalCertificate ( cert );
//     else
//       qWarning() << "can not load public certificate";
//
//     fp0.close();
//   }
//
//   QByteArray pass ( QByteArray::fromBase64 ( value ( QLatin1String ( "sslPassPhrase" ) ).toByteArray() ) );
//   QFile fp1 ( value ( QLatin1String ( "sslPrivateKey" ) ).toString() );
//   if ( ! pass.isEmpty() && fp1.open ( QIODevice::ReadOnly ) )
//   {
//     QSslKey sslKey ( &fp1, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, pass );
//     ssl.setPrivateKey ( sslKey );
//     fp1.close();
//   }
  return ssl;
}

const QList<QString> NetworkSettings::trustedCertsList()
{
  QList<QString> list;
  int size = beginReadArray ( QLatin1String ( "TrustedCertsHosts" ) );
  for ( int i = 0; i < size; ++i )
  {
    setArrayIndex ( i );
    list.append ( value ( "host" ).toString() );
  }
  endArray();
  return list;
}

NetworkSettings::~NetworkSettings()
{
}
