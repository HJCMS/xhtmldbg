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

#include "certissuertable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidgetItem>

/* KDE */
#include <KDE/KLocale>

CertIssuerTable::CertIssuerTable ( QWidget * parent )
    : QTableWidget ( parent )
{
  setObjectName ( QLatin1String ( "certissuertable" ) );
  setMinimumHeight ( 250 );

  setRowCount(0);
  setColumnCount ( 3 );
  setHorizontalHeaderItem ( 0, new QTableWidgetItem ( i18n ( "Organization (O)" ) ) );
  setHorizontalHeaderItem ( 1, new QTableWidgetItem ( i18n ( "CommonName (CN)" ) ) );
  setHorizontalHeaderItem ( 2, new QTableWidgetItem ( i18n ( "Expires" ) ) );

  setContextMenuPolicy ( Qt::NoContextMenu );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setProperty ( "showDropIndicator", QVariant ( false ) );
  setGridStyle ( Qt::DashLine );
  setDragDropOverwriteMode ( false );
  setCornerButtonEnabled ( false );
  setAlternatingRowColors ( true );
  setTabKeyNavigation ( true );
  setWordWrap ( false );

  horizontalHeader()->setHighlightSections ( false );
  horizontalHeader()->setProperty ( "showSortIndicator", QVariant ( false ) );
  horizontalHeader()->setStretchLastSection ( true );

  verticalHeader()->setVisible ( false );
  verticalHeader()->setStretchLastSection ( true );
}

CertIssuerTable::~CertIssuerTable()
{}
