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

#ifndef DOMTOOLBAR_H
#define DOMTOOLBAR_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

class DomToolBar : public QToolBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QAction* acHighlight;

  Q_SIGNALS:
    void prune();
    void expand();
    void unselect();

  public:
    DomToolBar ( QWidget * patent = 0 );
    bool onHoverEnabled();
    ~DomToolBar();
};

#endif
