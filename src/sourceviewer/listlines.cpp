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

#include "listlines.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QPalette>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QPalette>

ListLines::ListLines ( const QFont &font, QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( "listlines" );
  setContentsMargins ( 0, 0, 0, 0 );
  setLayoutDirection ( Qt::RightToLeft );
  setMinimumWidth ( 30 );
  setMaximumWidth ( 100 );
  setBackgroundRole ( QPalette::Light );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 4, 0, 4 );

  m_listWidget = new QListWidget ( this );
  layout->addWidget ( m_listWidget );

  m_listWidget->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Expanding );
  m_listWidget->setFrameStyle ( QFrame::NoFrame );
  m_listWidget->setAutoScroll ( false );
  m_listWidget->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  m_listWidget->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  m_listWidget->setVerticalScrollMode ( QAbstractItemView::ScrollPerPixel );
  m_listWidget->setHorizontalScrollMode ( QAbstractItemView::ScrollPerPixel );
  m_listWidget->setFont ( font );

  QPalette pl = m_listWidget->palette();
  pl.setColor ( QPalette::Text, Qt::lightGray );
  m_listWidget->setPalette ( pl );

  setLayout ( layout );

  connect ( m_listWidget->verticalScrollBar (), SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( valueChanged ( int ) ) );

  connect ( m_listWidget, SIGNAL ( currentRowChanged ( int ) ),
            this, SIGNAL ( currentRowChanged ( int ) ) );
}

void ListLines::setCurrentRow ( int r )
{
  /* NOTE Block Signals an didn't send currentRowChanged 
  * if User hase selected by TextEdit */
  blockSignals ( true );
  m_listWidget->setCurrentRow ( r );
  blockSignals ( false );
}

void ListLines::setItems ( const QList<QListWidgetItem*> &list )
{
  int c = 0;
  if ( list.size() >= 1 )
  {
    m_listWidget->clear();
    foreach ( QListWidgetItem* i, list )
    {
      m_listWidget->addItem ( i );
      c++;
      if ( c >= list.size() )
        setMaximumWidth ( i->font().weight() );
    }
  }
}

void ListLines::setValue ( int i )
{
  m_listWidget->verticalScrollBar ()->setValue ( i );
}

ListLines::~ListLines ()
{
}

