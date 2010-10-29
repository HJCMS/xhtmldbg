/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef JSMESSANGER_H
#define JSMESSANGER_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

class JSMessanger : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QListWidget* m_listWidget;
    const QIcon iconNotice;

  private Q_SLOTS:
    void sortAscending();
    void sortDescending();

  protected:
    virtual void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void itemsChanged();

  public Q_SLOTS:
    void insertMessage ( const QString & );
    void clearItems();

  public:
    JSMessanger ( QWidget * parent = 0 );
    ~JSMessanger();
};

#endif
