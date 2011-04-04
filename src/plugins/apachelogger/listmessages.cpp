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

#include "listmessages.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ListMessages::ListMessages ( QWidget * parent )
    : QListWidget ( parent )
{
  setObjectName ( QLatin1String ( "ListMessages" ) );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setContextMenuPolicy ( Qt::ActionsContextMenu );

  QAction* m_clear = new QAction ( KIcon ( "edit-clear-list" ), i18n ( "Clear" ), this );
  insertAction ( 0, m_clear );

  connect ( m_clear, SIGNAL ( triggered() ), this, SLOT ( clear() ) );
}

void ListMessages::addLogMessage ( const QString &m )
{
  QListWidgetItem* item = new QListWidgetItem ( m, this, QListWidgetItem::UserType );
  item->setData ( Qt::DisplayRole, m );
  item->setData ( Qt::EditRole, m );

  if ( m.contains ( "[warning]" ) )
    item->setData ( Qt::DecorationRole, KIcon ( "dialog-warning" ) );
  else if ( m.contains ( "[error]" ) )
    item->setData ( Qt::DecorationRole, KIcon ( "dialog-error" ) );
  else
    item->setData ( Qt::DecorationRole,  KIcon ( "dialog-information" ) );

  addItem ( item );
}

ListMessages::~ListMessages()
{}
