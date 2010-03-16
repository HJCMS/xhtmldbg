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

#ifndef DOMTREE_H
#define DOMTREE_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class DomTree : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    int minCellWidth;
    QTreeWidgetItem* createTopLevelItem ( const QString & );
    void parseAttributes ( const QWebElement &, QTreeWidgetItem* );
    void parseElements ( const QWebElement &, QTreeWidgetItem* );
    QTreeWidgetItem* createChildItem ( const QString &, QTreeWidgetItem* );

  private Q_SLOTS:
    void itemSelected ( QTreeWidgetItem *, int );

  Q_SIGNALS:
    void itemClicked ( const QWebElement & );

  public:
    struct TreeItem
    {
      QString name;
      QWebElement element;
    };
    DomTree ( QWidget * parent = 0 );
    void setDomTree ( const QWebElement & );
    bool findItem ( const QWebElement & );
    ~DomTree();
};

Q_DECLARE_METATYPE ( DomTree::TreeItem )

#endif
