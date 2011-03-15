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

#ifndef USERAGENTSTABLEMODEL_H
#define USERAGENTSTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractTableModel>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QTableView>
#include <QtGui/QHeaderView>

class UserAgent;

class UserAgentsTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QList<UserAgent*> agentList;
    QHeaderView* m_tableHeader;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags ( const QModelIndex & ) const;
    QVariant data ( const QModelIndex &, int ) const;
    QVariant headerData ( int, Qt::Orientation, int ) const;
    bool setData ( const QModelIndex &, const QVariant &, int role = Qt::EditRole );
    QStringList mimeTypes () const;
    QMimeData* mimeData ( const QModelIndexList & ) const;

  Q_SIGNALS:
    void update ( const QModelIndex & );
    void agentChanged ( bool );

  public Q_SLOTS:
    void addItem ( const QModelIndex &parent = QModelIndex() );
    void removeItem ( const QModelIndex & );
    void moveRowTo ( int, int );

  public:
    UserAgentsTableModel ( QTableView * parent = 0 );
    void addItems ( const QList<UserAgent*> & );
    const QList<UserAgent*> agents() { return agentList; };
    int rowCount ( const QModelIndex & ) const;
    int columnCount ( const QModelIndex & ) const;
    QModelIndex index ( int, int, const QModelIndex & ) const;
    virtual ~UserAgentsTableModel();
};

#endif
