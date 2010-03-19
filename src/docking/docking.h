/**
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#ifndef DOCKING_H
#define DOCKING_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QFontMetrics>
#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/**
* @class Docking
* @brief My basic QDockWidget/QTreeWidget Class for Subclassing TreeWidgets
* @author Jürgen Heinemann (Undefined) http://www.hjcms.de
* @version 0.2.1
* @date 2008-2010
*/

class Docking : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( int columns READ columnCount WRITE setColumnCount )

  private:
    /**
    * DockingSplitter
    */
    QSplitter* DockingSplitter;

    /**
    * DockingTreeWidgetTop with index 0
    */
    QTreeWidget* DockingTreeWidgetTop;

    /**
    * Current Column Count
    * Default: 2
    */
    int columns;

  protected:
    /**
    * Set Item Labels
    * @param labels Header Labels
    * @param index DockingTreeWidget Index
    */
    virtual void setTreeHeaderLabels ( const QStringList &labels, int index = 0 ) = 0;

    /**
    * Invisible RootItem
    * @param index DockingTreeWidget Index
    */
    QTreeWidgetItem* rootItem ( int index = 0 );

    /**
    * Font Metric from ListWidget
    * @param index DockingTreeWidget Index
    */
    const QFontMetrics fontMetric ( int index = 0 );

    /**
    * insert a second Docking TreeWidget
    */
    void addTreeWidget ( QTreeWidget * widget );

    /**
    * get DockingTreeWidget with index
    * @param index DockingTreeWidget Index
    */
    QTreeWidget* widget ( int index = 0 );

  public Q_SLOTS:
    /**
    * Remove all ListWidget Items
    * @param index DockingTreeWidget Index
    */
    void clearContent ( int index = 0 );

    /**
    * Resize all Header Sections to Contents
    * @param index DockingTreeWidget Index
    */
    void resizeSections ( int index = 0 );

  public:
    /**
    * Current Column Count
    */
    int columnCount ( int index = 0 );

    /**
    * set Column Count
    * @param count count
    * @param index DockingTreeWidget Index
    */
    void setColumnCount ( int count, int index = 0 );

    /**
    * resize the Column With
    * @param column Column
    * @param width Width
    * @param index DockingTreeWidget Index
    */
    void setColumnWidth ( int column, int width, int index = 0 );

    /**
    * insert a TopLevelItem
    * @param parent Item
    * @param expand Expanded or Collapse
    */
    QTreeWidgetItem* addTopLevelItem ( QTreeWidgetItem * parent, bool expand = true );

    /**
    * insert a ChildItem to TopLevelItem
    * @param child Item
    * @param index DockingTreeWidget Index
    */
    void addTopLevelChildItem ( QTreeWidgetItem * child, int index = 0 );

    /**
    * find item with given text
    * this matches exactly
    * @param txt String to match
    * @param index DockingTreeWidget Index
    */
    bool itemExists ( const QString &txt, int index = 0 );

    explicit Docking ( QWidget * parent = 0 );
    virtual ~Docking();
};

#endif
