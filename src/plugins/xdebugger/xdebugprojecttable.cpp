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

#include "xdebugprojecttable.h"
#include "xdebugprojectitem.h"
#include "xdebugprojecttablemodel.h"

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

/* QtNetwork */
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>

/**
* @class XDebugProjectTable
* xdebugprojecttable
*/
XDebugProjectTable::XDebugProjectTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "projecttable" ) );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setWordWrap ( false );
  setCornerButtonEnabled ( false );

  m_model = new XDebugProjectTableModel ( this );
  setModel ( m_model );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMovable ( true );

  connect ( m_model, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );
}

void XDebugProjectTable::addRow()
{
  QString ip;
  QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
  // Suche die erste NICHT localhost IPv4 Adresse
  for ( int i = 0; i < ipList.size(); ++i )
  {
    if ( ipList.at ( i ) != QHostAddress::LocalHost && ipList.at ( i ).toIPv4Address() )
    {
      ip = ipList.at ( i ).toString();
      break;
    }
  }
  // wenn kein Treffer dann localhost verwenden
  if ( ip.isEmpty() )
    ip = QHostAddress ( QHostAddress::LocalHost ).toString();

  QString num = QString::number ( ( m_model->rowCount() + 1 ) );
  XDebugProjectItem *item = new XDebugProjectItem;
  item->setPort ( QLatin1String ( "9000" ) );
  item->setName ( trUtf8 ( "Project %1" ).arg ( num ) );
  item->setSessionId ( QLatin1String ( "XDEBUG_PROFILE" ) );
  item->setRemoteHost ( ip );
  m_model->addProject ( item );
}

void XDebugProjectTable::removeRow()
{
  if ( currentIndex ().isValid() )
  {
    QModelIndex index = currentIndex();
    m_model->removeProject ( index.row(), index.parent() );
  }
}

void XDebugProjectTable::contextMenuEvent ( QContextMenuEvent *ev )
{
  QMenu *m_menu = new QMenu ( "Actions", this );
  QAction *add = m_menu->addAction ( QIcon::fromTheme ( "list-add" ), trUtf8 ( "New" ) );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( addRow() ) );
  QAction *del= m_menu->addAction ( QIcon::fromTheme ( "list-remove" ), trUtf8 ( "Remove" ) );
  connect ( del, SIGNAL ( triggered() ), this, SLOT ( removeRow() ) );
  m_menu->exec ( ev->globalPos() );
  delete m_menu;
}

void XDebugProjectTable::addProject ( XDebugProjectItem * prj )
{
  m_model->addProject ( prj );
}

int XDebugProjectTable::rowCount()
{
  return m_model->rowCount();
}

XDebugProjectItem* XDebugProjectTable::rowProjectItem ( int row )
{
  return m_model->rowItem ( row );
}

XDebugProjectTable::~XDebugProjectTable()
{}
