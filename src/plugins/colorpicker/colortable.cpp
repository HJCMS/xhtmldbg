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

#include "colortable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QBrush>
#include <QtGui/QHeaderView>
#include <QtGui/QSizePolicy>

ColorTable::ColorTable ( QWidget * parent )
    : QTableWidget ( parent )
    , fixedColumnCount ( 20 )
    , fixedSizeFactor ( 25 )
{
  setObjectName ( QLatin1String ( "colortable" ) );
  setFrameShape ( QFrame::NoFrame );
  setAutoScroll ( false );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setSelectionBehavior ( QAbstractItemView::SelectItems );
  setDragDropMode ( QAbstractItemView::NoDragDrop );
  setTabKeyNavigation ( false );
  setProperty ( "showDropIndicator", QVariant ( false ) );
  setWordWrap ( false );
  setCornerButtonEnabled ( false );
  setColumnCount ( fixedColumnCount );

  horizontalHeader()->setVisible ( false );
  horizontalHeader()->setDefaultSectionSize ( fixedSizeFactor );
  horizontalHeader()->setHighlightSections ( false );
  horizontalHeader()->setMinimumSectionSize ( fixedSizeFactor );

  verticalHeader()->setVisible ( false );
  verticalHeader()->setDefaultSectionSize ( fixedSizeFactor );
  verticalHeader()->setHighlightSections ( false );
  verticalHeader()->setMinimumSectionSize ( fixedSizeFactor );

  setMaximumWidth ( qRound ( fixedSizeFactor * fixedColumnCount ) );

  connect ( this, SIGNAL ( itemClicked ( QTableWidgetItem * ) ),
            this, SLOT ( selectedItem ( QTableWidgetItem * ) ) );
}

void ColorTable::selectedItem ( QTableWidgetItem * item )
{
  QColor col = item->data ( Qt::UserRole ).value<QColor>();
  if ( col.isValid() )
    emit colorChanged ( col );
}

void ColorTable::insertColorCell ( int row, int column, const QColor &color )
{
  QVariant val ( QVariant::Color );
  val.setValue ( color );

  // qDebug() << Q_FUNC_INFO << row << column << color;
  QTableWidgetItem* item = new QTableWidgetItem ( QTableWidgetItem::UserType );
  item->setBackground ( QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::BackgroundRole , QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::BackgroundColorRole , QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::UserRole, val );
  QString tip = QString ( "rgb(%1,%2,%3)\n%4" ).arg (
                    QString::number ( color.red() ),
                    QString::number ( color.green() ),
                    QString::number ( color.blue() ),
                    color.name()
                );
  item->setData ( Qt::ToolTipRole, tip );
  item->setData ( Qt::TextAlignmentRole, Qt::AlignCenter );

  setItem ( row, column, item );
}

/**
* Farbtabelle neu befüllen
*/
void ColorTable::insertColors ( const QVector<QColor> &vector )
{
  int row = 0;
  int column = 0;
  if ( vector.size() < fixedColumnCount )
    return;

  clearContents ();
  setRowCount ( 1 );
  for ( int i = 0; i < vector.size(); ++i )
  {
    if ( ( column >= fixedColumnCount ) && ( i % fixedColumnCount == 0 ) )
    {
      ++row;
      column = 0;
      insertRow ( row );
    }
    insertColorCell ( row, column++, vector[i] );
  }
  setMinimumHeight ( ( fixedSizeFactor * rowCount() ) );
  setMaximumHeight ( ( fixedSizeFactor * rowCount() ) );
  updateGeometries();
}

ColorTable::~ColorTable()
{}
