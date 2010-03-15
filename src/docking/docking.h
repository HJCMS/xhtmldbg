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

class Docking : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( int columns READ columnCount WRITE setColumnCount )

  private:
    QTreeWidget* m_treeWidget;

  protected:
    int columns;
    virtual void setTreeHeaderLabels ( const QStringList & ) = 0;
    QTreeWidgetItem* rootItem() const;
    const QFontMetrics fontMetric();

  public Q_SLOTS:
    void clearContent();

  public:
    explicit Docking ( QWidget * parent = 0 );
    int columnCount();
    void setColumnCount ( int );
    void setColumnWidth ( int column, int width );
    QTreeWidgetItem* addTopLevelItem ( QTreeWidgetItem *, bool expand = true );
    void addTopLevelChildItem ( QTreeWidgetItem * child );
    bool itemExists ( const QString & );
    virtual ~Docking();
};

#endif
