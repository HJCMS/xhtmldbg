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

#include "appevents.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>

AppEvents::AppEvents ( QWidget * parent )
    : QDockWidget ( parent )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
{
  setObjectName ( "appevents" );
  setWindowTitle ( trUtf8 ( "XHTMLDBG Impartations" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setFocusPolicy ( Qt::StrongFocus );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( true );
  m_listWidget->setWordWrap ( false );
  m_listWidget->sortItems ( Qt::AscendingOrder );
  m_listWidget->setSelectionMode ( QAbstractItemView::SingleSelection );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );

  setWidget ( m_listWidget );
}

void AppEvents::sortAscending()
{
  m_listWidget->sortItems ( Qt::AscendingOrder );
}

void AppEvents::sortDescending()
{
  m_listWidget->sortItems ( Qt::DescendingOrder );
}

void AppEvents::contextMenuEvent ( QContextMenuEvent *e )
{
  QMenu* menu = new QMenu ( this );

  QAction* ac_Ascending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-ascending" ) ),
                          trUtf8 ( "Ascending" ) );
  connect ( ac_Ascending, SIGNAL ( triggered() ), this, SLOT ( sortAscending() ) );

  QAction* ac_Descending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-descending" ) ),
                           trUtf8 ( "Descending" ) );
  connect ( ac_Descending, SIGNAL ( triggered() ), this, SLOT ( sortDescending() ) );

  QAction* ac_clear = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "edit-clear" ) ),
                                        trUtf8 ( "Clear" ) );
  connect ( ac_clear, SIGNAL ( triggered() ), m_listWidget, SLOT ( clear() ) );

  menu->exec ( e->globalPos() );
}

void AppEvents::statusMessage ( const QString &mess )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconNotice, mess.trimmed(), m_listWidget ) );
  emit itemsChanged();
}

void AppEvents::warningMessage ( const QString &mess )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconWarning, mess.trimmed(), m_listWidget ) );
  emit itemsChanged();
}

void AppEvents::clearItems()
{
  m_listWidget->clear();
}

AppEvents::~AppEvents()
{
  clearItems();
}
