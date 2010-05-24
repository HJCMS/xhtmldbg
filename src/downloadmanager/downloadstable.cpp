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

#include "downloadstable.h"
#include "downloader.h"
#include "downloadstablemodel.h"

/* QtCore */
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QAbstractItemView>

DownloadsTable::DownloadsTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "downloadstableviewer" ) );

  m_model = new DownloadsTableModel ( this );
  setModel ( m_model );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMovable ( true );

//   connect ( m_model, SIGNAL ( modified ( bool ) ),
//             this, SIGNAL ( modified ( bool ) ) );
}

void DownloadsTable::addItem ( Downloader * item )
{
  if ( ! item )
    return;

  m_model->addDownload ( item );
}

DownloadsTable::~DownloadsTable()
{}
