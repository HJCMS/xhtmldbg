/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "fontweight.h"

/* QtCore */
#include <QtCore/QDebug>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KDialog>
#include <KDE/KLocale>

FontWeight::FontWeight ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "FontWeightBox" ) );
  KIcon icon ( "list-add-font" );
  insertItem ( 0, icon, i18n ( "Thin" ), 100 );
  insertItem ( 1, icon, i18n ( "Extra Light (Ultra Light)" ), 200 );
  insertItem ( 2, icon, i18n ( "Light" ), 300 );
  insertItem ( 3, icon, i18n ( "Normal" ), 400 );
  insertItem ( 4, icon, i18n ( "Medium" ), 500 );
  insertItem ( 5, icon, i18n ( "Semi Bold (Demi Bold)" ), 600 );
  insertItem ( 6, icon, i18n ( "Bold" ), 700 );
  insertItem ( 7, icon, i18n ( "Extra Bold (Ultra Bold)" ), 800 );
  insertItem ( 8, icon, i18n ( "Black (Heavy)" ), 900 );
  setCurrentIndex ( 3 );

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( valueChanged ( int ) ) );
}

void FontWeight::valueChanged ( int index )
{
  QVariant value = itemData ( index, Qt::UserRole );
  emit weightChanged ( value );
}

void FontWeight::setNormal()
{
  setCurrentIndex ( 3 );
}

void FontWeight::setBold()
{
  setCurrentIndex ( 6 );
}

void FontWeight::setBolder()
{
  int v = itemData ( currentIndex(), Qt::UserRole ).toUInt();
  if ( ( v >= 100 ) && ( v <= 300 ) )
    setCurrentIndex ( 3 );
  else if ( ( v >= 400 ) && ( v <= 500 ) )
    setCurrentIndex ( 6 );
  else if ( ( v >= 600 ) && ( v <= 900 ) )
    setCurrentIndex ( 8 );
}

void FontWeight::setLighter()
{
  int v = itemData ( currentIndex(), Qt::UserRole ).toUInt();
  if ( ( v >= 100 ) && ( v <= 500 ) )
    setCurrentIndex ( 0 );
  else if ( ( v >= 600 ) && ( v <= 700 ) )
    setCurrentIndex ( 3 );
  else if ( ( v >= 800 ) && ( v <= 900 ) )
    setCurrentIndex ( 6 );
}

void FontWeight::setValue ( const QVariant &value )
{
  bool ok;
  int data = value.toUInt ( &ok );
  if ( ok )
  {
    switch ( data )
    {
      case 100:
        setCurrentIndex ( 0 );
        break;

      case 200:
        setCurrentIndex ( 1 );
        break;

      case 300:
        setCurrentIndex ( 2 );
        break;

      case 400:
        setCurrentIndex ( 3 );
        break;

      case 500:
        setCurrentIndex ( 4 );
        break;

      case 600:
        setCurrentIndex ( 5 );
        break;

      case 700:
        setCurrentIndex ( 6 );
        break;

      case 800:
        setCurrentIndex ( 7 );
        break;

      case 900:
        setCurrentIndex ( 8 );
        break;

      default:
        setCurrentIndex ( 3 );
        break;
    };
  }
  else
  {
    QString css = value.toString();
    if ( css.contains ( "bolder", Qt::CaseInsensitive ) )
      setBolder();
    else if ( css.contains ( "lighter", Qt::CaseInsensitive ) )
      setLighter();
    else if ( css.contains ( "bold", Qt::CaseInsensitive ) )
      setCurrentIndex ( 6 );
    else
      setCurrentIndex ( 3 );
  }
}

int FontWeight::qFontWeight()
{
  int index = currentIndex();
  if ( index == 2 )
    return QFont::Light;
  else if ( index == 3 )
    return QFont::Normal;
  else if ( index == 5 )
    return QFont::DemiBold;
  else if ( index == 6 )
    return QFont::Bold;
  else if ( index == 8 )
    return QFont::Black;
  else
    return QFont::Normal;
}

const QVariant FontWeight::value()
{
  return itemData ( currentIndex(), Qt::UserRole );
}

FontWeight::~FontWeight()
{}
