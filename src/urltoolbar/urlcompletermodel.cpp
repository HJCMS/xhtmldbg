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

#include "urlcompletermodel.h"
#include "urlhistory.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

UrlCompleterModel::UrlCompleterModel ( QObject * parent )
    : QAbstractItemModel ( parent )
    , m_history ( new UrlHistory() )
{
  setObjectName ( QLatin1String ( "UrlCompleterModel" ) );
  initDefaults();
}

void UrlCompleterModel::initDefaults ()
{
  beginInsertRows ( QModelIndex(), items.size(), items.size() );
  items << m_history->load();
  endInsertRows();
}

void UrlCompleterModel::addItem ( const QString &item )
{
  if ( items.contains ( item ) )
    return;

  if ( ! item.contains ( "http://" ) )
    return;

  beginInsertRows ( QModelIndex(), items.size(), items.size() );
  items << item;
  endInsertRows();
}

QModelIndex UrlCompleterModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() || row < 0 )
    return QModelIndex();

  if ( column > columnCount() || column < 0 )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

QModelIndex UrlCompleterModel::parent ( const QModelIndex &index ) const
{
  if ( ! index.isValid() || ! index.parent().isValid() )
    return QModelIndex();

  return index.parent();
}

int UrlCompleterModel::rowCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return items.size();
}

int UrlCompleterModel::columnCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return 1;
}

QVariant UrlCompleterModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( ! index.isValid() )
    return val;

  if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) )
    return items.at ( index.row() );

  return val;
}

bool UrlCompleterModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  Q_UNUSED ( index );
  if ( ! value.isValid() )
    return false;

  QString data = value.toString();
  if ( items.contains ( data ) )
    return false;

  items << data;
  return true;
}

UrlCompleterModel::~UrlCompleterModel()
{
  m_history->save ( items );
  items.clear();
}
