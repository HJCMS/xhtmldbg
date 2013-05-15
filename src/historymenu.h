/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2013
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

#ifndef HISTORYMENU_H
#define HISTORYMENU_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSignalMapper>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class HistoryItem;

class HistoryMenu : public QMenu
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    const QIcon defaultIcon;
    QSignalMapper* m_mapper;

  private Q_SLOTS:
    void clicked ( const QString & );

  Q_SIGNALS:
    void openBookmark ( const QUrl & );

  public Q_SLOTS:
    void updateHistoryItems ( const QList<HistoryItem> & );

  public:
    HistoryMenu ( QMenu * parent = 0 );
    ~HistoryMenu();
};

#endif
