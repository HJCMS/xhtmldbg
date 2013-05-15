/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#ifndef RSSTREEVIEW_H
#define RSSTREEVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/* QtXml */
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

class RSSTreeView : public QTreeWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QTreeWidgetItem* createTopLevelItem ( const QString & );
    QTreeWidgetItem* createAttributeChildItem ( const QDomNode &, QTreeWidgetItem * ) const;
    void parseAttributes ( const QDomElement &, QTreeWidgetItem * ) const;
    QTreeWidgetItem* createElementChildItem ( const QDomNode &, QTreeWidgetItem * ) const;
    void parseElements ( const QDomElement &, QTreeWidgetItem * ) const;

  private Q_SLOTS:
    void onExpandCollapseResize ( QTreeWidgetItem * );

  public:
    RSSTreeView ( QWidget * parent = 0 );
    void createTreeView ( const QDomDocument & );
    ~RSSTreeView();
};

#endif
