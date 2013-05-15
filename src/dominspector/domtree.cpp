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

#include "domtree.h"
#include "contentdialog.h"
#include "stylesheeted.h"
#include "luminancedialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QPoint>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QFrame>
#include <QtGui/QFontMetrics>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>
#include <QtGui/QTreeWidgetItemIterator>

/* QtWebKit */
#ifndef MAINTAINER_REPOSITORY
# include <QtWebKit/QtWebKit>
#else
# include <QtWebKit/qwebkitglobal.h>
# include <QtWebKit/qwebkitversion.h>
#endif

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

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
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setFrameStyle ( QFrame::Box );
  setMouseTracking ( true );

  // Actions siehe \ref contextMenuEvent
  ac_swapHighlight = new QAction ( KIcon ( "edit-clear-list" ), i18n ( "unhighlighted" ), this );
  ac_swapHighlight->setStatusTip ( i18n ( "unhighlighted current document content" ) );

  ac_setStyleSheet = new QAction ( KIcon ( "preferences-desktop-color" ), i18n ( "StyleSheet" ), this );
  ac_setStyleSheet->setEnabled ( false );
  ac_setStyleSheet->setStatusTip ( i18n ( "modify the element stylesheet" ) );

  ac_showContent = new QAction ( KIcon ( "view-choose" ), i18n ( "Content" ), this );
  ac_showContent->setEnabled ( false );
  ac_showContent->setStatusTip ( i18n ( "show current element content" ) );

  ac_copyPredicates = new QAction ( KIcon ( "edit-copy" ), i18n ( "Copy Predicate" ), this );
  ac_copyPredicates->setEnabled ( false );
  ac_copyPredicates->setStatusTip ( i18n ( "copy predicate to clipboard" ) );

  ac_setHighlight = new QAction ( KIcon ( "view-statistics" ), i18n ( "Highlight" ), this );
  ac_setHighlight->setStatusTip ( i18n ( "show current element information" ) );

  ac_wcagLuminance = new QAction ( KIcon ( "validators" ), i18n ( "Luminance && Contrast" ), this );
  ac_wcagLuminance->setStatusTip ( i18n ( "Web Content Accessibility Guidelines" ) );

  // Signals
  connect ( ac_swapHighlight, SIGNAL ( triggered () ),
            this, SIGNAL ( unselect() ) );

  connect ( ac_setStyleSheet, SIGNAL ( triggered() ),
            this, SLOT ( editStyleClicked () ) );

  connect ( ac_showContent, SIGNAL ( triggered () ),
            this, SLOT ( visitContent() ) );

  connect ( ac_copyPredicates, SIGNAL ( triggered () ),
            this, SLOT ( copyPredicate() ) );

  connect ( ac_setHighlight, SIGNAL ( triggered () ),
            this, SLOT ( changeHighlight() ) );

  connect ( ac_wcagLuminance, SIGNAL ( triggered () ),
            this, SLOT ( checkLuminance() ) );

  connect ( this, SIGNAL ( itemClicked ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemSelected ( QTreeWidgetItem *, int ) ) );

  connect ( this, SIGNAL ( itemEntered ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemHovered ( QTreeWidgetItem *, int ) ) );
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

