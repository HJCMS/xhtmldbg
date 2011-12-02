/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#ifndef DOCKTREEWIDGET_H
#define DOCKTREEWIDGET_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/**
* @class DockTreeWidget
* @brief Child QTreeWidget for Docking
* @author Jürgen Heinemann (Undefined) http://www.hjcms.de
* @version 0.0.2
* @date 2008-2010
*/
class DockTreeWidget : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    int minWidth;

  private Q_SLOTS:
    void resizeColumnByItem ( QTreeWidgetItem *, int );
    void onExpandCollapseResize ( QTreeWidgetItem * );

  protected:
    virtual void contextMenuEvent ( QContextMenuEvent * );

  public Q_SLOTS:
    Q_SCRIPTABLE void restore ();

  public:
    DockTreeWidget ( QWidget * parent = 0 );
    ~DockTreeWidget();
};

#endif
