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

#include "xdebugreader.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtXml */
#include <QtXml/QDomDocumentType>
#include <QtXml/QDomNamedNodeMap>

// QDomDocumentType

XDebugReader::XDebugReader ( const QString &content )
    : QDomDocument ()
{
  QString errorMsg;
  if ( ! setContent ( content, &errorMsg ) )
    Q_ASSERT_X ( false, "(XHTMLDBG) XDebug XML Reader Invalid Document Structure.",
                 errorMsg.toAscii() );
}

XDebugReader::XDebugReader ( const QDomDocument &xml )
    : QDomDocument ( xml )
{
  QString errorMsg;
  if ( ! setContent ( xml.toString(), &errorMsg ) )
    Q_ASSERT_X ( false, "(XHTMLDBG) XDebug XML Reader Invalid Document Structure.",
                 errorMsg.toAscii() );
}

/**
* Sucht nach dem Attribut Namen und gibt bei erfolg den Inhalt zurück.
*/
const QString XDebugReader::attributeValue ( const QString &attrName )
{
  QDomElement e = documentElement();
  if ( e.hasAttributes() )
  {
    QDomNamedNodeMap map = e.attributes();
    for ( int i = 0; i < map.size(); i++ )
    {
      QString name = map.item ( i ).nodeName();
      if ( name.contains ( attrName, Qt::CaseInsensitive ) )
        return e.attribute ( name );
    }
  }
  return QString::null;
}

/**
* Erstellt eine Liste aller Attribute des Übergebenen Elementes
*/
const QHash<QString,QString> XDebugReader::attributeList ( const QDomElement &childNode )
{
  QHash<QString,QString> hash;
  if ( childNode.hasAttributes() )
  {
    QDomNamedNodeMap map = childNode.attributes();
    for ( int i = 0; i < map.size(); i++ )
    {
      QString attrName = map.item ( i ).nodeName();
      hash[attrName] = childNode.attribute ( attrName );
    }
  }
  return hash;
}

XDebugReader::~XDebugReader()
{}
