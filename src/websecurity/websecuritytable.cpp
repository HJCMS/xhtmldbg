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

#include "websecuritytable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QSizePolicy>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

WebSecurityTable::WebSecurityTable ( QWidget * parent )
    : QTableWidget ( 0, 0, parent )
{
  setObjectName ( QLatin1String ( "config_web_security_table" ) );
  setMinimumHeight ( 200 );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  setContextMenuPolicy ( Qt::ActionsContextMenu );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setGridStyle ( Qt::DashLine );
  setWordWrap ( false );

  m_remove = new QAction ( i18n ( "Remove Entry" ), this );
  m_remove->setIcon ( KIcon ( "list-remove" ) );
  insertAction ( 0, m_remove );

  QStringList labels;
  labels << i18n ( "Host" ) << i18n ( "Port" ) << i18n ( "Scheme" ) << i18n ( "Quota" );
  setColumnCount ( labels.size() );
  setHorizontalHeaderLabels ( labels );

  QHeaderView* headerView = horizontalHeader();
  headerView->setHighlightSections ( false );
  headerView->setProperty ( "showSortIndicator", QVariant ( false ) );
  headerView->setStretchLastSection ( true );
  headerView->setResizeMode ( QHeaderView::Interactive );
  headerView->setDefaultSectionSize ( 150 );

  connect ( m_remove, SIGNAL ( triggered () ),
            this, SLOT ( removeSelectedRow () ) );

  connect ( this, SIGNAL ( itemClicked ( QTableWidgetItem * ) ),
            this, SLOT ( rowChanged ( QTableWidgetItem * ) ) );
}

/** Auswahl auf Spalte weitergeben! */
void WebSecurityTable::rowChanged ( QTableWidgetItem * item )
{
  emit currentIndexChanged ( item->row() );
}

/** Ausgewählte Zeile entfernen */
void WebSecurityTable::removeSelectedRow()
{
  if ( selectedItems().size() < 1 )
    return;

  removeRow ( currentRow() );
  emit modified ( true );
}

WebSecurityTable::~WebSecurityTable()
{}
