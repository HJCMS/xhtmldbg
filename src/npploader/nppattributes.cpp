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

#include "nppattributes.h"

// https://developer.mozilla.org/en/Gecko_Plugin_API_Reference
// extern "C"
// {
// #include <npapi.h>
// #include <npfunctions.h>
// }

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSizePolicy>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QVBoxLayout>

NPPAttributes::NPPAttributes ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "nppattributes" ) );
  setMinimumWidth ( 150 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( "nppattributes/layout" );

  QLabel* info = new QLabel ( trUtf8 ( "Plugin Object Detected" ), this );
  info->setObjectName ( "nppattributes/layout/label" );
  vLayout->addWidget ( info );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setObjectName ( "nppattributes/layout/tree" );

  QStringList labels;
  labels << trUtf8 ( "Predicates" ) << trUtf8 ( "Values" );
  m_treeWidget->setColumnCount ( labels.size() );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->setSortingEnabled ( false );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  m_treeWidget->setFrameStyle ( QFrame::Box );
  vLayout->addWidget ( m_treeWidget );

  QLabel* notice = new QLabel ( trUtf8 ( "Do not report bugs to XHTMLDBG Project if plug-ins produce application crashes. XHTMLDBG is not relevant for loading plugged extensions. All NPP Browser Plugins loaded by WebKit." ), this );
  notice->setObjectName ( "nppattributes/layout/notice" );
  notice->setWordWrap ( true );
  vLayout->addWidget ( notice );

  QPushButton* fire = new QPushButton ( trUtf8 ( "Initiale" ), this );
  fire->setObjectName ( "nppattributes/layout/button" );
  vLayout->addWidget ( fire, 0, Qt::AlignLeft );

  setLayout ( vLayout );

  connect ( fire, SIGNAL ( clicked() ), this, SIGNAL ( startPlugin() ) );
}

void NPPAttributes::addItem ( const QString &param, const QString &value )
{
  QTreeWidgetItem* item = new QTreeWidgetItem ( m_treeWidget );
  item->setData ( 0, Qt::DisplayRole, param );
  item->setData ( 0, Qt::ToolTipRole, trUtf8 ( "Predicate" ) );
  item->setData ( 1, Qt::DisplayRole, value );
  item->setData ( 1, Qt::ToolTipRole, trUtf8 ( "Value" ) );
  m_treeWidget->addTopLevelItem ( item );
}

NPPAttributes::~NPPAttributes()
{}
