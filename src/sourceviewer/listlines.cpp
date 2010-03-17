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
#include <QtGui/QFontMetrics>
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
  setMinimumWidth ( 20 );
  setMaximumWidth ( 250 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 4, 0, 0 );

  m_listWidget = new QListWidget ( this );
  layout->addWidget ( m_listWidget );

  m_listWidget->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Expanding );
  m_listWidget->setFrameStyle ( QFrame::NoFrame );
  m_listWidget->setAutoScroll ( false );
  m_listWidget->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  m_listWidget->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  m_listWidget->setVerticalScrollMode ( QAbstractItemView::ScrollPerPixel );
  m_listWidget->setHorizontalScrollMode ( QAbstractItemView::ScrollPerPixel );
  m_listWidget->setBackgroundRole ( QPalette::AlternateBase );
  m_listWidget->setAlternatingRowColors ( true ); // @note AlternateBase required
  m_listWidget->setMovement ( QListView::Static );
  m_listWidget->setFont ( font );

  QPalette pl = m_listWidget->palette();
  pl.setColor ( QPalette::Text, Qt::darkGray );
  pl.setColor ( QPalette::Highlight, Qt::darkGreen );
  m_listWidget->setPalette ( pl );

  setLayout ( layout );

  connect ( m_listWidget->verticalScrollBar (), SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( valueChanged ( int ) ) );

  connect ( m_listWidget, SIGNAL ( currentRowChanged ( int ) ),
            this, SIGNAL ( currentRowChanged ( int ) ) );
}

/**
* Setze bei einem klick auf den Eintrag die aktuelle Zeile im Text Dokument.
* @note Weil @class SourceView und @class ListLines mit einander bei einem
*  Scrollen syncronisiert werden. Müssen die Signale in diesem Zeitraum
*  geblockt werden damit es nicht zu einer endlos Schleife kommt wenn
*  der Cursor auf die Textzeile einen Scroll im Textfeld verursacht.
*/
void ListLines::setCurrentRow ( int r )
{
  /* NOTE Block Signals an didn't send currentRowChanged 
  * if User hase selected by TextEdit */
  blockSignals ( true );
  m_listWidget->setCurrentRow ( r );
  blockSignals ( false );
}

/**
* Befülle die Liste mit den aus @ref SourceView::createListWidgetItems()
* erstellten Inhalten. Wenn das neu einfügen der Zeilen fertig ist nehme
* mit fontMetrics die aktuelle Schriftengröße und hänge als Margin Platzhalter 
* zwei 0. an damit es nicht zu Fehldarstellungen bei mehr als 13000 Zeilen kommt.
*/
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
    }
    QString lastItem = QString::number( list.size() );
    m_listWidget->addItem ( lastItem );
    int fwidth = fontMetrics().width( lastItem + "00" );
    setMaximumWidth ( fwidth );
  }
}

/**
* Verarbeite den Scrollbalken für die Syncronisierung mit
* @class SourceView siehe auch @ref setCurrentRow zu diesem Thema.
* Es ist wichtig das QAbstractItemView::ScrollPerPixel definiert ist
* damit es zu einer sauberen Scrollleisten syncronisierung kommt.
*/
void ListLines::setValue ( int i )
{
  m_listWidget->verticalScrollBar ()->setValue ( i );
}

ListLines::~ListLines ()
{
}

