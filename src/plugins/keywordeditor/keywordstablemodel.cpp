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

#include "keywordstablemodel.h"
#include "keywordstabledelegation.h"
#include "keywordstableitem.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHeaderView>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordsTableModel::KeywordsTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
    , items ()
{
  setObjectName ( QLatin1String ( "KeywordsTableModel" ) );

  QHeaderView* header = parent->horizontalHeader ();
  header->setResizeMode ( QHeaderView::ResizeToContents );
  header->setMovable ( false );
  header->setDefaultAlignment ( Qt::AlignLeft );
  header->setStretchLastSection ( true );

  m_tableDelegation = new KeywordsTableDelegation ( parent );
  parent->setItemDelegate ( m_tableDelegation );
}

Qt::ItemFlags KeywordsTableModel::flags ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  return ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
}

QVariant KeywordsTableModel::headerData ( int section, Qt::Orientation orient, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orient == Qt::Horizontal )
  {
    switch ( section )
    {
      case 0:
        return QVariant ( i18n ( "Identifier" ) );

      case 1:
        return QVariant ( i18n ( "Filename" ) );

      case 2:
        return QVariant ( i18n ( "Keywords" ) );

      default:
        return QVariant ( i18n ( "Unknown" ) );
    }
  }
  // Vertikaler eintrag
  return QString ( "%1" ).arg ( QString::number ( section ) );
}

QVariant KeywordsTableModel::data ( const QModelIndex & index, int role ) const
{
  if ( !index.isValid() || items.size() < 1 )
    return QVariant();

  int row = index.row();
  if ( items.at ( row ) )
  {
    if ( role == Qt::DisplayRole && index.column() == 0 )
      return QVariant ( items.at ( row )->id );
    if ( role == Qt::DisplayRole && index.column() == 1 )
      return QVariant ( items.at ( row )->file );
    if ( role == Qt::DisplayRole && index.column() == 2 )
      return QVariant ( items.at ( row )->words.join ( "," ) );
  }
  return QVariant();
}

bool KeywordsTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool retval = false;
  if ( ! index.isValid() || role != Qt::EditRole )
    return retval;

  if ( ! value.isValid() || value.toString().isEmpty() )
    return retval;

  int row = index.row();
  if ( items.at ( row ) )
  {
    if ( index.column() == 0 )
    {
      items.at ( row )->id = value.toString();
      retval = true;
    }
    else if ( index.column() == 1 )
    {
      items.at ( row )->file = value.toString();
      retval = true;
    }
    else if ( index.column() == 2 )
    {
      items.at ( row )->words = value.toString().split ( "," );
      retval = true;
    }
  }
  return retval;
}

int KeywordsTableModel::columnCount ( const QModelIndex &/*parent*/ ) const
{
  return 3;
}

int KeywordsTableModel::rowCount ( const QModelIndex &/*parent*/ ) const
{
  return items.size();
}

QModelIndex KeywordsTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

bool KeywordsTableModel::insertRow ( int row, const QModelIndex &parent )
{
  if ( ( row < 0 ) || ( row > items.size() ) )
    return false;

  beginInsertRows ( parent, items.size(), items.size() );
  items.prepend ( new KeywordsTableItem () );
  endInsertRows();
  return true;
}

bool KeywordsTableModel::removeRow ( int row, const QModelIndex &parent )
{
  if ( ( row < 0 ) || ( row > items.size() ) )
    return false;

  beginRemoveRows ( parent, row, row );
  KeywordsTableItem* item = items.takeAt ( row );
  endRemoveRows();

  if ( item )
    delete item;

  return true;
}

void KeywordsTableModel::insertRowItem ( KeywordsTableItem * item )
{
  beginInsertRows ( QModelIndex(), items.size(), items.size() );
  items.append ( item );
  endInsertRows();
}


void KeywordsTableModel::clearTableContents()
{
  if ( items.size() > 0 )
  {
    beginResetModel();
    items.clear();
    endResetModel();
  }
}

KeywordsTableItem* KeywordsTableModel::rowItem ( int row )
{
  return items.at ( row );
}

KeywordsTableModel::~KeywordsTableModel()
{
  clearTableContents();
}
