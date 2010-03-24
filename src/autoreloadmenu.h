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

#ifndef AUTORELOADMENU_H
#define AUTORELOADMENU_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMenu>

class AutoReloadMenu : public QMenu
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QAction* disableReload;
    QAction* ac15_seconds;
    QAction* ac30_seconds;
    QAction* ac45_seconds;
    QAction* ac60_seconds;

  private Q_SLOTS:
    void setTimerDisable();
    void set15_seconds();
    void set30_seconds();
    void set45_seconds();
    void set60_seconds();

  Q_SIGNALS:
    void reloadInterval ( int msek );

  public:
    AutoReloadMenu ( QMenu * parent = 0 );
    ~AutoReloadMenu();
};

#endif
