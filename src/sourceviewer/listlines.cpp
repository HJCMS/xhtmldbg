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

  QSizePolicy sizePolicy ( QSizePolicy::Minimum, QSizePolicy::Expanding );
  sizePolicy.setHorizontalStretch ( 0 );
  sizePolicy.setVerticalStretch ( 0 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  // Zeilenstart Korrigieren
  layout->setContentsMargins ( 0, 4, 0, 0 );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setContentsMargins ( 0, 0, 0, 0 );
  layout->addWidget ( m_listWidget );

  m_listWidget->setSizePolicy ( sizePolicy );
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

  setMinimumWidth ( m_listWidget->fontMetrics().width ( "00" ) );
  setMaximumWidth ( m_listWidget->fontMetrics().width ( "0000" ) );

  connect ( m_listWidget->verticalScrollBar (), SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( valueChanged ( int ) ) );

  connect ( m_listWidget, SIGNAL ( currentRowChanged ( int ) ),
            this, SIGNAL ( currentRowChanged ( int ) ) );
}

/**
* Setzt mit @ref QListWidget::fontMetrics die Maximal
* sichtbare Zeilenbreite für die Darstellung neu.
* FIXME KDE4 mit Oxgen stellt die Zeilenbreite von @class ListLines nicht korrekt dar!
*/
void ListLines::setRowsWidth ( int rows )
{
  // Füge eine Zeile wegen dem Scrollbalken hinzu.
  QString lastItem = QString::number ( ( rows + 1 ) );
  m_listWidget->addItem ( lastItem );
  int fwidth = m_listWidget->fontMetrics().width ( lastItem + "0" );
  if ( fwidth >= minimumWidth() )
    setMaximumWidth ( fwidth );

  // qDebug() << Q_FUNC_INFO << minimumWidth() << maximumWidth() << rows;
}

/**
* Wenn sich die Zeilenanzahl geändert hat wird in @ref setItems
* Der SLOT @ref QWidget::update aufgerufen.
* Setze an dieser Stelle mit @ref setRowsWidth die maximale Breite.
*/
void ListLines::paintEvent ( QPaintEvent * event )
{
  setRowsWidth ( m_listWidget->count() );
  QWidget::paintEvent ( event );
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
  if ( list.size() >= 1 )
  {
    int c = 0;
    m_listWidget->clear();
    foreach ( QListWidgetItem* i, list )
    {
      m_listWidget->addItem ( i );
      c++;
    }
    update();
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

