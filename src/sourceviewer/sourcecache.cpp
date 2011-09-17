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

#include "sourcecache.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

SourceCache::SourceCache ( QObject * parent )
    : QObject ( parent )
    , tmpPath ( Settings::tempDir ( "source" ) )
{
  setObjectName ( "SourceCache" );
  cacheFiles.clear();
}

const QByteArray SourceCache::md5sum ( const QUrl &url ) const
{
  QString str = url.toString ();
  QCryptographicHash cry ( QCryptographicHash::Md5 );
  cry.addData ( str.toUtf8() );
  return cry.result().toHex();
}

void SourceCache::cleanUp()
{
  QDir d ( QDir::tempPath () );
  if ( ! d.rmdir ( tmpPath ) )
  {
    d.setCurrent ( QDir::tempPath () );
    QFile fp;
    QHashIterator<QByteArray,QString> i ( cacheFiles );
    while ( i.hasNext() )
    {
      i.next();
      fp.setFileName ( i.value() );
      fp.remove();
    }
    d.rmdir ( tmpPath );
  }
}

bool SourceCache::setCache ( const QUrl &url, const QString &source )
{
  if ( ! url.isValid() || source.isEmpty() )
    return false;

  QByteArray salt = md5sum ( url );
  QString p ( tmpPath );
  p.append ( QDir::separator() );
  p.append ( "cache_" );
  p.append ( salt );
  p.append ( ".html" );

  QFile fp ( p );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << source;
    fp.close();
    cacheFiles.insert ( salt, p );
    return true;
  }
  return false;
}

const QString SourceCache::getCache ( const QUrl &url )
{
  QString buffer;
  if ( ! url.isValid() )
    return buffer;

  QByteArray salt = md5sum ( url );
  if ( cacheFiles.contains ( salt ) )
  {
    QFile fp ( cacheFiles.value ( salt ) );
    if ( fp.open ( QIODevice::ReadOnly ) )
    {
      QTextStream stream ( &fp );
      buffer = stream.readAll();
      fp.close();
    }
  }
  return buffer;
}

SourceCache::~SourceCache()
{
  cleanUp();
  cacheFiles.clear();
}
