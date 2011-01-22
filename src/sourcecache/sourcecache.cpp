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

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

static inline const QString sorry_no_cache_availably()
{
  QDomDocument dom;
  dom.appendChild ( dom.createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
  QDomElement html = dom.createElement ( "html" );
  html.setAttribute ( "xmlns", "http://www.w3.org/1999/xhtml" );
  dom.appendChild ( html );
  QDomElement head = dom.createElement ( "head" );
  html.appendChild ( head );
  QDomElement title = dom.createElement ( "title" );
  title.appendChild ( dom.createTextNode ( "Sorry" ) );
  head.appendChild ( title );
  QDomElement body = dom.createElement ( "body" );
  html.appendChild ( body );
  QDomElement div = dom.createElement ( "div" );
  div.appendChild ( dom.createTextNode ( "\n\tno cache availably\tcurrently no url reloaded\n" ) );
  body.appendChild ( div );
  return dom.toString ( 1 );
}

static inline const QString source_cache_temp_path()
{
  QDir d;
  QString p ( QDir::tempPath () );
  p.append ( d.separator() );
  QString uuid = QUuid::createUuid();
  p.append ( uuid.remove ( QRegExp ( "[\\{\\}]?" ) ) );
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

void SourceCache::cleanUp()
{
  QDir d ( QDir::tempPath () );
  if ( ! d.rmdir ( tmpPath ) )
  {
    d.setCurrent ( tmpPath );
    QFile fp;
    for ( int i = 0; i < cacheFiles.size(); ++i )
    {
      fp.setFileName ( cacheFiles.value ( i ) );
      fp.remove();
    }
    d.setCurrent ( QDir::tempPath () );
    d.rmdir ( tmpPath );
  }
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
  QString buffer ( sorry_no_cache_availably() );
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
  cleanUp();
  cacheFiles.clear();
}
