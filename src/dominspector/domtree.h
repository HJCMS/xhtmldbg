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

#ifndef DOMTREE_H
#define DOMTREE_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class DomTree : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    int minCellWidth;
    QAction* ac_swapHighlight;
    QAction* ac_showContent;
    QAction* ac_copyPredicates;
    QAction* ac_setHighlight;
    QAction* ac_setStyleSheet;
    QAction* ac_wcagLuminance;
    QTreeWidgetItem* createTopLevelItem ( const QString & );
    const QWebElement currentElement();
    void parseAttributes ( const QWebElement &, QTreeWidgetItem* );
    void parseElements ( const QWebElement &, QTreeWidgetItem* );
    QTreeWidgetItem* createChildItem ( const QString &, QTreeWidgetItem* );
    void openContentDialog ( const QString & );
    void openStyleSheetDialog ( const QWebElement &element );

  private Q_SLOTS:
    void highlightElement ( QTreeWidgetItem *, int );
    void itemSelected ( QTreeWidgetItem *, int );
    void itemHovered ( QTreeWidgetItem *, int );
    void editStyleClicked ();
    void changeHighlight ();
    void copyPredicate ();
    void visitContent ();
    void checkLuminance ();

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void itemHighlight ( const QWebElement & );
    void unselect ();
    void initStyleSheet ( const QWebElement & );
    void elementHovered ( const QWebElement & );
    void errorMessage ( const QString & );

  public Q_SLOTS:
    void setPrune();

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
