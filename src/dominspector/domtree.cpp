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

#include "domtree.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>

DomTree::DomTree ( QWidget * parent )
    : QTreeWidget ( parent )
{
  setObjectName ( QLatin1String ( "domtree" ) );
  QStringList labels;
  labels << trUtf8 ( "STag" ) << trUtf8 ( "AttName" ) << trUtf8 ( "AttValue" );
  setHeaderLabels ( labels );
  setSortingEnabled ( false );
  header()->setResizeMode ( QHeaderView::ResizeToContents );
  setFrameStyle ( QFrame::Box );

  connect ( this, SIGNAL ( itemClicked ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemSelected ( QTreeWidgetItem *, int ) ) );
}

QTreeWidgetItem* DomTree::createTopLevelItem ( const QString &name )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( invisibleRootItem() );
  item->setExpanded ( name.contains ( QRegExp ( "(html|body)" ) ) );
  item->setData ( 0, Qt::DisplayRole, name );
  addTopLevelItem ( item );
  return item;
}

void DomTree::parseAttributes ( const QWebElement &element, QTreeWidgetItem* parent )
{
  if ( element.hasAttributes() )
  {
    QList<QTreeWidgetItem*> attributes;
    foreach ( QString name, element.attributeNames() )
    {
      QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
      item->setText ( 0, QString::fromUtf8 ( "AttDef" ) );
      item->setData ( 0, Qt::UserRole, parent->data ( 0, Qt::UserRole ) );
      item->setForeground ( 0, Qt::lightGray );
      item->setText ( 1, name );
      item->setForeground ( 1, Qt::blue );
      item->setTextAlignment ( 1, Qt::AlignRight );
      item->setText ( 2, element.attribute ( name ) );
      item->setTextAlignment ( 2, Qt::AlignLeft );
      attributes << item;
    }
    parent->addChildren ( attributes );
    attributes.clear();
  }
  else if ( element.toPlainText().isEmpty() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setText ( 0, QString::fromUtf8 ( "EmptyElemTag" ) );
    item->setData ( 0, Qt::UserRole, parent->data ( 0, Qt::UserRole ) );
    item->setForeground ( 0, Qt::lightGray );
  }
  else if ( element.firstChild().isNull() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setText ( 0, QString::fromUtf8 ( "CDSect" ) );
    item->setData ( 0, Qt::UserRole, parent->data ( 0, Qt::UserRole ) );
    item->setForeground ( 0, Qt::lightGray );
  }
}

void DomTree::parseElements ( const QWebElement &element, QTreeWidgetItem* parent )
{
  if ( ! parent )
    return;

  QWebElement child = element.firstChild();
  while ( ! child.isNull() )
  {
    QString name = child.localName();
    QTreeWidgetItem* item = createChildItem ( name, parent );

    QVariant itemVars;
    TreeItem dataItem;
    dataItem.name = name;
    dataItem.element = child;
    itemVars.setValue ( dataItem );

    item->setData ( 0, Qt::UserRole, itemVars );

    parseAttributes ( child, item );
    parent->addChild ( item );
    parseElements ( child, item );
    child = child.nextSibling();
  }
}

QTreeWidgetItem* DomTree::createChildItem ( const QString &name, QTreeWidgetItem* parent )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setExpanded ( name.contains ( QRegExp ( "(html|body)" ) ) );
  item->setData ( 0, Qt::DisplayRole, name );
  return item;
}

void DomTree::itemSelected ( QTreeWidgetItem * item, int column )
{
  Q_UNUSED ( column )

  TreeItem ti = item->data ( 0, Qt::UserRole ).value<TreeItem>();
  emit itemClicked ( ti.element );
}

void DomTree::setDomTree ( const QWebElement &we )
{
  clear();

  QTreeWidgetItem* item = createTopLevelItem ( we.localName() );
  item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  item->setToolTip ( 0, we.namespaceUri() );
  parseAttributes ( we, item );
  parseElements ( we, item );
}

DomTree::~DomTree()
{}
