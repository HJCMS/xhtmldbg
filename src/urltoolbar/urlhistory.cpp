/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "urlhistory.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDesktopServices>

UrlHistory::UrlHistory() : QDomDocument ()
{
  xmlPath = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  xmlPath.append ( QDir::separator() );
  xmlPath.append ( "RecentUrl.xml" );
  QFile fp ( xmlPath );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    setContent ( &fp );
    fp.close();
  }
  else
  {
    appendChild ( createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
    rootNode = createElement ( "RecentFiles" );
    rootNode.setAttribute ( "version", "0.2" );
    rootNode.setAttribute ( "xmlns", "http://www.w3.org/2001/XMLSchema" );
    rootNode.setAttribute ( "xmlns:file", "http://standards.freedesktop.org/recent-file" );
    appendChild ( rootNode );
  }
}

const QDomElement UrlHistory::createEntry ( const QString &url )
{
  QDomElement node = createElement ( "RecentItem" );
  QDomElement textNode = createElement ( "URI" );
  textNode.appendChild ( createCDATASection ( url ) );
  node.appendChild ( textNode );

  QDomElement mimeNode = createElement ( "Mime-Type" );
  mimeNode.appendChild ( createTextNode ( "text/html" ) );
  node.appendChild ( mimeNode );

  QDomElement timeNode = createElement ( "Timestamp" );
  timeNode.appendChild ( createTextNode ( "0" ) );
  node.appendChild ( timeNode );

  node.appendChild ( createElement ( "Private" ) );
  node.appendChild ( createElement ( "Groups" ) );
  return node;
}

const QStringList UrlHistory::load()
{
  QStringList items;
  QDomNodeList nodes = elementsByTagName ( "URI" );
  if ( nodes.size() > 0 )
  {
    for ( int i = 0; i < nodes.size(); ++i )
    {
      QString url ( nodes.at ( i ).firstChild().nodeValue() );
      if ( ! url.isEmpty() )
        items.append ( url );
    }
  }
  else
  {
    items << "http://localhost" << "http://127.0.0.1" << "http://[::1]";
  }
  return items;
}

void UrlHistory::save ( const QStringList &items )
{
  QDomDocument dom;
  dom.appendChild ( dom.createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
  QDomElement node = dom.createElement ( "RecentFiles" );
  node.setAttribute ( "version", "0.2" );
  node.setAttribute ( "xmlns", "http://www.w3.org/2001/XMLSchema" );
  node.setAttribute ( "xmlns:file", "http://standards.freedesktop.org/recent-file" );
  dom.appendChild ( node );

  int max = 25; // Maximale Einträge in XML Datei
  foreach ( QString entry, items )
  {
    if ( max < 0 )
      break;

    if ( ! entry.isEmpty() )
    {
      node.appendChild ( createEntry ( entry ) );
      max--;
    }
  }

  QFile fp ( xmlPath );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << dom.toByteArray();
    fp.close();
  }
}

UrlHistory::~UrlHistory()
{}
