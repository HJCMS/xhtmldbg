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
#include "downloadstablemodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QMenu>

DownloadsTable::DownloadsTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "downloadstable" ) );
  m_model = new DownloadsTableModel ( this );
  setModel ( m_model );
}

void DownloadsTable::abort()
{
  if ( currentIndex ().isValid() )
  {
    QModelIndex index = currentIndex();
    qDebug() << Q_FUNC_INFO << "TODO" << index.row();
    // m_model->removeProject ( index.row(), index.parent() );
  }
}

void DownloadsTable::remove()
{
  if ( currentIndex ().isValid() )
  {
    QModelIndex index = currentIndex();
    qDebug() << Q_FUNC_INFO << "TODO" << index.row();
    // m_model->removeProject ( index.row(), index.parent() );
  }
}

void DownloadsTable::contextMenuEvent ( QContextMenuEvent *ev )
{
  QMenu *m_menu = new QMenu ( "Actions", this );
  QAction *add = m_menu->addAction ( QIcon::fromTheme ( "process-stop" ), trUtf8 ( "Stop" ) );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( abort() ) );
  QAction *del= m_menu->addAction ( QIcon::fromTheme ( "list-remove" ), trUtf8 ( "Remove" ) );
  connect ( del, SIGNAL ( triggered() ), this, SLOT ( remove() ) );
  m_menu->exec ( ev->globalPos() );
  delete m_menu;
}

void DownloadsTable::setDownloadItem ( Downloader * item )
{
  m_model->addDownload ( item );
}

DownloadsTable::~DownloadsTable()
{}
