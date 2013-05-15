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

#include "rsstreeview.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QSizePolicy>
#include <QtGui/QTreeWidgetItemIterator>

/* QtXml */
#include <QtXml/QDomNodeList>
#include <QtXml/QDomElement>
#include <QtXml/QDomNamedNodeMap>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

RSSTreeView::RSSTreeView ( QWidget * parent )
    : QTreeWidget ( parent )
{
  setObjectName ( QLatin1String ( "rsstreeview" ) );
  QStringList labels;
  labels << i18n ( "Element" ) << i18n ( "Predicate" ) << i18n ( "Value" );
  setHeaderLabels ( labels );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  setFrameStyle ( QFrame::Box );
  setAutoScroll ( true );
  setWordWrap ( false );
  setSortingEnabled ( false );

  // Header
  QHeaderView* headerView = header();
  headerView->setResizeMode ( QHeaderView::Interactive );
  headerView->setCascadingSectionResizes ( true );
  headerView->setSortIndicatorShown ( false );

  // Beim ein/ausklappen die Spaltenbreite neu setzen
  connect ( this, SIGNAL ( itemExpanded ( QTreeWidgetItem * ) ),
            this, SLOT ( onExpandCollapseResize ( QTreeWidgetItem * ) ) );

  connect ( this, SIGNAL ( itemCollapsed ( QTreeWidgetItem * ) ),
            this, SLOT ( onExpandCollapseResize ( QTreeWidgetItem * ) ) );
}

/**
* Reagiert nur bei einem einzelnen Aufklappen und nicht wenn der Baum
* schon komplett mit @ref QTreeWidget::expandAll aufgeklappt war!
*/
void RSSTreeView::onExpandCollapseResize ( QTreeWidgetItem *item )
{
  for ( int c = 0; c < item->columnCount(); c++ )
  {
    resizeColumnToContents ( c );
  }
}

/**
* Oberste Baumeinträge erstellen.
*/
QTreeWidgetItem* RSSTreeView::createTopLevelItem ( const QString &name )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( invisibleRootItem() );
  item->setExpanded ( true );
  item->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );
  item->setData ( 0, Qt::DisplayRole, name );
  addTopLevelItem ( item );
  return item;
}

/**
* Prädikate für das Element einfügen
*/
QTreeWidgetItem* RSSTreeView::createAttributeChildItem ( const QDomNode &node, QTreeWidgetItem *parent ) const
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setText ( 0, QString::fromUtf8 ( "AttDef" ) );
  item->setForeground ( 0, Qt::lightGray );
  item->setText ( 1, node.nodeName() );
  item->setForeground ( 1, Qt::blue );
  item->setTextAlignment ( 1, Qt::AlignRight );
  item->setText ( 2, node.nodeValue() );
  item->setTextAlignment ( 2, Qt::AlignLeft );
  return item;
}

/**
* Nach Prädikaten im Element suchen
*/
void RSSTreeView::parseAttributes ( const QDomElement &element, QTreeWidgetItem * parent ) const
{
  if ( element.hasAttributes() )
  {
    QList<QTreeWidgetItem*> attributes;
    QDomNamedNodeMap nodeMap = element.attributes();
    for ( int i = 0; i < nodeMap.count(); i++ )
    {
      attributes << createAttributeChildItem ( nodeMap.item ( i ), parent );
    }
    parent->addChildren ( attributes );
  }
}

/**
* Kinderknoten einfügen
*/
QTreeWidgetItem* RSSTreeView::createElementChildItem ( const QDomNode &node, QTreeWidgetItem* parent ) const
{
  QString value;
  QString name = node.nodeName();

  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setText ( 0, name );
  if ( name.contains ( QRegExp ( "^#\\w+" ) ) )
    item->setForeground ( 0, Qt::lightGray );

  if ( name.contains ( "#text" ) )
  {
    value = node.nodeValue();
    if ( ! value.isEmpty() )
      item->setText ( 2, value.trimmed() );
  }
  else if ( name.contains ( "#cdata-section" ) )
  {
    value = node.nodeValue();
    if ( ! value.isEmpty() )
      item->setText ( 2, value.trimmed() );
  }
  else
    item->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );

  if ( node.isElement() )
    parseAttributes ( node.toElement(), item );

  return item;
}

/**
* Nach Kinderknoten suchen
*/
void RSSTreeView::parseElements ( const QDomElement &element, QTreeWidgetItem* parent ) const
{
  if ( element.hasChildNodes () )
  {
    QDomNode child = element.firstChild();
    while ( ! child.isNull() )
    {
      QDomElement e = child.toElement();
      QTreeWidgetItem* item = createElementChildItem ( child, parent );
      parseAttributes ( e, item );
      parent->addChild ( item );
      parseElements ( e, item );
      child = child.nextSibling();
    }
  }
}

/**
* Initialisiere die Baumeinträge
*/
void RSSTreeView::createTreeView ( const QDomDocument &dom )
{
  clear();

  QDomElement rootNode = dom.documentElement();
  QTreeWidgetItem* item = createTopLevelItem ( rootNode.tagName() );
  item->setIcon ( 0, KIcon ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  parseAttributes ( rootNode, item );
  parseElements ( rootNode, item );
  onExpandCollapseResize ( item );
}

RSSTreeView::~RSSTreeView()
{}
