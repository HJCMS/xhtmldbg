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

#include "postdock.h"

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QTreeWidgetItem>

PostDock::PostDock ( QWidget * parent )
    : Docking ( parent )
{
  setObjectName ( "postdockwidget" );
  setWindowTitle ( trUtf8 ( "Postvars" ) );
  setColumnCount ( 2 );

  QStringList labels;
  labels << trUtf8 ( "Param" ) << trUtf8 ( "Value" );
  setTreeHeaderLabels ( labels );
}

void PostDock::setTreeHeaderLabels ( const QStringList &labels )
{
  Docking::setTreeHeaderLabels ( labels );
}

void PostDock::setPostedData ( const QUrl &url, const QStringList &list )
{
  int minWidth = 0;

  clearContent ();

  QTreeWidgetItem* parent = addTopLevelItem ( rootItem() );
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
    int cw = ( fontMetric().width ( data.last() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  setColumnWidth ( 1, minWidth );
}

PostDock::~PostDock()
{}
