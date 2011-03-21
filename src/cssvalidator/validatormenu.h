/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2011
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

#ifndef VALIDATORMENU_H
#define VALIDATORMENU_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class ValidatorMenu : public QMenu
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QAction* aCheck;
    QAction* aDropout;
    QAction* aAscending;
    QAction* aDecending;
    QAction* aConfig;
    QAction* aClearItem;

  Q_SIGNALS:
    void check();
    void dropout();
    void ascending();
    void descending();
    void configure();
    void clearlist();

  public Q_SLOTS:
    void enableCheckUrlAction ( bool b = false );
    void enableKillProcessAction ( bool b = false );

  public:
    ValidatorMenu ( QWidget * parent = 0 );
    ~ValidatorMenu ();
};

#endif
