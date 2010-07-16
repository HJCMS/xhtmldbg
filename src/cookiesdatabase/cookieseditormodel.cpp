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

#include "cookieseditormodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtSql */
#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlError>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlQuery>

CookiesEditorModel::CookiesEditorModel ( const QSqlDatabase &db, QTableView * parent )
    : QAbstractTableModel ( parent )
    , sql ( db )
{
}

bool CookiesEditorModel::select()
{
  if ( sql.isOpen() )
  {
    QString queryString ( "SELECT AccessType,Hostname,AllowThirdParty,RFC2109 " );
    queryString.append ( "FROM cookieshandle WHERE ( Hostname != '' ) ORDER BY Hostname ASC;" );
    QSqlQuery query = sql.exec ( queryString );
    if ( query.lastError().type() == QSqlError::NoError )
    {
      QSqlRecord rec = query.record();
      if ( rec.count() > 0 )
      {
        items.clear();
        beginInsertRows ( QModelIndex(), items.size(), items.size() );
        while ( query.next() )
        {
          CookiesAccessItem it;
          it.Access = query.value ( rec.indexOf ( "AccessType" ) ).toUInt();
          it.Hostname = query.value ( rec.indexOf ( "Hostname" ) ).toString();
          it.AllowThirdParty = query.value ( rec.indexOf ( "AllowThirdParty" ) ).toUInt();
          it.RFC2109 = query.value ( rec.indexOf ( "RFC2109" ) ).toUInt();
          items.append ( it );
        }
        endInsertRows();
      }
      query.finish();
      return true;
    }
  }
  else
    return false;
}

/**
* Wenn mit einem TableModel gearbeitet wird müssen die
* Editor flags neu gesetzt werden.
*/
Qt::ItemFlags CookiesEditorModel::flags ( const QModelIndex &index ) const
{
  if ( ! index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags  = ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
  return flags;
}

QModelIndex CookiesEditorModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() )
    return QModelIndex();

  if ( column > columnCount () )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

/**
* Setze die Anzahl der Einträge
*/
int CookiesEditorModel::rowCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return items.size();
}

/**
* Setze die Anzahl der Horizontalen Kopf-Datenfelder
*/
int CookiesEditorModel::columnCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return 4;
}

/**
* Füge die Datenkopf Beschriftungen ein.
*/
QVariant CookiesEditorModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  QVariant fallback;
  if ( orientation == Qt::Vertical )
    return fallback;

  if ( role != Qt::DisplayRole )
    return fallback;

  switch ( section )
  {
    case 0:
      // AccessType
      return trUtf8 ( "Access" );

    case 1:
      // Hostname
      return trUtf8 ( "Domain" );

    case 2:
      // AllowThirdParty
      return trUtf8 ( "Allow third party" );

    case 3:
      // RFC2109
      return trUtf8 ( "rfc2109" );

    default:
      return trUtf8 ( "Unknown" );
  }

  return fallback;
}

QVariant CookiesEditorModel::data ( const QModelIndex & index, int role ) const
{
  QVariant value;
  if ( ! index.isValid() )
    return value;

  if ( role != Qt::DisplayRole )
    return value;

  switch ( index.column() )
  {
    case 0:
    {
      // AccessType
      return items.at ( index.row() ).Access;
    }

    case 1:
    {
      // Hostname
      return items.at ( index.row() ).Hostname;
    }

    case 2:
    {
      // AllowThirdParty
      return items.at ( index.row() ).AllowThirdParty;
    }

    case 3:
    {
      // RFC2109
      return items.at ( index.row() ).RFC2109;
    }

    default:
      return trUtf8 ( "Unknown" );
  }
}

bool CookiesEditorModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !index.isValid() )
    return false;

  if ( role != Qt::EditRole )
    return false;

  switch ( index.column() )
  {
    case 0:
    {
      // AccessType
      qDebug() << Q_FUNC_INFO << "Access" << value.toInt();
      return true;
    }

    case 1:
    {
      // Hostname
      qDebug() << Q_FUNC_INFO << "Domain" << value.toString();
      return true;
    }

    case 2:
    {
      // AllowThirdParty
      qDebug() << Q_FUNC_INFO << "Allow third party" << value.toInt();
      return true;
    }

    case 3:
    {
      // RFC2109
      qDebug() << Q_FUNC_INFO << "rfc2109" << value.toInt();
      return true;
    }

    default:
      return false;
  }
  return true;
}

CookiesEditorModel::~CookiesEditorModel()
{}
