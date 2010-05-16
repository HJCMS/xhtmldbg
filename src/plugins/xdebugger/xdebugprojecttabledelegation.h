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

#ifndef XDEBUGPROJECTTABLEDELEGATION_H
#define XDEBUGPROJECTTABLEDELEGATION_H

/* QtCore */
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QItemDelegate>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

class XDebugProjectTableModel;

class XDebugProjectTableDelegation : private QItemDelegate
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

    friend class XDebugProjectTableModel;

  private:
    QTableView* m_tableView;
    QWidget* createEditor ( QWidget* parent, const QStyleOptionViewItem &option,
                            const QModelIndex &index ) const;

    void setEditorData ( QWidget *editor,
                         const QModelIndex &index ) const;

    void setModelData ( QWidget* editor, QAbstractItemModel* model,
                        const QModelIndex &index ) const;

    void updateEditorGeometry ( QWidget* editor, const QStyleOptionViewItem &option,
                                const QModelIndex &index ) const;

  public:
    XDebugProjectTableDelegation ( QTableView * parent );
    virtual ~XDebugProjectTableDelegation();
};

#endif
