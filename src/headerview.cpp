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

#include "headerview.h"

/* QtCore */
#include <QtCore/QMapIterator>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QSizePolicy>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItem>

HeaderView::HeaderView ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( "headerview" );
  setWindowTitle ( trUtf8 ( "Header" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  QStringList labels;
  labels << trUtf8 ( "Header" ) << trUtf8 ( "Value" );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setAutoScroll ( true );
  m_treeWidget->setWordWrap ( true );
  m_treeWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->setSortingEnabled ( false );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setFrameStyle ( QFrame::Box );

  setWidget ( m_treeWidget );
}

void HeaderView::setHeaders ( const QString &host, const QMap<QString,QString> &map )
{
  QFontMetrics fontMetric = m_treeWidget->fontMetrics();
  int minWidth = 0;

  if ( m_treeWidget->findItems ( host, Qt::MatchExactly, 0 ).size() > 0 )
    return;

  clearItems ();

  QTreeWidgetItem* parent = new QTreeWidgetItem ( m_treeWidget->invisibleRootItem() );
  parent->setExpanded ( true );
  parent->setData ( 0, Qt::DisplayRole, host );
  parent->setText ( 1, trUtf8 ( "Hostname" ) );
  parent->setForeground ( 1, Qt::lightGray );

  QMapIterator<QString,QString> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setData ( 0, Qt::DisplayRole, it.key() );
    item->setData ( 1, Qt::DisplayRole, it.value() );
    parent->addChild ( item );
    int cw = ( fontMetric.width ( it.value() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  if ( minWidth > 10 )
    m_treeWidget->setColumnWidth ( 1, minWidth );
}

void HeaderView::clearItems ()
{
  m_treeWidget->clear();
}

HeaderView::~HeaderView()
{}
