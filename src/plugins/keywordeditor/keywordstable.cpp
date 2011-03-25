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

#include "keywordstable.h"
#include "keywordsdom.h"
#include "keywordstableitem.h"
#include "keywordstablemodel.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QAbstractItemView>
#include <QtGui/QHeaderView>

/* QtXml */
// #include <QtXml/QDomElement>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordsTable::KeywordsTable ( QWidget * parent )
    : QTableView ( parent )
    , m_model ( new KeywordsTableModel ( this ) )
{
  setObjectName ( QLatin1String ( "KeywordsTable" ) );
  setContextMenuPolicy ( Qt::ActionsContextMenu );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setModel ( m_model );

  // Actions
  m_insert = new QAction ( KIcon ( "edit-table-insert-row-below" ), i18n ( "Insert" ), this );
  m_insert->setToolTip ( i18n ( "insert row below" ) );

  m_remove = new QAction ( KIcon ( "edit-table-delete-row" ), i18n ( "Delete" ), this );
  m_remove->setToolTip ( i18n ( "delete row" ) );

  insertAction ( 0, m_insert );
  insertAction ( m_insert, m_remove );

  // Signals
  connect ( m_insert, SIGNAL ( triggered () ), m_model, SLOT ( actionInsertRow() ) );
  connect ( m_remove, SIGNAL ( triggered() ), m_model, SLOT ( actionDeleteRow() ) );

  connect ( this, SIGNAL ( clicked ( const QModelIndex & ) ),
            this, SLOT ( prepareModelIndex ( const QModelIndex & ) ) );
}

void KeywordsTable::prepareModelIndex ( const QModelIndex &index )
{
  setCurrentIndex ( index );
  if ( index.column() >= 0 && index.column() <= 2 )
    emit itemClicked ( index );
}

void KeywordsTable::setDomDocument ( const KeywordsDom &dom )
{
  QDomNodeList nodes = dom.keywordNodes();
  int size = nodes.size();
  for ( int i = 0; i < size; ++i )
  {
    QDomElement element = nodes.at ( i ).toElement();
    m_model->insertRowItem ( new KeywordsTableItem ( element.attribute ( "id",QString::number ( i ) ),
                             element.attribute ( "file","index" ),
                             element.text().split ( "," ) ) );
  }
}

KeywordsTable::~KeywordsTable()
{}
