/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "formbookmarks.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QListWidget>
#include <QtGui/QTreeWidgetItemIterator>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

FormBookmarks::FormBookmarks ( QWidget * parent )
    : QTreeWidget ( parent )
{
  setObjectName ( QLatin1String ( "FormBookmarker" ) );
  setHeaderLabel ( i18n ( "Bookmarks" ) );
  headerItem()->setIcon ( 0, KIcon ( "folder-bookmark" ) );

  connect ( this, SIGNAL ( doubleClicked ( const QModelIndex & ) ),
            this, SLOT ( itemDoubleClicked ( const QModelIndex & ) ) );

  connect ( this, SIGNAL ( clicked ( const QModelIndex & ) ),
            this, SLOT ( itemClicked ( const QModelIndex & ) ) );
}

QTreeWidgetItem* FormBookmarks::addProjectItem ( const QString &project )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( this );
  item->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );
  item->setText ( 0, project );
  item->setIcon ( 0, KIcon ( "folder-bookmark" ) );
  item->setExpanded ( true );
  addTopLevelItem ( item );
  return item;
}

QTreeWidgetItem* FormBookmarks::findTopLevelItem ( const QString &project )
{
  QTreeWidgetItemIterator it ( this, QTreeWidgetItemIterator::NotHidden );
  while ( *it )
  {
    if ( ( *it )->text ( 0 ) == project )
      return *it;
    ++it;
  }
  return addProjectItem ( project );
}

void FormBookmarks::itemDoubleClicked ( const QModelIndex &index )
{
  if ( ! index.isValid() )
    return;

  FormBookmarkItem* item = static_cast<FormBookmarkItem*> ( index.internalPointer() );
  if ( item->getUrl().isValid() )
    emit bookmarkDoubleClicked ( item, index.column() );
}

void FormBookmarks::itemClicked ( const QModelIndex &index )
{
  if ( ! index.isValid() )
    return;

  FormBookmarkItem* item = static_cast<FormBookmarkItem*> ( index.internalPointer() );
  if ( item->getUrl().isValid() )
    emit bookmarkClicked ( item, index.column() );
}

void FormBookmarks::addItem ( const QString &project, FormBookmarkItem * item )
{
  findTopLevelItem ( project )->addChild ( item );
}

FormBookmarks::~FormBookmarks()
{}
