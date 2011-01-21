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

#include "sourcecache.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QUuid>

static inline const QString source_cache_temp_path()
{
  QDir d;
  QString p ( QDir::tempPath () );
  p.append ( d.separator() );
  QString uuid = QUuid::createUuid();
  p.append ( uuid.remove ( QRegExp ( "[\{\}]" ) ) );
  d.mkpath ( p );
  return p;
}

SourceCache::SourceCache ( QObject * parent )
    : QObject ( parent )
    , tmpPath ( source_cache_temp_path() )
{
  setObjectName ( "SourceCache" );
  cacheFiles.clear();
}

void SourceCache::setCache ( int index, const QString &source )
{
  if ( index < 0 && source.isEmpty() )
    return;

  QString p ( tmpPath );
  p.append ( QDir::separator() );
  p.append ( QString::number ( index ) );
  p.append ( ".tmp" );

  QFile fp ( p );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << source;
    fp.close();
    cacheFiles.insert ( index, p );
  }
}

const QString SourceCache::getCache ( int index )
{
  QString buffer;
  if ( cacheFiles.contains ( index ) )
  {
    QFile fp ( cacheFiles.value ( index ) );
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
  cacheFiles.clear();
}
