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
#include <QtGui/QFontMetrics>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>
#include <QtGui/QTreeWidgetItemIterator>

DomTree::DomTree ( QWidget * parent )
    : QTreeWidget ( parent )
    , minCellWidth ( 50 )
{
  setObjectName ( QLatin1String ( "domtree" ) );
  QStringList labels;
  labels << QLatin1String ( "STag" ) << QLatin1String ( "AttName " ) << QLatin1String ( "AttValue" );
  setHeaderLabels ( labels );
  setSortingEnabled ( false );
  header()->setResizeMode ( QHeaderView::ResizeToContents );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  setFrameStyle ( QFrame::Box );

  connect ( this, SIGNAL ( itemClicked ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemSelected ( QTreeWidgetItem *, int ) ) );
}

/**
* Oberste Baumeinträge erstellen. Wenn im Text html oder body
* enthalten sind erstelle einen aufgeklappten Eintrag.
* @note Diese Einträge enthalten kein struct zum WebElement.
*/
QTreeWidgetItem* DomTree::createTopLevelItem ( const QString &name )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( invisibleRootItem() );
  item->setExpanded ( name.contains ( QRegExp ( "(html|body)" ) ) );
  item->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );
  item->setData ( 0, Qt::DisplayRole, name );
  addTopLevelItem ( item );
  return item;
}

/**
* In dem übergeben WebElement nach Prädikaten suchen.
* Es werden je nach vorkommen 3 Zustände dargestellt.
* @li AttDef (Attribute Definition) - Blau hervorgehoben
* @li EmptyElemTag (Leeres Element) - Hellgrau dargestellt
* @li CDSect      (CDATA Abschnitt) - Hellgrau dargestellt
* @note Die hier erstellten Einträge erben das struct vom Elterneintrag.
*/
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
      QString attr = element.attribute ( name );
      item->setText ( 2, attr );
      item->setTextAlignment ( 2, Qt::AlignLeft );
      int cw = ( fontMetrics().width ( attr ) + 10 );
      if ( cw > minCellWidth )
        minCellWidth = cw;

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

/**
* Einen Eintrag für das WebElement erstellen und nach Kindern suchen.
* @note In dieser Methode wird das struct @ref TreeItem gesetzt.
*/
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

/**
* Standard Methode für das erstellen eines Eintrages.
* Es wird das Objekt erstellt und der Titel gesetzt.
*/
QTreeWidgetItem* DomTree::createChildItem ( const QString &name, QTreeWidgetItem* parent )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setText ( 0, name );
  return item;
}

/**
* Bei einer Auswahl mit der Maus wird der Eintrag hier
* aufbereitet in dem das struct eingelesen wird und mit
* dem WebElement ein signal @ref itemClicked angestoßen.
*/
void DomTree::itemSelected ( QTreeWidgetItem * item, int column )
{
  Q_UNUSED ( column )

  TreeItem ti = item->data ( 0, Qt::UserRole ).value<TreeItem>();
  emit itemClicked ( ti.element );
}

/**
* Einstiegs Methode für das befüllen des Baums.
* Setze den Kopfeintrag und rufe @ref parseAttributes
* und @ref parseElements auf danach korrigiere die
* zweite Spaltenbreite mit dem @ref minCellWidth Wert.
*/
void DomTree::setDomTree ( const QWebElement &we )
{
  clear();
  minCellWidth = 50;

  QTreeWidgetItem* item = createTopLevelItem ( we.localName() );
  item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  item->setToolTip ( 0, we.namespaceUri() );
  parseAttributes ( we, item );
  parseElements ( we, item );

  setColumnWidth ( 2, minCellWidth );
}

/**
* Wenn im Browser ein klick auf ein Webelement gemacht wird.
* Suche bei den Einträgen - In dem der Baum mit einem Iterator
* durchlaufen wird. Vergleiche das übergebene Element mit dem
* in @ref TreeItem abgelegten Element. Bei Erfolg setze die
* Position auf den Eintrag und klappe diesen Baum auf.
*/
bool DomTree::findItem ( const QWebElement &element )
{
  bool found = false;
  if ( element.localName().isEmpty() || topLevelItemCount() < 1 )
    return found;

  QTreeWidgetItemIterator it ( invisibleRootItem(), QTreeWidgetItemIterator::Enabled );
  while ( *it )
  {
    if ( ( *it )->data ( 0, Qt::UserRole ).value<TreeItem>().element == element )
    {
      ( *it )->setExpanded ( true );
      setCurrentItem ( ( *it ), 0 );
      scrollToItem ( ( *it ), QAbstractItemView::PositionAtTop );
      found = true;
      break;
    }
    ++it;
  }
  return found;
}

/**
* Den Baum wieder zusammen ziehen, Elemente wie
* html, head und body wieder sichtbar machen.
*/
void DomTree::setPrune()
{
  collapseAll();
  // expand html head body
  for ( int t = 0; t < 3; t++ )
  {
    if ( invisibleRootItem()->child ( t ) )
      invisibleRootItem()->child ( t )->setExpanded ( true );
  }
}

/**
* Damit die farbliche Hervorhebung im Browser wieder
* entfernt wird setze den Zeiger auf eines der Elemente.
* Die kein struct @ref TreeItem enthalten.
*/
void DomTree::setUnselect()
{
  TreeItem ti = invisibleRootItem()->data ( 0, Qt::UserRole ).value<TreeItem>();
  emit itemClicked ( ti.element );
}

DomTree::~DomTree()
{}
