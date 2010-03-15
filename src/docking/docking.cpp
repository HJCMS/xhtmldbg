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

#include "docking.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QStringList>

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

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setObjectName ( QLatin1String ( "dockingtreewidget" ) );
  m_treeWidget->setColumnCount ( columns );
  m_treeWidget->setAutoScroll ( true );
  m_treeWidget->setWordWrap ( true );
  m_treeWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  m_treeWidget->setSortingEnabled ( false );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setFrameStyle ( QFrame::Box );

  setWidget ( m_treeWidget );
}

void Docking::setTreeHeaderLabels ( const QStringList &labels )
{
  m_treeWidget->setHeaderLabels ( labels );
}

QTreeWidgetItem* Docking::rootItem() const
{
  return m_treeWidget->invisibleRootItem();
}

const QFontMetrics Docking::fontMetric()
{
  return m_treeWidget->fontMetrics();
}

void Docking::clearContent()
{
  m_treeWidget->clear ();
}

int Docking::columnCount()
{
  return m_treeWidget->columnCount ();
}

void Docking::setColumnCount ( int c )
{
  m_treeWidget->setColumnCount ( c );
}

void Docking::setColumnWidth ( int column, int width )
{
  if ( column < 0 || width < 10 )
    return;

  m_treeWidget->setColumnWidth ( column, width );
}

QTreeWidgetItem* Docking::addTopLevelItem ( QTreeWidgetItem * parent, bool expand )
{
  Q_ASSERT ( parent );
  QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
  item->setExpanded ( expand );
  return item;
}

void Docking::addTopLevelChildItem ( QTreeWidgetItem * child )
{
  Q_ASSERT ( child );
  rootItem()->addChild ( child );
}

bool Docking::itemExists ( const QString &txt )
{
  if ( m_treeWidget->findItems ( txt, Qt::MatchExactly, 0 ).size() > 0 )
    return true;
  else
    return false;
}

Docking::~Docking()
{}
