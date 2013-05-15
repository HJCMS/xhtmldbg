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

#include "formtablemodel.h"
#include "formtabledelegation.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHeaderView>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

FormTableModel::FormTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
{
  setObjectName ( QLatin1String ( "FormTableModel" ) );

  QHeaderView* header = parent->horizontalHeader ();
  header->setResizeMode ( QHeaderView::ResizeToContents );
  header->setMovable ( false );
  header->setDefaultAlignment ( Qt::AlignLeft );
  header->setStretchLastSection ( true );

  m_formTableDelegation = new FormTableDelegation ( parent );
  parent->setItemDelegate ( m_formTableDelegation );
}

Qt::ItemFlags FormTableModel::flags ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  return ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
}

QVariant FormTableModel::headerData ( int section, Qt::Orientation orient, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orient == Qt::Horizontal )
  {
    switch ( section )
    {
      case 0:
        return QVariant ( i18n ( "Element" ) );

      case 1:
        return QVariant ( i18n ( "Type" ) );

      case 2:
        return QVariant ( i18n ( "Name" ) );

      case 3:
        return QVariant ( i18n ( "Id" ) );

      case 4:
        return QVariant ( i18n ( "Value" ) );

      default:
        return QVariant ( i18n ( "Unknown" ) );
    }
  }
  // Vertikaler eintrag
  return QString ( "%1" ).arg ( QString::number ( section ) );
}

QVariant FormTableModel::data ( const QModelIndex & index, int role ) const
{
  if ( !index.isValid() || items.size() < 1 )
    return QVariant();

  if ( role != Qt::DisplayRole )
    return QVariant();

  int row = index.row();
  if ( ! items.at ( row ).isNull() )
  {
    QWebElement element = items.at ( row );
    if ( index.column() == 0 )
      return QVariant ( element.tagName() );
    if ( ( index.column() == 1 ) && ( element.hasAttribute ( "type" ) ) )
      return QVariant ( element.attribute ( "type", "" ) );
    if ( ( index.column() == 2 ) && element.hasAttribute ( "name" ) )
      return QVariant ( element.attribute ( "name", "" ) );
    if ( ( index.column() == 3 ) && element.hasAttribute ( "id" ) )
      return QVariant ( element.attribute ( "id", "" ) );
    if ( ( index.column() == 4 ) && element.hasAttribute ( "value" ) )
      return QVariant ( element.attribute ( "value", "" ) );
  }
  return QVariant();
}

bool FormTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool retval = false;
  if ( ! index.isValid() || role != Qt::EditRole )
    return retval;

  if ( ! value.isValid() || value.toString().isEmpty() )
    return retval;

  int row = index.row();
  if ( ! items.at ( row ).isNull() )
  {
    QWebElement element = items.at ( row );
    if ( ( index.column() == 4 ) && element.hasAttribute ( "value" ) )
    {
      element.setAttribute ( "value", value.toString() );
      retval = true;
    }
  }
  return retval;
}

int FormTableModel::columnCount ( const QModelIndex &/*parent*/ ) const
{
  return 5;
}

int FormTableModel::rowCount ( const QModelIndex &/*parent*/ ) const
{
  return items.size();
}

QModelIndex FormTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

bool FormTableModel::removeRow ( int row, const QModelIndex &parent )
{
  if ( ( row < 0 ) || ( row > items.size() ) )
    return false;

  beginRemoveRows ( parent, row, row );
  items.removeAt ( row );
  endRemoveRows();

  return true;
}

void FormTableModel::insertRowItem ( QWebElement item )
{
  beginInsertRows ( QModelIndex(), items.size(), items.size() );
  items.append ( item );
  endInsertRows();
}

void FormTableModel::clearTableContents()
{
  if ( items.size() > 0 )
  {
    beginResetModel();
    items.clear();
    endResetModel();
  }
}

QWebElement FormTableModel::rowItem ( int row )
{
  return items.at ( row );
}

FormTableModel::~FormTableModel()
{
  clearTableContents();
}
