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

#include "bookmarktreereader.h"

#include <QtCore>

BookmarkTreeReader::BookmarkTreeReader ( QTreeWidget * parent )
    : m_treeWidget ( parent )
    , bookmarkIcon ( QIcon::fromTheme ( QLatin1String ( "bookmarks" ) ) )
    , folderIcon ( QIcon::fromTheme ( QLatin1String ( "folder-bookmark" ) ) )
{}

void BookmarkTreeReader::rebuildBookmarkList()
{
  Q_ASSERT ( xmlStream.isStartElement() && xmlStream.name() == "xbel" );

  while ( xmlStream.readNextStartElement() )
  {
    if ( xmlStream.name() == "folder" )
      readFolder ( 0 );
    else if ( xmlStream.name() == "bookmark" )
      readBookmark ( 0 );
    else if ( xmlStream.name() == "separator" )
      readSeparator ( 0 );
    else
      xmlStream.skipCurrentElement();
  }
}

void BookmarkTreeReader::readTitle ( QTreeWidgetItem* item )
{
  Q_ASSERT ( xmlStream.isStartElement() && xmlStream.name() == "title" );

  QString title = xmlStream.readElementText();
  item->setText ( 0, title );
}

void BookmarkTreeReader::readFolder ( QTreeWidgetItem* item )
{
  Q_ASSERT ( xmlStream.isStartElement() && xmlStream.name() == "folder" );

  QTreeWidgetItem* folder = createChildItem ( item );
  folder->setFlags ( folder->flags() | Qt::ItemIsEditable );
  folder->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );
  folder->setIcon ( 0, folderIcon );

  bool folded = ( xmlStream.attributes().value ( "folded" ) != "no" );
  m_treeWidget->setItemExpanded ( folder, !folded );

  while ( xmlStream.readNextStartElement() )
  {
    if ( xmlStream.name() == "title" )
      readTitle ( folder );
    else if ( xmlStream.name() == "folder" )
      readFolder ( folder );
    else if ( xmlStream.name() == "bookmark" )
      readBookmark ( folder );
    else if ( xmlStream.name() == "separator" )
      readSeparator ( folder );
    else
      xmlStream.skipCurrentElement();
  }
}

void BookmarkTreeReader::readBookmark ( QTreeWidgetItem* item )
{
  Q_ASSERT ( xmlStream.isStartElement() && xmlStream.name() == "bookmark" );

  // Disable Drop Indicator for Bookmark Items
  Qt::ItemFlags flags = ( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled );

  QTreeWidgetItem* bookmark = createChildItem ( item );
  bookmark->setIcon ( 0, bookmarkIcon );
  bookmark->setText ( 0, QObject::tr ( "Unknown title" ) );
  bookmark->setText ( 1, xmlStream.attributes().value ( "href" ).toString() );
  bookmark->setFlags ( flags );

  while ( xmlStream.readNextStartElement() )
  {
    if ( xmlStream.name() == "title" )
      readTitle ( bookmark );
    else
      xmlStream.skipCurrentElement();
  }
}

void BookmarkTreeReader::readSeparator ( QTreeWidgetItem* item )
{
  Q_ASSERT ( xmlStream.isStartElement() && xmlStream.name() == "separator" );

  QTreeWidgetItem* separator = createChildItem ( item );
  separator->setFlags ( item->flags() & ~Qt::ItemIsSelectable );
  separator->setText ( 0, QString ( 30, 0xB7 ) );
  xmlStream.skipCurrentElement();
}

QTreeWidgetItem* BookmarkTreeReader::createChildItem ( QTreeWidgetItem* item )
{
  QTreeWidgetItem* childItem;
  if ( item )
    childItem = new QTreeWidgetItem ( item );
  else
    childItem = new QTreeWidgetItem ( m_treeWidget );

  childItem->setData ( 0, Qt::UserRole, xmlStream.name().toString() );
  return childItem;
}

bool BookmarkTreeReader::read ( QIODevice* fp )
{
  xmlStream.setDevice ( fp );
  if ( xmlStream.readNextStartElement() )
  {
    if ( xmlStream.name() == "xbel" && xmlStream.attributes().value ( "version" ) == "1.0" )
      rebuildBookmarkList();
    else
      xmlStream.raiseError ( QLatin1String ( "Bookmark file is not Valid XBEL Scheme!" ) );
  }
  return !xmlStream.error();
}

BookmarkTreeReader::~BookmarkTreeReader()
{}
