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

#include "useragentstable.h"
#include "useragent.h"
#include "useragentstablemodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include <QtGui/QMenu>

UserAgentsTable::UserAgentsTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "UserAgentsTable" ) );
  setMinimumWidth ( 400 );
  setWordWrap ( false );
  setAlternatingRowColors ( true );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setSelectionBehavior ( QAbstractItemView::SelectItems );
  setSelectionMode ( QAbstractItemView::ExtendedSelection );
  setDragDropMode ( QAbstractItemView::DragOnly );
  setDragEnabled ( true );
  verticalHeader()->hide();

  m_model = new UserAgentsTableModel ( this );
  setModel ( m_model );

  connect ( m_model, SIGNAL ( update ( const QModelIndex & ) ),
            this, SLOT ( update ( const QModelIndex & ) ) );

  connect ( m_model, SIGNAL ( agentChanged ( bool ) ),
            this, SIGNAL ( agentChanged ( bool ) ) );
}

/** Zeile entfernen */
void UserAgentsTable::removeRow()
{
  if ( currentIndex ().isValid() )
    m_model->removeItem ( currentIndex() );
}

/** Menü */
void UserAgentsTable::contextMenuEvent ( QContextMenuEvent *ev )
{
  QMenu *m_menu = new QMenu ( "Actions", this );

  QAction* add = m_menu->addAction ( QIcon::fromTheme ( "list-add" ), i18n ( "Add" ) );
  connect ( add, SIGNAL ( triggered() ), m_model, SLOT ( addItem() ) );

  QAction* del = m_menu->addAction ( QIcon::fromTheme ( "list-remove" ), i18n ( "Remove" ) );
  connect ( del, SIGNAL ( triggered() ), this, SLOT ( removeRow() ) );

  m_menu->exec ( ev->globalPos() );
  delete m_menu;
}

/** Zeile verschieben */
void UserAgentsTable::moveRow ( int t )
{
  if ( ! currentIndex ().isValid() )
    return;

  int size = m_model->rowCount ( currentIndex () );
  int row = currentIndex ().row();
  if ( ( t == 1 ) && ( size > ( row + 1 ) ) )
  {
    m_model->moveRowTo ( row, ( row + 1 ) );
    clearSelection();
    clearFocus();
    emit agentChanged ( true );
  }
  else if ( ( t == -1 ) && ( row - 1 ) >= 0 )
  {
    m_model->moveRowTo ( row, ( row - 1 ) );
    clearSelection();
    clearFocus();
    emit agentChanged ( true );
  }
}

/** Tabelle befüllen */
void UserAgentsTable::loadUserAgents ( Settings * cfg )
{
  int size = cfg->beginReadArray ( QLatin1String ( "UserAgents" ) );
  QList<UserAgent*> list;
  for ( int i = 0; i < size; ++i )
  {
    cfg->setArrayIndex ( i );
    UserAgent* item = new UserAgent ( this );
    item->setName ( cfg->value ( QLatin1String ( "name" ), "" ).toString() );
    item->setAgent ( cfg->value ( QLatin1String ( "agent" ), "" ).toString() );
    item->setTokens ( cfg->value ( QLatin1String ( "tokens" ), "" ).toString() );
    list << item;
  }
  cfg->endArray();
  m_model->addItems ( list );
}

/** Tabelle Speichern */
void UserAgentsTable::saveUserAgents ( Settings * cfg )
{
  cfg->remove ( QLatin1String ( "UserAgents" ) );
  if ( m_model->rowCount ( QModelIndex() ) < 1 )
    return;

  cfg->beginWriteArray ( QLatin1String ( "UserAgents" ) );
  int i = 0;
  foreach ( UserAgent* item, m_model->agents() )
  {
    cfg->setArrayIndex ( i++ );
    cfg->setValue ( QLatin1String ( "name" ), item->getName() );
    cfg->setValue ( QLatin1String ( "agent" ), item->getAgent() );
    cfg->setValue ( QLatin1String ( "tokens" ), item->getTokens() );
  }
  cfg->endArray();
}

UserAgentsTable::~UserAgentsTable()
{}
