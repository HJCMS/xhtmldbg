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

#include "keywordsdom.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtXml */
#include <QtXml/QDomProcessingInstruction>

KeywordsDom::KeywordsDom () : QDomDocument ()
{
  appendChild ( createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
  QDomElement rootNode = createElement ( "tags" );
  rootNode.setAttribute ( "xmlns", "http://www.w3.org/2001/XMLSchema/" );
  rootNode.appendChild ( createElement ( "default" ) );
  appendChild ( rootNode );
}

KeywordsDom::KeywordsDom ( const QDomDocument &parent )  : QDomDocument ( parent )
{}

KeywordsDom& KeywordsDom::operator= ( const KeywordsDom &x )
{
  return static_cast<KeywordsDom&> ( QDomDocument::operator= ( x ) );
}

const QDomNode KeywordsDom::rootNode() const
{
  return firstChildElement ( "tags" );
}

const QDomNode KeywordsDom::defaultNode() const
{
  return rootNode().firstChildElement ( "default" );
}

const QDomNodeList KeywordsDom::keywordNodes()
{
  if ( rootNode().hasChildNodes() )
    return elementsByTagName ( QLatin1String ( "keywords" ) );
  else
    return QDomNodeList();
}

const QStringList KeywordsDom::fileNamesList()
{
  QStringList list;
  QDomNodeList nodes = keywordNodes();
  for ( int i = 0; i < nodes.size(); ++i )
  {
    QDomElement element = nodes.at ( i ).toElement();
    if ( element.hasAttribute ( QLatin1String ( "file" ) ) )
      list.append ( element.attribute ( QLatin1String ( "file" ), QString::null ) );
  }
  return list;
}

