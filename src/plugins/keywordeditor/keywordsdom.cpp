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
#include "keywordstableitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QTextStream>

/* QtXml */
#include <QtXml/QDomProcessingInstruction>

KeywordsDom::KeywordsDom ( const QString &siblings ) : QDomDocument (), keysName ( siblings )
{
  appendChild ( createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );
  QDomElement rootNode = createElement ( "tags" );
  rootNode.appendChild ( createElement ( "default" ) );
  appendChild ( rootNode );
}

QDomNode KeywordsDom::rootNode()
{
  return firstChildElement ( "tags" );
}

const QDomNodeList KeywordsDom::keywordNodes() const
{
  if ( documentElement().hasChildNodes() )
    return elementsByTagName ( keysName );
  else
    return QDomNodeList();
}

const QString KeywordsDom::defaultKeywords() const
{
  return documentElement().firstChildElement ( "default" ).toElement().text();
}

void KeywordsDom::setDefaults ( const QStringList &keys )
{
  if ( keys.size() < 1 )
    return;

  QStringList buffer;
  foreach ( QString k, keys )
  {
    QString key = k.trimmed();
    if ( !key.isEmpty() )
      buffer.append ( key );
  }
  buffer.sort();
  rootNode().firstChildElement ( "default" ).appendChild ( createCDATASection ( buffer.join ( "," ) ) );
}

const QList<KeywordsTableItem*> KeywordsDom::keywordsItemList() const
{
  QList<KeywordsTableItem*> list;
  QDomNodeList nodes = keywordNodes();
  int size = nodes.size();
  for ( int i = 0; i < size; ++i )
  {
    QDomElement element = nodes.at ( i ).toElement();
    list.append ( new KeywordsTableItem ( element.attribute ( "id",QString::number ( i ) ),
                                          element.attribute ( "file","index" ),
                                          element.text().split ( "," ) ) );
  }
  return list;
}

void KeywordsDom::setKeywords ( const QList<KeywordsTableItem*> &list )
{
  if ( list.size() < 1 )
    return;

  foreach ( KeywordsTableItem* item, list )
  {
    QDomElement node = createElement ( keysName );
    node.setAttribute ( QLatin1String ( "id" ), item->id );
    node.setAttribute ( QLatin1String ( "file" ), item->file );
    node.appendChild ( createCDATASection ( item->words.join ( "," ) ) );
    rootNode().appendChild ( node );
  }
}

const QString KeywordsDom::dump() const
{
  return toString ( 1 );
}

bool KeywordsDom::saveDocument ( const QString &path ) const
{
  QFile fp ( path );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << toString ( 1 );
    fp.close();
    return true;
  }
  return false;
}
