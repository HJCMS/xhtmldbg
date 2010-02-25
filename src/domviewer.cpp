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

#include "domviewer.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

/* QtWebKit */
#include <QtXml/QDomDocument>

DomViewer::DomViewer ( QWidget * parent )
    : QTreeWidget ( parent )
{
  setObjectName ( QLatin1String ( "domviewer" ) );
  labels << trUtf8 ( "STag" ) << trUtf8 ( "AttName" ) << trUtf8 ( "AttValue" );
  setHeaderLabels ( labels );
  setSortingEnabled ( false );
  header()->setResizeMode ( QHeaderView::ResizeToContents );
  setFrameStyle ( QFrame::Box );
}

QTreeWidgetItem* DomViewer::createChildItem ( const QString &name, QTreeWidgetItem* parent )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setExpanded ( name.contains ( QRegExp ( "(html|head|body)" ) ) );
  item->setData ( 0, Qt::UserRole, name );
  item->setText ( 0, name );
  return item;
}

void DomViewer::parseAttributes ( const QWebElement &element, QTreeWidgetItem* parent )
{
  if ( element.hasAttributes() )
  {
    QList<QTreeWidgetItem*> attributes;
    foreach ( QString name, element.attributeNames() )
    {
      QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
      item->setText ( 0, QString::fromUtf8 ( "AttDef" ) );
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
    item->setForeground ( 0, Qt::lightGray );
  }
  else if ( element.firstChild().isNull() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setText ( 0, QString::fromUtf8 ( "CDSect" ) );
    item->setForeground ( 0, Qt::lightGray );
  }
}

void DomViewer::parseElements ( const QWebElement &element, QTreeWidgetItem* parent )
{
  if ( ! parent )
    return;

  QWebElement child = element.firstChild();
  while ( ! child.isNull() )
  {
    QString name = child.localName();
    QTreeWidgetItem* item = createChildItem ( name, parent );
    parseAttributes ( child, item );
    parent->addChild ( item );
    parseElements ( child, item );
    child = child.nextSibling();
  }
}

void DomViewer::setDomTree ( const QWebElement &we )
{
  clear();
  QTreeWidgetItem* item = createChildItem ( we.localName(), invisibleRootItem() );
  item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  item->setToolTip ( 0, we.namespaceUri() );
  parseAttributes ( we, item );
  addTopLevelItem ( item );
  parseElements ( we, item );
}

DomViewer::~DomViewer()
{
}
