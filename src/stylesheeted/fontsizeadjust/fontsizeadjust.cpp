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

#include "fontsizeadjust.h"

/* QtCore */
#include <QtCore/QString>

FontSizeAdjust::FontSizeAdjust ( QWidget * parent )
    : KDoubleNumInput ( parent )
{
  setObjectName ( QLatin1String ( "FontSizeAdjust" ) );
  setRange ( 0.1, 1.0 );
  setValue ( 0.1 );
  setSingleStep ( 0.1 );
  setDecimals ( 1 );
  setSliderEnabled ( true );
}

FontSizeAdjust::~FontSizeAdjust()
{}