/** Zeiger auf das Aktuell ausgewählte Element */
const QWebElement DomTree::currentElement()
{
  return currentItem()->data ( 0, Qt::UserRole ).value<TreeItem>().element;
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
      if ( ! name.contains ( QRegExp ( "(html|head|body)" ) ) )
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

/** Element CDATA Inhalt Anzeigen Dialog öffnen */
void DomTree::openContentDialog ( const QString &content )
{
  if ( content.isEmpty() )
    return;

  ContentDialog dialog ( content, this );
  dialog.exec();
}

/** Öffnet den StyleSheet Dialog zum Manipulieren des WebElements! */
void DomTree::openStyleSheetDialog ( const QWebElement &element )
{
  StyleSheeted* dialog = new StyleSheeted ( element, this );
  dialog->exec();
  delete dialog;
}

/**
* Bei einem Doppelklick der Maus wird der Eintrag hier
* aufbereitet in dem das struct eingelesen wird und mit
* dem WebElement ein signal @ref itemHighlight angestoßen.
* @note Der Grund für diese Änderung ist, das wenn ein Item
*       ausgwählt wird erst eine Kopie der Prädikate für den Kopieren
*       Menüeintrag erstellt werden muss. Ansnonsten würde das "background"
*       Prädikat überschrieben und einen falschen Kopiereintrag erzeugen!
*/
void DomTree::highlightElement ( QTreeWidgetItem * item, int column )
{
  Q_UNUSED ( column )
  TreeItem ti = item->data ( 0, Qt::UserRole ).value<TreeItem>();
  emit itemHighlight ( ti.element );
}

/**
* Bei einer Auswahl mit der Maus wird der Eintrag nach
* Prädikaten durchsucht und die Vorbereitung für die
* Kopieren Aktionen erstellt!
*/
void DomTree::itemSelected ( QTreeWidgetItem * item, int column )
{
  if ( item->text ( 0 ) == QString::fromUtf8 ( "AttDef" ) )
    ac_copyPredicates->setEnabled ( true );
  else if ( item->text ( column ) == QString::fromUtf8 ( "AttDef" ) )
    ac_copyPredicates->setEnabled ( true );
  else
    ac_copyPredicates->setEnabled ( false );

  TreeItem ti = item->data ( 0, Qt::UserRole ).value<TreeItem>();
  if ( ! ti.element.isNull() )
  {
    ac_showContent->setEnabled ( ( ti.element.toPlainText().isEmpty() ? false : true ) );
    emit initStyleSheet ( ti.element );
  }
}

/**
* Slot zum Suchen des Aktuell ausgewählten Elements
* und rufe dann @ref openColorElementDialog auf.
*/
void DomTree::editStyleClicked ()
{
  if ( ! currentElement().isNull() )
    openStyleSheetDialog ( currentElement() );
}

/**
* Beim Überfahren mit der Maus das Element weiter geben!
*/
void DomTree::itemHovered ( QTreeWidgetItem * item, int column )
{
  if ( column != 0 )
    return;

  TreeItem ti = item->data ( column, Qt::UserRole ).value<TreeItem>();
  if ( ti.element.isNull() )
    return;

  /* Wenn eine Sichtbare Geometrie vorhanden ist dann den
  * StyleSheet Editor Aktivieren. */
  ac_setStyleSheet->setEnabled ( ti.element.geometry().isValid() );

  emit elementHovered ( ti.element );
}

/**
* Manuelle Auswahl zum Hervorheben von Elementen im Browser
*/
void DomTree::changeHighlight()
{
  QWebElement element = currentElement();
  if ( ! element.isNull() )
  {
    emit initStyleSheet ( element );
    emit itemHighlight ( element );
  }
}

/** Aktion Prädikate in das Clipboard einfügen! */
void DomTree::copyPredicate ()
{
  QTreeWidgetItem* item = currentItem();
  TreeItem ti = item->data ( 0, Qt::UserRole ).value<TreeItem>();
  if ( ! ti.element.hasAttributes() )
    return;

  QString txt = QString::fromUtf8 ( "%1=\"%2\"" ).arg ( item->text ( 1 ), item->text ( 2 ) );
  qApp->clipboard()->setText ( txt );
}

/**
* Wenn das Element ein CDATA Section enthält diese Anzeigen!
*/
void DomTree::visitContent()
{
  QWebElement element = currentElement();
  if ( ! element.isNull() )
    openContentDialog ( element.toInnerXml() );
}

/**
* TODO http://www.w3.org/TR/WCAG20/#relativeluminancedef
*/
void DomTree::checkLuminance()
{
  QWebElement element = currentElement();
  if ( ! element.isNull() )
  {
    LuminanceDialog dialog ( element, this );
    if ( dialog.isValid() )
      dialog.exec();
  }
}

/** Maus Menü Aktionen verarbeiten */
void DomTree::contextMenuEvent ( QContextMenuEvent * event )
{
  QMenu* menu = new QMenu ( this );
  menu->addAction ( ac_setHighlight );
  menu->addAction ( ac_swapHighlight );
  menu->addAction ( ac_copyPredicates );
  menu->addAction ( ac_showContent );
  menu->addAction ( ac_setStyleSheet );
  menu->addAction ( ac_wcagLuminance );
  menu->exec ( event->globalPos() );
  delete menu;
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

  if ( we.findFirst ( "BODY" ).toOuterXml().isEmpty() )
  {
    QString message ( "QWebFrame::documentElement() Rendering BUG" );
    emit errorMessage ( QString::fromUtf8 ( "(XHTMLDBG) %1 WebKit(%2 %3) Qt(%4)" )
                        .arg ( message, qWebKitVersion(), QTWEBKIT_VERSION_STR, QT_VERSION_STR ) );
    return;
  }

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

DomTree::~DomTree()
{}
