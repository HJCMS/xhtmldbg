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

#include "bookmarkwriter.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>

/* QtXml */
#include <QtXml/QDomDocumentType>
#include <QtXml/QDomNode>
#include <QtXml/QDomProcessingInstruction>

static const QString qtidy_license()
{
  return QString::fromUtf8 ( "\n\
********************************************************************************\n\
* This file is part of the xhtmldbg project\n\
*\n\
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010\n\
*\n\
* This library is free software; you can redistribute it and/or\n\
* modify it under the terms of the GNU Library General Public\n\
* License as published by the Free Software Foundation; either\n\
* version 2 of the License, or (at your option) any later version.\n\
*\n\
* This library is distributed in the hope that it will be useful,\n\
* but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n\
* Library General Public License for more details.\n\
*\n\
* You should have received a copy of the GNU Library General Public License\n\
* along with this library; see the file COPYING.LIB.  If not, write to\n\
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,\n\
* Boston, MA 02110-1301, USA.\n\
********************************************************************************\n" );
}

static const QString doctype_system()
{
  return QString::fromUtf8 ( "http://pyxml.sourceforge.net/topics/dtds/xbel-1.0.dtd" );
}

static const QString doctype_public()
{
  return QString::fromUtf8 ( "+//IDN python.org//DTD XML Bookmark Exchange Language 1.0//EN//XML" );
}

BookmarkWriter::BookmarkWriter ( QObject * parent, QTreeWidget * widget )
    : QObject ( parent )
    , tree ( widget )
    , dom ( new QDomDocument () )
{
  setObjectName ( QLatin1String ( "bookmarkwriter" ) );
  QDomProcessingInstruction pi = dom->createProcessingInstruction ( "xml",
                                 "version = '1.0' encoding = 'utf-8'" );
  dom->appendChild ( pi );

  QDomImplementation impl = dom->implementation();
  QDomDocumentType doctype = impl.createDocumentType ( "xbel", doctype_public(), doctype_system() );
  dom->appendChild ( doctype );

  dom->appendChild ( dom->createComment ( qtidy_license() ) );

  xbel = dom->createElement ( "xbel" );
  xbel.setAttribute ( "version", "1.0" );
  dom->appendChild ( xbel );
}

void BookmarkWriter::addDomItem ( QTreeWidgetItem *item, QDomElement &node )
{
  QDomElement subNode;
  QString title = item->data ( 0, Qt::EditRole ).toString();
  bool folded = !tree->isItemExpanded ( item );

  if ( node.tagName() == "xbel" )
  {
    subNode = dom->createElement ( "folder" );
    subNode.setAttribute ( "folded", ( folded ? "yes" : "no" ) );
    xbel.appendChild ( subNode );

    QDomElement t = dom->createElement ( "title" );
    t.appendChild ( dom->createTextNode ( title ) );
    subNode.appendChild ( t );
  }
  else if ( node.tagName() == "folder" && item->columnCount() == 1 )
  {
    subNode = dom->createElement ( "folder" );
    subNode.setAttribute ( "folded", ( folded ? "yes" : "no" ) );
    node.appendChild ( subNode );

    QDomElement t = dom->createElement ( "title" );
    t.appendChild ( dom->createTextNode ( title ) );
    subNode.appendChild ( t );
  }
  else if ( node.tagName() == "folder" && item->columnCount() == 2 )
  {
    QString url = item->data ( 1, Qt::EditRole ).toString();
    subNode = dom->createElement ( "bookmark" );
    subNode.setAttribute ( "href", url );
    node.appendChild ( subNode );

    QDomElement t = dom->createElement ( "title" );
    t.appendChild ( dom->createTextNode ( title ) );
    subNode.appendChild ( t );
  }

  for ( int i = 0; i < item->childCount(); ++i )
  {
    addDomItem ( item->child ( i ), subNode );
  }
}

bool BookmarkWriter::prepareXML()
{
  if ( ! tree )
    return false;

  for ( int i = 0; i < tree->topLevelItemCount(); ++i )
  {
    addDomItem ( tree->topLevelItem ( i ), xbel );
  }
  return true;
}

bool BookmarkWriter::save()
{
  QString file = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  file.append ( QDir::separator() );

  QDir dir ( file );
  if ( ! dir.exists() )
    dir.mkpath ( dir.absolutePath() );

  file.append ( QLatin1String ( "bookmarks.xbel" ) );

  QFile fp ( file );
  fp.setPermissions ( dir.absolutePath(), ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) );
  if ( prepareXML() && fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream* stream = new QTextStream ( &fp );
    dom->save ( *stream, 1, QDomNode::EncodingFromDocument );
    fp.close();
    delete stream;
    return true;
  }
  else
  {
    // @note if save operation fail's remove corrupted bookmark file!
    fp.remove ( file );
  }
  return false;
}

BookmarkWriter::~BookmarkWriter()
{
  delete dom;
}
