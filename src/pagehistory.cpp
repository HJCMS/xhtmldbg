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

#include "pagehistory.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QTime>
#include <QtCore/QString>

/* QtXml */
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

static inline const QDomDocument create_document_construct ()
{
  QDomDocument dom;
  dom.appendChild ( dom.createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
  QDomElement node = dom.createElement ( "RecentFiles" );
  node.setAttribute ( "version", "0.2" );
  node.setAttribute ( "xmlns", "http://www.w3.org/2001/XMLSchema" );
  node.setAttribute ( "xmlns:file", "http://standards.freedesktop.org/recent-file" );
  dom.appendChild ( node );
  return dom;
}

PageHistory::PageHistory()
    : QDomDocument ( create_document_construct () )
{}

const QList<QUrl> PageHistory::history ( const QUrl &filePath )
{
  QList<QUrl> list;
  QFile fp ( filePath.path() );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QDomDocument dom;
    if ( dom.setContent ( &fp ) )
    {
      QDomNodeList nodes = dom.elementsByTagName ( "URI" );
      if ( nodes.size() > 0 )
      {
        for ( int i = 0; i < nodes.size(); ++i )
        {
          QUrl url ( nodes.at ( i ).firstChild().nodeValue() );
          if ( url.isValid() )
            list.append ( url );
        }
      }
      dom.clear();
    }
    fp.close();
  }
  return list;
}

inline const QDomElement PageHistory::mimeType()
{
  QDomElement node = createElement ( "Mime-Type" );
  node.appendChild ( createTextNode ( "text/html" ) );
  documentElement().appendChild ( node );
  return node;
}

const QDomElement PageHistory::timeStamp()
{
  QDomElement node = createElement ( "Timestamp" );
  node.appendChild ( createTextNode ( "0" ) );
  documentElement().appendChild ( node );
  return node;
}

const QDomElement PageHistory::createEntry ( const QUrl &url )
{
  QDomElement recentItem = createElement ( "RecentItem" );
  QDomElement urlItem = createElement ( "URI" );
  urlItem.appendChild ( createCDATASection ( url.toString() ) );
  recentItem.appendChild ( urlItem );
  recentItem.appendChild ( mimeType() );
  recentItem.appendChild ( timeStamp() );
  recentItem.appendChild ( createElement ( "Private" ) );
  recentItem.appendChild ( createElement ( "Groups" ) );
  return recentItem;
}

void PageHistory::addEntries ( const QList<QUrl> &list )
{
  foreach ( QUrl url, list )
  {
    documentElement().appendChild ( createEntry ( url ) );
  }
}

bool PageHistory::saveHistory ( const QUrl &filePath )
{
  QFile fp ( filePath.path() );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << toString ( 1 );
    fp.close();
    return true;
  }
  return false;
}

PageHistory::~PageHistory()
{
  clear();
}
