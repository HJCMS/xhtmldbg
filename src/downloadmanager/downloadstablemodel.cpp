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

#include "downloadstablemodel.h"
#include "downloader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

/** @class DownloadsTableModel */
DownloadsTableModel::DownloadsTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
    , table ( parent )
{
  setObjectName ( QLatin1String ( "downloadstablemodel" ) );
  table->setAlternatingRowColors ( true );
  // table->verticalHeader()->hide();
  table->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  table->setWordWrap ( false );
  table->setSelectionBehavior ( QAbstractItemView::SelectRows );

  /* Zellen anpassen */
  QFontMetrics metrics ( table->font() );
  m_tableHeader = table->horizontalHeader();
  m_tableHeader->setCascadingSectionResizes ( true );
  m_tableHeader->setMinimumSectionSize ( metrics.boundingRect ( "00:00:00.000" ).width() );
  m_tableHeader->setDefaultAlignment ( Qt::AlignLeft );
  m_tableHeader->setResizeMode ( QHeaderView::ResizeToContents );
}

/**
* Signal Verarbeitung aktualisierungen an die Zellen weiter geben!
* Verwende hierbei den ModelIndex von Spalte Progress
* siehe auch @ref addDownload und @ref Downloader::setStartProgressModel
*/
void DownloadsTableModel::updateDownloadStatus ( const QModelIndex &m )
{
  emit table->update ( m ); // Zelle 0=Progress
  emit table->update ( index ( m.row(), 1 ) ); // Zelle 1=Time
  emit table->update ( index ( m.row(), 2 ) ); // Zelle 2=Size
}

/**
* Einen Neuen Spalteneintrag einfügen
*/
void DownloadsTableModel::addDownload ( Downloader *item, const QModelIndex &parent )
{
  beginInsertRows ( parent, downloads.size(), downloads.size() );
  downloads << item;
  endInsertRows();
  item->setStartProgressModel ( index ( ( downloads.size() - 1 ), 0, parent ) );
  connect ( item, SIGNAL ( dataChanged ( const QModelIndex & ) ),
            this, SLOT ( updateDownloadStatus ( const QModelIndex & ) ) );
}

/**
* Denn lade Prozess für diesen Download abbrechen
*/
void DownloadsTableModel::abortDownload ( const QModelIndex &index )
{
  if ( ( index.row() < 0 ) || ( index.row() > downloads.size() ) )
    return;

  downloads.at ( index.row() )->abort();
  emit modified ( true );
}

/**
* Diesen Download aus der Tabelle entfernen
* und das Objekt löschen.
*/
void DownloadsTableModel::removeDownload ( const QModelIndex &index )
{
  if ( ( index.row() < 0 ) || ( index.row() > downloads.size() ) )
    return;

  Downloader* item = downloads.at ( index.row() );
  beginRemoveRows ( index.parent(), index.row(), index.row() );
  downloads.removeAt ( index.row() );
  endRemoveRows();

  item->abort();
  item->deleteLater();
  emit modified ( true );
}

void DownloadsTableModel::restartDownload ( const QModelIndex &index )
{
  if ( ( index.row() < 0 ) || ( index.row() > downloads.size() ) )
    return;

  downloads.at ( index.row() )->restart ();
}

const QList<Downloader*> DownloadsTableModel::downloadItems()
{
  return downloads;
}

/**
* Gibt den Zeiger auf einen SpaltenEintrag zurück
*/
Downloader* DownloadsTableModel::rowItem ( int row )
{
  return downloads.at ( row );
}

/**
* Wenn mit einem TableModel gearbeitet wird müssen die
* Editor flags neu gesetzt werden.
*/
Qt::ItemFlags DownloadsTableModel::flags ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags  = ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
  return flags;
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
  return 5;
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
          return item->status();

        case 1:
          return item->uploadTime();

        case 2:
          return item->fileSize();

        case 3:
          return item->url();

        case 4:
          return item->destFile();

        default:
          return val;
      }
    }
  }
  return val;
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
      {
        return i18n ( "Progress" );
      }

      case 1:
      {
        return i18n ( "Time" );
      }

      case 2:
      {
        return i18n ( "Size" );
      }

      case 3:
      {
        return i18n ( "Url" );
      }

      case 4:
      {
        return i18n ( "Destination" );
      }

      default:
        return i18n ( "Unknown" );
    }
  }

  // Vertikaler eintrag
  return QString::number ( section );
}

DownloadsTableModel::~DownloadsTableModel()
{
  downloads.clear();
}
