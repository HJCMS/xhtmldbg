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

#include "xdebugprojecttablemodel.h"
#include "xdebugprojecttabledelegation.h"
#include "xdebugprojectitem.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHeaderView>

/**
* @class XDebugProjectTableModel
*/
XDebugProjectTableModel::XDebugProjectTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
    , table ( parent )
{
  setObjectName ( QLatin1String ( "projecttablemodel" ) );
  table->horizontalHeader()->setResizeMode ( QHeaderView::Stretch );
  table->setItemDelegate ( new XDebugProjectTableDelegation ( table ) );
}

/**
* Einen Neuen Spalteneintrag einfügen
*/
void XDebugProjectTableModel::addProject ( XDebugProjectItem *item, const QModelIndex &parent )
{
  beginInsertRows ( parent, projects.size(), projects.size() );
  projects << item;
  endInsertRows();
  emit modified ( true );
}

void XDebugProjectTableModel::removeProject ( int row, const QModelIndex &parent )
{
  if ( ( row < 0 ) || ( row > projects.size() ) )
    return;

  XDebugProjectItem* item = projects.at ( row );

  beginRemoveRows ( parent, row, row );
  projects.removeAt ( row );
  endRemoveRows();

  delete item;
  emit modified ( true );
}

/**
* Gibt den Zeiger auf einen SpaltenEintrag zurück
*/
XDebugProjectItem* XDebugProjectTableModel::rowItem ( int row )
{
  return projects.at ( row );
}

/**
* Wenn mit einem TableModel gearbeitet wird müssen die
* Editor flags neu gesetzt werden.
*/
Qt::ItemFlags XDebugProjectTableModel::flags ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags  = ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
  return flags;
}

QModelIndex XDebugProjectTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > projects.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, projects.size() );
}

/**
* Die aktuelle Anzahl von @ref projects ist gleichzeitig auch die Spaltenanzahl.
*/
int XDebugProjectTableModel::rowCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return projects.size();
}

/**
* Setze die Anzahl der Horizontalen Kopf-Datenfelder
* Aktuell wird im Moment verwendet:
* @li "Project Name" => @ref XDebugProjectItem::getName
* @li "Remote Port" => @ref XDebugProjectItem::getPort
* @li "Remote Host" => @ref XDebugProjectItem::getRemoteHost
* @li "Session Key" => @ref XDebugProjectItem::getSessionId
* @see XDebugProjectTableModel::headerData
*/
int XDebugProjectTableModel::columnCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return 4;
}

/**
* Liest die Dateninhalte aus dem Pointer von @ref projects
*/
QVariant XDebugProjectTableModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( !index.isValid() )
    return val;

  if ( role == Qt::DisplayRole )
  {
    int row = index.row();
    if ( projects.size () >= row )
    {
      XDebugProjectItem* prj = projects.at ( row );

      switch ( index.column() )
      {
        case 0:
          return prj->getName();

        case 1:
          return prj->getPort();

        case 2:
          return prj->getRemoteHost();

        case 3:
          return prj->getSessionId();

        default:
          return val;
      }
    }
  }
  return val;
}

/**
* Wird nach einem Editieren aufgerufen und Modifiziert unter anderem
* auch den Datensatz von @ref projects. Bei einem Erfogreichen Update wird
* das Signal @ref dataChanged aufgerufen! Diese Methode wird
* Hauptsächlich von @ref XDebugProjectTableDelegation::setModelData verwendet.
*/
bool XDebugProjectTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool onUpdate = false;
  if ( ! value.isValid() || value.toString().isEmpty() )
    return onUpdate;

  if ( role != Qt::DisplayRole )
    return onUpdate;

  if ( projects.size () >= index.row() )
  {
    XDebugProjectItem* prj = projects.at ( index.row() );

    switch ( index.column() )
    {
      case 0:
      {
        prj->setName ( value.toString() );
        onUpdate = true;
      }
      break;

      case 1:
      {
        prj->setPort ( value.toString() );
        onUpdate = true;
      }
      break;

      case 2:
      {
        prj->setRemoteHost ( value.toString() );
        onUpdate = true;
      }
      break;

      case 3:
      {
        prj->setSessionId ( value.toString() );
        onUpdate = true;
      }
      break;

      default:
        onUpdate = false;
        break;
    }
  }

  if ( onUpdate )
  {
#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG_VERBOSE
    qDebug() << "TableModelData:" << index.row() << index.column() << value;
#endif
    emit dataChanged ( index, index );
    return true;
  }
  return false;
}

/**
* Füge die Datenkopf Beschriftungen ein.
*/
QVariant XDebugProjectTableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal )
  {
    switch ( section )
    {
      case 0:
        return trUtf8 ( "Project Name" );

      case 1:
        return trUtf8 ( "Port" );

      case 2:
        return trUtf8 ( "Hostname" );

      case 3:
        return trUtf8 ( "Session Key" );

      default:
        return trUtf8 ( "Unknown" );
    }
  }
  // Vertikaler eintrag
  return QString ( "%1" ).arg ( section );
}

XDebugProjectTableModel::~XDebugProjectTableModel()
{}
