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

#include "downloadstablemodel.h"
#include "downloader.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

DownloadsTableModel::DownloadsTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
    , table ( parent )
{
  setObjectName ( QLatin1String ( "downloadstablemodel" ) );
  table->horizontalHeader()->setResizeMode ( QHeaderView::Stretch );
  table->setAlternatingRowColors ( true );
  table->verticalHeader()->hide();
  table->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  table->setWordWrap ( false );
  table->setCornerButtonEnabled ( false );
}

/**
* Einen Neuen Spalteneintrag einfügen
*/
void DownloadsTableModel::addDownload ( Downloader *item, const QModelIndex &parent )
{
  beginInsertRows ( parent, downloads.size(), downloads.size() );
  downloads << item;
  endInsertRows();
  emit modified ( true );
}

void DownloadsTableModel::removeDownload ( int row, const QModelIndex &parent )
{
  if ( ( row < 0 ) || ( row > downloads.size() ) )
    return;

  Downloader* item = downloads.at ( row );

  beginRemoveRows ( parent, row, row );
  downloads.removeAt ( row );
  endRemoveRows();

  delete item;
  emit modified ( true );
}

/**
* Gibt den Zeiger auf einen SpaltenEintrag zurück
*/
Downloader* DownloadsTableModel::rowItem ( int row )
{
  return downloads.at ( row );
}

QModelIndex DownloadsTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > downloads.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, downloads.size() );
}

/**
* Die aktuelle Anzahl von Downloads ist gleichzeitig auch die Spaltenanzahl.
*/
int DownloadsTableModel::rowCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return downloads.size();
}

/**
* Setze die Anzahl der Horizontalen Kopf-Datenfelder
* Aktuell wird im Moment verwendet:
* @li Url
* @li Progress
* @li Bytes
* @li Status
* @see DownloadsTableModel::headerData
*/
int DownloadsTableModel::columnCount ( const QModelIndex &parent ) const
{
  Q_UNUSED ( parent )
  return 4;
}

/**
* Liest die Dateninhalte aus dem Pointer von @ref downloads
*/
QVariant DownloadsTableModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( !index.isValid() )
    return val;

  if ( role == Qt::DisplayRole )
  {
    int row = index.row();
    if ( downloads.size () >= row )
    {
      Downloader* item = downloads.at ( row );

      switch ( index.column() )
      {
        case 0:
          return item->url();

        case 1:
          return QVariant ( "TODO:1" );

        case 2:
          return QVariant ( "TODO:2" );

        case 3:
          return QVariant ( "TODO:3" );

        default:
          return val;
      }
    }
  }
  return val;
}

/**
* Wird nach einem Editieren aufgerufen und Modifiziert unter anderem
* auch den Datensatz von @ref downloads. Bei einem Erfogreichen Update wird
* das Signal @ref dataChanged aufgerufen! Diese Methode wird
* Hauptsächlich von @ref DownloadsTableModel::setModelData verwendet.
*/
bool DownloadsTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool onUpdate = false;
  if ( ! value.isValid() || value.toString().isEmpty() )
    return onUpdate;

  if ( role != Qt::DisplayRole )
    return onUpdate;

  if ( downloads.size () >= index.row() )
  {
    Downloader* item = downloads.at ( index.row() );

    switch ( index.column() )
    {
      case 0:
      {
        // Kann nicht geändert werden!
        onUpdate = false;
      }
      break;

      case 1:
      {
        // Kann nicht geändert werden!
        onUpdate = false;
      }
      break;

      case 2:
      {
        // Kann nicht geändert werden!
        onUpdate = false;
      }
      break;

      case 3:
      {
        // Kann nicht geändert werden!
        onUpdate = false;
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
QVariant DownloadsTableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal )
  {
    switch ( section )
    {
      case 0:
        return trUtf8 ( "Url" );

      case 1:
        return trUtf8 ( "Progress" );

      case 2:
        return trUtf8 ( "Bytes" );

      case 3:
        return trUtf8 ( "Time" );

      default:
        return trUtf8 ( "Unknown" );
    }
  }
  // Vertikaler eintrag
  return QString ( "%1" ).arg ( section );
}

DownloadsTableModel::~DownloadsTableModel()
{}
