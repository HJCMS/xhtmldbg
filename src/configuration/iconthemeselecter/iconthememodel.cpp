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

#include "iconthememodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QIcon>

#define MODEL_COLUMNS 1

IconThemeModel::IconThemeModel ( QObject * parent )
    : QAbstractItemModel ( parent )
{}

/**
*
*/
QModelIndex IconThemeModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > itemDataset.size() )
    return QModelIndex();

  if ( column > MODEL_COLUMNS )
    return QModelIndex();

  return createIndex ( row, column, itemDataset.size() );
}

/**
*
*/
QModelIndex IconThemeModel::parent ( const QModelIndex & ) const
{
  return QModelIndex();
}

/**
*
*/
int IconThemeModel::rowCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return itemDataset.size();
}

/**
*
*/
int IconThemeModel::columnCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return MODEL_COLUMNS;
}

/**
*
*/
QVariant IconThemeModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( ! index.isValid() )
    return val;

  int r = index.row();
  if ( ( r >= itemDataset.size() ) || ( r <= itemDataset.size() ) )
    return itemDataset.at ( r ).data ( role );

  return val;
}

/**
* Einen neuen Pfad einfügen
*/
void IconThemeModel::insert ( const QString &index )
{
  QSettings settings ( index, QSettings::IniFormat );
  if ( settings.contains ( QString::fromUtf8 ( "16x16/actions/Size" ) ) )
  {
    IconThemeItem item ( settings );
    beginInsertRows ( QModelIndex(), itemDataset.size(), itemDataset.size() );
    itemDataset << item;
    endInsertRows();
  }
}

/**
* Leert das Model und setzt alle zeiger zurück.
*/
void IconThemeModel::clear()
{
  int size = itemDataset.size();
  if ( size < 1 )
    return;

  beginRemoveRows ( QModelIndex(), 0, size );
  itemDataset.clear();
  endRemoveRows();
}

IconThemeModel::~IconThemeModel()
{}
