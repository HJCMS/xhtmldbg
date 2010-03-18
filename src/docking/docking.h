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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
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
    QTreeWidget* m_treeWidget;

  protected:
    /**
    * Current Column Count
    * Default: 2
    */
    int columns;

    /**
    * Set Item Labels
    * @param
    */
    virtual void setTreeHeaderLabels ( const QStringList & ) = 0;

    /**
    * Invisible RootItem
    */
    QTreeWidgetItem* rootItem() const;

    /**
    * Font Metric from ListWidget
    */
    const QFontMetrics fontMetric();

  public Q_SLOTS:
    /**
    * Remove all ListWidget Items
    */
    void clearContent();

  public:
    /**
    * Current Column Count
    */
    int columnCount();

    /**
    * set Column Count
    * @param count count
    */
    void setColumnCount ( int count );

    /**
    * resize the Column With
    * @param column Column
    * @param width  Width
    */
    void setColumnWidth ( int column, int width );

    /**
    * insert a TopLevelItem
    * @param parent Item
    * @param expand Expanded or Collapse
    */
    QTreeWidgetItem* addTopLevelItem ( QTreeWidgetItem * parent, bool expand = true );

    /**
    * insert a ChildItem to TopLevelItem
    * @param child Item
    */
    void addTopLevelChildItem ( QTreeWidgetItem * child );

    /**
    * find item with given text
    * this matches exactly
    * @param txt String to match
    */
    bool itemExists ( const QString &txt );

    explicit Docking ( QWidget * parent = 0 );
    virtual ~Docking();
};

#endif
