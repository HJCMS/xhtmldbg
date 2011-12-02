/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "docktreewidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QSizePolicy>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

DockTreeWidget::DockTreeWidget ( QWidget * parent )
    : QTreeWidget ( parent )
    , minWidth ( 50 )
{
  setObjectName ( "dockingtreewidget" );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  setSortingEnabled ( true );
  setAutoScroll ( true );
  setWordWrap ( false );
  setFrameStyle ( QFrame::Box );

  // Header
  QHeaderView* headerView = header();
  headerView->setResizeMode ( QHeaderView::Interactive );
  headerView->setCascadingSectionResizes ( true );
  headerView->setSortIndicatorShown ( true );

  connect ( this, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ),
            this, SLOT ( resizeColumnByItem ( QTreeWidgetItem *, int ) ) );

  // Beim ein/ausklappen die Spaltenbreite neu setzen
  connect ( this, SIGNAL ( itemExpanded ( QTreeWidgetItem * ) ),
            this, SLOT ( onExpandCollapseResize ( QTreeWidgetItem * ) ) );

  connect ( this, SIGNAL ( itemCollapsed ( QTreeWidgetItem * ) ),
            this, SLOT ( onExpandCollapseResize ( QTreeWidgetItem * ) ) );
}

/** Die erste Zelle immer Automatisch ändern */
void DockTreeWidget::resizeColumnByItem ( QTreeWidgetItem *item, int i )
{
  QString txt = item->data ( i, Qt::EditRole ).toString();
  int w = ( fontMetrics().width ( txt ) + item->font ( i ).weight() );
  /* Wenn die Breite der ersten Zelle die aktuelle Fensterbreite
  * überschreitet nicht weiter machen. */
  if ( w < 50 || w >= ( width() - 20 ) )
    return;

  if ( w >= minWidth )
    minWidth = w;

  setColumnWidth ( i, minWidth );
}

/**
* Reagiert nur bei einem einzelnen Aufklappen und nicht wenn der Baum
* schon komplett mit @ref QTreeWidget::expandAll aufgeklappt war!
*/
void DockTreeWidget::onExpandCollapseResize ( QTreeWidgetItem *item )
{
  for ( int c = 0; c < item->columnCount(); c++ )
  {
    resizeColumnToContents ( c );
  }
}

/** Kontext Menü für die Datenbäume */
void DockTreeWidget::contextMenuEvent ( QContextMenuEvent *event )
{
  QMenu* menu = new QMenu ( this );
  QAction* acClear = menu->addAction ( KIcon ( QLatin1String ( "edit-clear" ) ), i18n ( "Clear" ) );
  connect ( acClear, SIGNAL ( triggered() ), this, SLOT ( clear() ) );

  QAction* acExpand = menu->addAction ( KIcon ( QLatin1String ( "view-process-all-tree" ) ),
                                        i18n ( "Expand All" ) );
  connect ( acExpand, SIGNAL ( triggered() ), this, SLOT ( expandAll() ) );

  QAction* acCollapse = menu->addAction ( KIcon ( QLatin1String ( "view-list-tree" ) ),
                                          i18n ( "Collapse All" ) );
  connect ( acCollapse, SIGNAL ( triggered() ), this, SLOT ( collapseAll() ) );

  menu->exec ( event->globalPos() );
}

/**
* Baum leeren und die Spaltenbereite zurück setzen.
*/
void DockTreeWidget::restore ()
{
  minWidth = 50;
  clear();
}

DockTreeWidget::~DockTreeWidget()
{}
