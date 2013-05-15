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

#ifndef KEYWORDSTABLEMODEL_H
#define KEYWORDSTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QTableView>
#include <QtGui/QWidget>

class KeywordsTableDelegation;
class KeywordsTableItem;

class Q_DECL_EXPORT KeywordsTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QList<KeywordsTableItem*> items;
    KeywordsTableDelegation* m_tableDelegation;
    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    QVariant headerData ( int section, Qt::Orientation orient = Qt::Horizontal,
                          int role = Qt::DisplayRole ) const;

  protected:
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

  Q_SIGNALS:
    void modified ( bool );

  public Q_SLOTS:
    void clearTableContents();

  public:
    KeywordsTableModel ( QTableView * parent = 0 );
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    bool insertRow ( int row, const QModelIndex &parent = QModelIndex() );
    bool removeRow ( int row, const QModelIndex &parent = QModelIndex() );
    void insertRowItem ( KeywordsTableItem * item );
    KeywordsTableItem* rowItem ( int row );
    virtual ~KeywordsTableModel();
};

#endif
