/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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
#include "downloader.h"

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

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

DownloadsTable::DownloadsTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "downloadstable" ) );
  m_model = new DownloadsTableModel ( this );
  setModel ( m_model );

  connect ( this, SIGNAL ( clicked ( const QModelIndex & ) ),
            this, SLOT ( downloadClicked ( const QModelIndex & ) ) );
}

/**
* Bei einem anklicken Klasse @ref Downloader an @ref itemClicked weitergeben.
*/
void DownloadsTable::downloadClicked ( const QModelIndex &index )
{
  Downloader* item = m_model->rowItem ( index.row() );
  if ( item )
    emit itemClicked ( item );
}

/**
* Einen aktuellen Download abbrechen!
*/
void DownloadsTable::abort()
{
  if ( currentIndex ().isValid() )
    m_model->abortDownload ( currentIndex() );
}

/**
* Entferne Download aus der Tabelle
*/
void DownloadsTable::remove()
{
  if ( currentIndex ().isValid() )
    m_model->removeDownload ( currentIndex() );
}

/**
* Download neu Starten
*/
void DownloadsTable::restart()
{
  if ( currentIndex ().isValid() )
    m_model->restartDownload ( currentIndex() );
}

/**
* Kontext Menü für die Tabelle
*/
void DownloadsTable::contextMenuEvent ( QContextMenuEvent *ev )
{
  QMenu *m_menu = new QMenu ( "Actions", this );
  QAction *add = m_menu->addAction ( KIcon ( "process-stop" ), i18n ( "Stop" ) );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( abort() ) );
  QAction *del= m_menu->addAction ( KIcon ( "list-remove" ), i18n ( "Remove" ) );
  connect ( del, SIGNAL ( triggered() ), this, SLOT ( remove() ) );
//   QAction *start = m_menu->addAction ( KIcon ( "process-working" ), i18n ( "Restart" ) );
//   connect ( start, SIGNAL ( triggered() ), this, SLOT ( restart() ) );
  m_menu->exec ( ev->globalPos() );
  delete m_menu;
}

/**
* Übergebe den Download an das Model.
* Wenn ein Eintrag bereits vorhanden ist wird hier abgebrochen!
*/
bool DownloadsTable::setDownloadItem ( Downloader * item )
{
  foreach ( Downloader *it, m_model->downloadItems() )
  {
    if ( it->destFile() == item->destFile() )
      return false;
  }
  m_model->addDownload ( item );
  return true;
}

DownloadsTable::~DownloadsTable()
{}
