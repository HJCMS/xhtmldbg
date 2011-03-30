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

#ifndef FORMBOOKMARKS_H
#define FORMBOOKMARKS_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

/* FormManager */
#include "formbookmarkitem.h"

class FormBookmarks : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QTreeWidgetItem* m_rootItem;
    QTreeWidgetItem* addProjectItem ( const QString &project );
    QTreeWidgetItem* findTopLevelItem ( const QString &project );

  private Q_SLOTS:
    void itemClicked ( const QModelIndex & );
    void itemDoubleClicked ( const QModelIndex & );

  Q_SIGNALS:
    void bookmarkClicked ( FormBookmarkItem * item, int column );
    void bookmarkDoubleClicked ( FormBookmarkItem * item, int column );

  public Q_SLOTS:
    void addItem ( const QString &project, FormBookmarkItem * item );

  public:
    FormBookmarks ( QWidget * parent = 0 );
    virtual ~FormBookmarks();
};

#endif
