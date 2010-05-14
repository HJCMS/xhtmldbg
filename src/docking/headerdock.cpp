/**
* This file is part of the xhtmldbg project
*
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

#include "headerdock.h"

/* QtCore */
#include <QtCore/QMapIterator>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QSizePolicy>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItem>

HeaderDock::HeaderDock ( QWidget * parent )
    : Docking ( parent )
{
  setObjectName ( "headerview" );
  setWindowTitle ( trUtf8 ( "Header" ) );

  QStringList labels;
  labels << trUtf8 ( "Header" ) << trUtf8 ( "Value" );
  setColumnCount ( 2, 0 );
  setTreeHeaderLabels ( labels, 0 );

  m_treePostVars = new QTreeWidget ( this );
  m_treePostVars->setObjectName ( QLatin1String ( "postvariables" ) );
  addTreeWidget ( m_treePostVars );
  setColumnCount ( 2, 1 );
  setTreeHeaderLabels ( labels, 1 );
}

void HeaderDock::setTreeHeaderLabels ( const QStringList &labels, int index )
{
  Docking::setTreeHeaderLabels ( labels, index );
}

void HeaderDock::setHeaderData ( const QString &host, const QMap<QString,QString> &map )
{
  int widgetIndex = 0;
  int minWidth = 0;

  if ( itemExists ( host ) )
    return;

  clearContent ( widgetIndex );

  QTreeWidgetItem* parent = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
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
    int cw = ( fontMetric ( widgetIndex ).width ( it.value() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  setColumnWidth ( 1, minWidth, widgetIndex );
  resizeSections ( widgetIndex );
}

void HeaderDock::setPostedData ( const QUrl &url, const QStringList &list )
{
  int widgetIndex = 1;
  int minWidth = 0;

  clearContent ( widgetIndex );

  QTreeWidgetItem* parent = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
  parent->setData ( 0, Qt::DisplayRole, url.host() );
  parent->setText ( 1, url.path() );
  parent->setForeground ( 1, Qt::lightGray );

  if ( url.hasQuery() )
  {
    QList<QPair<QString, QString> > pairs ( url.queryItems() );
    for ( int i = 0; i < pairs.size(); i++ )
    {
      QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
      item->setData ( 0, Qt::DisplayRole, pairs.at ( i ).first );
      item->setForeground ( 0, Qt::lightGray );
      item->setData ( 1, Qt::DisplayRole, pairs.at ( i ).second );
      parent->addChild ( item );
    }
  }

  if ( url.hasFragment() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setData ( 0, Qt::DisplayRole, trUtf8 ( "Anchor" ) );
    item->setForeground ( 0, Qt::lightGray );
    item->setData ( 1, Qt::DisplayRole, url.fragment() );
    item->setData ( 1, Qt::ToolTipRole, QLatin1String ( "Fragment" ) );
    parent->addChild ( item );
  }

  foreach ( QString str, list )
  {
    QStringList data = str.split ( QString::fromUtf8 ( "=" ) );
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setData ( 0, Qt::DisplayRole, data.first() );
    item->setData ( 1, Qt::DisplayRole, data.last() );
    parent->addChild ( item );
    int cw = ( fontMetric ( widgetIndex ).width ( data.last() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  setColumnWidth ( 1, minWidth, widgetIndex );
  resizeSections ( widgetIndex );
}

HeaderDock::~HeaderDock()
{}
