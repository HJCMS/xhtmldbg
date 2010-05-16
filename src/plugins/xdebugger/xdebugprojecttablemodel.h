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

#ifndef XDEBUGPROJECTTABLEMODEL_H
#define XDEBUGPROJECTTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractTableModel>
#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QTableView>
#include <QtGui/QWidget>

class XDebugProjectItem;

class XDebugProjectTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  protected:
    QTableView* table;
    QList<XDebugProjectItem*> projects;

  Q_SIGNALS:
    void modified ( bool );

  public Q_SLOTS:
    void addProject ( XDebugProjectItem *item, const QModelIndex &parent = QModelIndex() );
    void removeProject ( int row, const QModelIndex &parent = QModelIndex() );

  public:
    XDebugProjectTableModel ( QTableView * parent = 0 );
    XDebugProjectItem* rowItem ( int row );
    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual ~XDebugProjectTableModel();
};

#endif
