/**
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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

#include "docking.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QSizePolicy>

DockTreeWidget::DockTreeWidget ( QWidget * parent )
    : QTreeWidget ( parent )
    , minWidth ( 50 )
{
  setObjectName ( "dockingtreewidget" );
  header()->setResizeMode ( 0, QHeaderView::ResizeToContents );
  header()->setStretchLastSection ( true );

  connect ( this, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ),
            this, SLOT ( resizeFirstColumn ( QTreeWidgetItem *, int ) ) );
}

/** Die erste Zelle immer Automatisch ändern */
void DockTreeWidget::resizeFirstColumn ( QTreeWidgetItem *item, int i )
{
  if ( i != 0 )
    return;

  QString txt = item->data ( i, Qt::EditRole ).toString();
  int w = ( fontMetrics().width ( txt ) + item->font ( i ).weight() );
  if ( w >= minWidth )
    minWidth = w;

  setColumnWidth ( i, minWidth );
}

/** Kontext Menü für die Datenbäume */
void DockTreeWidget::contextMenuEvent ( QContextMenuEvent *event )
{
  QMenu* menu = new QMenu ( this );
  QAction* acClear = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "edit-clear" ) ),
                                       trUtf8 ( "Clear" ) );
  connect ( acClear, SIGNAL ( triggered() ), this, SLOT ( clear() ) );

  QAction* acExpand = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-process-all-tree" ) ),
                                        trUtf8 ( "Expand All" ) );
  connect ( acExpand, SIGNAL ( triggered() ), this, SLOT ( expandAll() ) );

  QAction* acCollapse = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-list-tree" ) ),
                                          trUtf8 ( "Collapse All" ) );
  connect ( acCollapse, SIGNAL ( triggered() ), this, SLOT ( collapseAll() ) );

  menu->exec ( event->globalPos() );
}

DockTreeWidget::~DockTreeWidget()
{}

Docking::Docking ( QWidget * parent )
    : QDockWidget ( parent )
    , columns ( 2 )
{
  setObjectName ( "dockingwidget" );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  DockingSplitter = new QSplitter ( Qt::Vertical, this );

  DockingTreeWidgetTop = new DockTreeWidget ( this );
  DockingTreeWidgetTop->setObjectName ( QLatin1String ( "dockingtreewidgettop" ) );
  DockingTreeWidgetTop->setColumnCount ( columns );
  addTreeWidget ( DockingTreeWidgetTop );

  setWidget ( DockingSplitter );
}

void Docking::setTreeHeaderLabels ( const QStringList &labels, int index )
{
  widget ( index )->setHeaderLabels ( labels );
}

QTreeWidgetItem* Docking::rootItem ( int index )
{
  return widget ( index )->invisibleRootItem();
}

const QFontMetrics Docking::fontMetric ( int index )
{
  return widget ( index )->fontMetrics();
}

void Docking::addTreeWidget ( DockTreeWidget * widget )
{
  Q_ASSERT ( widget );
  widget->setAutoScroll ( true );
  widget->setWordWrap ( true );
  widget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  widget->setSortingEnabled ( false );
  widget->header()->setResizeMode ( QHeaderView::Interactive );
  widget->setFrameStyle ( QFrame::Box );
  DockingSplitter->insertWidget ( DockingSplitter->count(), widget );
}

DockTreeWidget* Docking::widget ( int index )
{
  Q_ASSERT_X ( ( DockingSplitter->count() >= index ), "Docking::widget", "Invalid Index to find TreeWidget" );
  return qobject_cast<DockTreeWidget*> ( DockingSplitter->widget ( index ) );
}

void Docking::clearContent ( int index )
{
  widget ( index )->clear ();
}

void Docking::resizeSections ( int index )
{
  widget ( index )->header ()->resizeSections ( QHeaderView::ResizeToContents );
}

int Docking::columnCount ( int index )
{
  return widget ( index )->columnCount ();
}

void Docking::setColumnCount ( int count, int index )
{
  widget ( index )->setColumnCount ( count );
}

void Docking::setColumnWidth ( int column, int width, int index )
{
  if ( column < 0 || width < 10 )
    return;

  widget ( index )->setColumnWidth ( column, width );
}

QTreeWidgetItem* Docking::addTopLevelItem ( QTreeWidgetItem * parent, bool expand )
{
  Q_ASSERT ( parent );
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setExpanded ( expand );
  return item;
}

void Docking::addTopLevelChildItem ( QTreeWidgetItem * child, int index )
{
  Q_ASSERT ( child );
  rootItem ( index )->addChild ( child );
}

bool Docking::itemExists ( const QString &txt, int index, int column )
{
  if ( widget ( index )->findItems ( txt, Qt::MatchExactly, column ).size() > 0 )
    return true;
  else
    return false;
}

Docking::~Docking()
{}
