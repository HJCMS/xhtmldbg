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

#include "liststylesheet.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui/QListWidgetItem>

ListStyleSheet::ListStyleSheet ( QWidget * parent )
    : QListWidget ( parent )
{
  setObjectName ( QLatin1String ( "liststylesheet" ) );

  setSortingEnabled ( true );
  setWordWrap ( false );
  sortItems ( Qt::AscendingOrder );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setBackgroundRole ( QPalette::AlternateBase );
  setAlternatingRowColors ( true );
  setContextMenuPolicy ( Qt::NoContextMenu );
  setMinimumHeight ( 80 );

  QFile fp ( QString::fromUtf8 ( ":/css.list" ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QString attributesList ( fp.readAll() );
    fp.close();
    foreach ( QString p, attributesList.split ( "\n" ) )
    {
      if ( ! p.isEmpty() )
        cssAttributes.append ( p );
    }
  }

}

void ListStyleSheet::setStyleSheetList ( const QWebElement &element )
{
  clear();
  foreach ( QString attr, cssAttributes )
  {
    QString value = element.styleProperty ( attr, QWebElement::CascadedStyle );
    if ( value.isEmpty() )
      continue;

    addItem ( QString ( attr + ": " + value + ";" ) );
  }
}

ListStyleSheet::~ListStyleSheet()
{}
