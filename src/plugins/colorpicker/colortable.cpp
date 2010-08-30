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
    , fixedColumns ( 20 )
{
  setObjectName ( QLatin1String ( "colortable" ) );
  setFrameShape ( QFrame::NoFrame );
  setAutoScroll ( false );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setTabKeyNavigation ( false );
  setProperty ( "showDropIndicator", QVariant ( false ) );
  setDragDropOverwriteMode ( false );
  setWordWrap ( false );
  setCornerButtonEnabled ( false );
  setColumnCount ( fixedColumns );

  horizontalHeader()->setVisible ( false );
  horizontalHeader()->setDefaultSectionSize ( 20 );
  horizontalHeader()->setHighlightSections ( false );
  horizontalHeader()->setMinimumSectionSize ( 20 );

  verticalHeader()->setVisible ( false );
  verticalHeader()->setDefaultSectionSize ( 20 );
  verticalHeader()->setHighlightSections ( false );
  verticalHeader()->setMinimumSectionSize ( 20 );
}

void ColorTable::insertColorCell ( int row, int column, const QColor &color )
{
  // qDebug() << Q_FUNC_INFO << row << column << color;
  QTableWidgetItem* item = new QTableWidgetItem ( QTableWidgetItem::UserType );
  item->setBackground ( QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::BackgroundRole , QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::BackgroundColorRole , QBrush ( color, Qt::SolidPattern ) );
  item->setData ( Qt::UserRole, color );
  item->setData ( Qt::DisplayRole, color.name() );
  QString tip = QString ( "rgb(%1,%2,%3)" ).arg (
                    QString::number ( color.red() ),
                    QString::number ( color.green() ),
                    QString::number ( color.blue() )
                );
  item->setData ( Qt::ToolTipRole, tip );
  item->setData ( Qt::TextAlignmentRole, Qt::AlignCenter );

  // TODO Farben Unterschiede erkennen
//   if ( qMin( color.green(), color.blue() ) == 0 )
//     item->setData ( Qt::TextColorRole, QBrush( QColor(255,255,255), Qt::SolidPattern ) );

  setItem ( row, column, item );
}

/**
* Farbtabelle neu befüllen
*/
void ColorTable::insertColors ( const QVector<QColor> &vector )
{
  int row = 0;
  int column = 0;
  if ( vector.size() < fixedColumns )
    return;

  clearContents ();
  setRowCount ( 1 );
  for ( int i = 0; i < vector.size(); ++i )
  {
    if ( ( column >= fixedColumns ) && ( i % fixedColumns == 0 ) )
    {
      ++row;
      column = 0;
      insertRow ( row );
    }
    insertColorCell ( row, column++, vector[i] );
  }
  resizeRowsToContents();
  resizeColumnsToContents();
}

ColorTable::~ColorTable()
{}
