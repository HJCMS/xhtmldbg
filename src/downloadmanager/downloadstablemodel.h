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

#ifndef DOWNLOADSTABLEMODEL_H
#define DOWNLOADSTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

class Downloader;
class DownloadsProgressWidget;

/**
* @class DownloadsTableModel
*/
class DownloadsTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QHeaderView* m_tableHeader;
    QList<Downloader*> downloads;

  protected:
    QTableView* table;

  Q_SIGNALS:
    void modified ( bool );

  public Q_SLOTS:
    void addDownload ( Downloader *item, const QModelIndex &parent = QModelIndex() );
    void abortDownload ( const QModelIndex & );
    void removeDownload ( const QModelIndex & );
    void restartDownload ( const QModelIndex & );

  public:
    DownloadsTableModel ( QTableView * parent = 0 );
    Downloader* rowItem ( int row );
    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual ~DownloadsTableModel();
};

#endif
