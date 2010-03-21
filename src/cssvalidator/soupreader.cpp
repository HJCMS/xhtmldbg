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

#include "soupreader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

SoupReader::SoupReader ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "soupreader" ) );
}

const QString SoupReader::prepareMessage ( const QDomNode &node ) const
{
  //nodeItem( nodes.item(i).firstChild() );
  QString value;
  if ( ! node.isElement() )
    return value;

  QDomElement element = node.toElement();
  if ( ! element.hasChildNodes() )
    return value;

  QStringList exclude ( "m:errortype" );
  exclude << "m:errorsubtype" << "m:skippedstring";

  QStringList messages ( trUtf8 ( "Line" ) );
  for ( QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling() )
  {
    QDomText t = n.firstChild().toText();
    if ( !t.isNull() && ! exclude.contains ( n.nodeName() ) )
      messages << t.data().trimmed();
  }

  if ( messages.size() > 2 )
    return messages.join ( " " );

  return value;
}

/**
* Suche m:errorlist Element
* @code
*   <m:errorlist>...</m:errorlist>
* @endcode
**/
const QDomElement SoupReader::errorListNode ( int index )
{
  QDomNode node = dom.elementsByTagName ( "m:errorlist" ).item ( index );
  if ( node.isElement() )
    return node.toElement();

  return QDomElement();
}

/**
* Suche m:warninglist Element
* @code
*   <m:warninglist>...</m:warninglist>
* @endcode
**/
const QDomElement SoupReader::warningListNode ( int index )
{
  QDomNode node = dom.elementsByTagName ( "m:warninglist" ).item ( index );
  if ( node.isElement() )
    return node.toElement();

  return QDomElement();
}

const QString SoupReader::nodeItem ( const QDomNode &node ) const
{
  QString value;
  if ( ! node.isElement() )
    return value;

  QDomElement childNode = node.toElement();
  if ( childNode.firstChild().isElement() )
  {
    QDomElement nextChild = childNode.firstChild().toElement();
    if ( ! nextChild.nodeValue().isEmpty() )
      return nextChild.nodeValue().trimmed();
  }
  return value;
}

/**
* Suche alle errors im gesamtem dom.
* @code
*   <m:errors>...</m:errors>
* @endcode
**/
void SoupReader::readAllErrors()
{
  QDomNodeList nodes = errorListNode().elementsByTagName ( "m:error" );
  for ( int i = 0; i < nodes.count(); i++ )
  {
    QString buffer = prepareMessage ( nodes.item ( i ) );
    if ( buffer.isEmpty() )
      continue;

    emit parserError ( buffer );
  }
}

/**
* Suche alle warnings im gesamtem dom.
* @code
*   <m:warnings>...</m:warnings>
* @endcode
**/
void SoupReader::readAllWarnings()
{
  QDomNodeList nodes = warningListNode().elementsByTagName ( "m:warning" );
  for ( int i = 0; i < nodes.count(); i++ )
  {
    QString buffer = prepareMessage ( nodes.item ( i ) );
    if ( buffer.isEmpty() )
      continue;

    emit warnings ( buffer );
  }
}

/**
* Wenn m:validity true ist dann nicht weiter nach fehlern suchen.
* @code
*   <m:validity>true</m:validity>
* @endcode
**/
bool SoupReader::hasErrors()
{
  QDomNode node = dom.elementsByTagName ( "m:validity" ).item ( 0 );
  if ( node.isElement() && node.toElement().firstChild().nodeValue() == "true" )
  {
    emit congratulation ( trUtf8 ( "Congratulations! No Error Found." ) );
    return false;
  }
  return true;
}

bool SoupReader::readReceivedXML ( const QByteArray &xml )
{
  QString errorMsg;
  int errorLine;
  int errorColumn;

  QFile fp ( "/tmp/xhtmldbg_parsing.xml" );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << xml;
    fp.close();
  }

  if ( ! dom.setContent ( xml, &errorMsg, &errorLine, &errorColumn ) )
  {
    QString erdom = QString ( "%1 at Line %2 Column %3" )
                    .arg ( errorMsg, QString::number ( errorLine ), QString::number ( errorColumn ) );
    emit parserError ( erdom );
    return false;
  }

  if ( hasErrors() )
    readAllErrors();

  readAllWarnings();

  return true;
}

SoupReader::~SoupReader()
{}
