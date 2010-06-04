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
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>

Docking::Docking ( QWidget * parent )
    : QDockWidget ( parent )
    , columns ( 2 )
{
  setObjectName ( "dockingwidget" );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  DockingSplitter = new QSplitter ( Qt::Vertical, this );

  DockingTreeWidgetTop = new QTreeWidget ( this );
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

void Docking::addTreeWidget ( QTreeWidget * widget )
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

QTreeWidget* Docking::widget ( int index )
{
  Q_ASSERT_X ( ( DockingSplitter->count() >= index ), "Docking::widget", "Invalid Index to find TreeWidget" );
  return qobject_cast<QTreeWidget*> ( DockingSplitter->widget ( index ) );
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
