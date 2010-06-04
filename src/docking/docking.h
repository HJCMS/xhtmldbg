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
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QFontMetrics>
#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/**
* @class DockTreeWidget
* @brief Child QTreeWidget Class for Subclassing Docking
* @author Jürgen Heinemann (Undefined) http://www.hjcms.de
* @version 0.0.1
* @date 2010
*/
class DockTreeWidget : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    int minWidth;

  private Q_SLOTS:
    void resizeFirstColumn ( QTreeWidgetItem *, int );

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  public:
    DockTreeWidget ( QWidget * parent = 0 );
    virtual ~DockTreeWidget();
};

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
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_PROPERTY ( int columns READ columnCount WRITE setColumnCount )

  private:
    /**
    * @var DockingSplitter
    * this splitter is already add in constructer
    * and contains 1 TreeWidget by default
    */
    QSplitter* DockingSplitter;

    /**
    * @var DockingTreeWidgetTop
    * the default TreeWidget with index 0
    * this widget is already add in constructer
    */
    DockTreeWidget* DockingTreeWidgetTop;

    /**
    * @var columns
    * this variable have no effect and was add for compatible property flags
    * the default column count for TreeWidgets is 2
    */
    int columns;

  protected:
    /**
    * force reimplementation for setting header labels to indexed TreeWidget
    * @param labels List of Header Labels
    * @param index  The Index for used TreeWidget Default 0
    */
    virtual void setTreeHeaderLabels ( const QStringList &labels, int index = 0 ) = 0;

    /**
    * get the invisible rootitem for given indexed TreeWidget
    * @param index The Index for used TreeWidget Default 0
    */
    QTreeWidgetItem* rootItem ( int index = 0 );

    /**
    * QFontMetric from TreeWidget with given index
    * @param index The Index for used TreeWidget Default 0
    */
    const QFontMetrics fontMetric ( int index = 0 );

    /**
    * insert a new TreeWidget behind the top TreeWidget
    * the class will automatically append this TreeWidget from top
    */
    void addTreeWidget ( QTreeWidget * widget );

    /**
    * fetch TreeWidget by index
    * @param index The Index for used TreeWidget Default 0
    */
    QTreeWidget* widget ( int index = 0 );

  public Q_SLOTS:
    /**
    * remove all TreeWidget items with given index
    * @param index The Index for used TreeWidget Default 0
    */
    void clearContent ( int index = 0 );

    /**
    * resize all header sections to column contents with given index
    * @param index The Index for used TreeWidget Default 0
    */
    void resizeSections ( int index = 0 );

  public:
    /**
    * the current column count from indexed TreeWidget
    * @param index The Index for used TreeWidget Default 0
    */
    int columnCount ( int index = 0 );

    /**
    * set column count to indexed TreeWidget
    * @param count count
    * @param index The Index for used TreeWidget Default 0
    */
    void setColumnCount ( int count, int index = 0 );

    /**
    * resize the column with to indexed TreeWidget
    * @param column Column
    * @param width Width
    * @param index The Index for used TreeWidget Default 0
    */
    void setColumnWidth ( int column, int width, int index = 0 );

    /**
    * insert a new TopLevelItem to parent QTreeWidgetItem
    * @param parent Item
    * @param expand Expanded or Collapse
    */
    QTreeWidgetItem* addTopLevelItem ( QTreeWidgetItem * parent, bool expand = true );

    /**
    * insert a ChildItem to TopLevelItem
    * @param child Item
    * @param index The Index for used TreeWidget Default 0
    */
    void addTopLevelChildItem ( QTreeWidgetItem * child, int index = 0 );

    /**
    * find item with given text and indexed TreeWidget
    * this matches exactly
    * @param txt String to match
    * @param index The Index for used TreeWidget Default 0
    * @param column Default value is 0
    */
    bool itemExists ( const QString &txt, int index = 0, int column = 0 );

    explicit Docking ( QWidget * parent = 0 );
    virtual ~Docking();
};

#endif
