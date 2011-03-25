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

#include "keywordscombobox.h"

/* QtCore */
#include <QtCore/QDebug>

/* KDE */
#include <KDE/KIcon>

KeywordsComboBox::KeywordsComboBox ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "KeywordsComboBox" ) );
  setDuplicatesEnabled ( false );
  setEditable ( true );
  setInsertPolicy ( QComboBox::InsertAtCurrent );
  // Siehe updateItemList
  insertItem ( 0, QString(), QVariant ( false ) );
  setItemIcon ( 0, KIcon ( "edit-rename" ) );

  connect ( this, SIGNAL ( editTextChanged ( const QString & ) ),
            this, SLOT ( updateItemList ( const QString & ) ) );
}

void KeywordsComboBox::updateItemList ( const QString &text )
{
  int index = currentIndex();
  if ( text.isEmpty() && itemData ( index ).toBool() )
    removeItem ( index );
}

void KeywordsComboBox::setText ( const QString &txt )
{
  QStringList list = txt.split ( "," );
  for ( int i = 0; i < list.size(); ++i )
  {
    QString buffer = list.at ( i ).trimmed();
    if ( ! buffer.isEmpty() )
      addItem ( buffer, QVariant ( true ) );
  }
}

const QString KeywordsComboBox::text()
{
  QStringList list;
  for ( int i = 0; i < count(); ++i )
  {
    QString buffer = itemText ( i ).trimmed();
    if ( ! buffer.isEmpty() )
      list.append ( buffer );
  }
  return list.join ( "," );
}

KeywordsComboBox::~KeywordsComboBox()
{}
