/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "networkcache.h"

/* QtCore */
#include <QtCore/QChar>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>

/* QtNetwork */
#include <QtNetwork/QAbstractNetworkCache>
#include <QtNetwork/QNetworkCacheMetaData>

NetworkCache::NetworkCache ( const QString &path, QObject * parent )
    : QNetworkDiskCache ( parent )
    , cacheDir ( path )
{
  setObjectName ( QLatin1String ( "NetworkCache" ) );
  setCacheDirectory ( cacheDir );
}

/**
* Suffix und Prefix sind im Quelltext von qt unter
* sources/network/access/qnetworkdiskcache.cpp als Macros definiert.
*/
const QString NetworkCache::cacheFilePath ( const QUrl &url ) const
{
  QByteArray file ( cacheDir.toUtf8() );
  file.append ( QDir::separator().toLatin1() );
  file.append ( url.scheme().toUtf8() );
  file.append ( QDir::separator().toLatin1() );
  file.append ( "cache_" );

  QUrl cleanUrl = url;
  cleanUrl.setPassword ( QString() );
  cleanUrl.setFragment ( QString() );

  QCryptographicHash hash ( QCryptographicHash::Sha1 );
  hash.addData ( cleanUrl.toEncoded() );
  file.append ( hash.result().toHex() );
  file.append ( ".cache" );
  return QString ( file );
}

const QString NetworkCache::sourceCache ( const QString &filePath )
{
  QString buffer;
  QFile fp ( filePath );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    
    fp.close();
  }
  return buffer;
}

bool NetworkCache::initCache ( const QUrl &url )
{
  if ( ! url.isValid() )
    return false;

  QFileInfo info ( cacheFilePath ( url ) );
  if ( ! info.exists () )
    return false;

  QUrl sendUrl;
  sendUrl.setScheme ( "file" );
  sendUrl.setPath ( info.absoluteFilePath() );
  emit readyRead ( url );

  return true;
}

NetworkCache::~NetworkCache()
{}
