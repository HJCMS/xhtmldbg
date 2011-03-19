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

#include "hexinput.h"

HexInput::HexInput ( QWidget * parent )
    : QLineEdit ( parent )
    , minHex ( QRegExp ( "^#([0-9a-fA-F]{3})$" ) )
    , maxHex ( QRegExp ( "^#([0-9a-fA-F]{6})$" ) )
    , highlightStyle ( QLatin1String ( "*{background-color:#FFFFCC;}" ) )
{
  setObjectName ( QLatin1String ( "HexInput" ) );
  validator = new QRegExpValidator ( QRegExp ( "^#([0-9a-fA-F]{3,6})$" ), this );
  setValidator ( validator );
  connect ( this, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( colorHexValidate ( const QString & ) ) );
}

void HexInput::colorHexValidate ( const QString &hex )
{
  if ( hex.isEmpty() )
    setStyleSheet ( QString::null );
  else if ( hex.length() < 4 )
    setStyleSheet ( highlightStyle );
  else if ( hex.contains ( minHex ) || hex.contains ( maxHex ) )
    setStyleSheet ( QString::null );
  else
    setStyleSheet ( highlightStyle );
}

HexInput::~HexInput()
{
  clear();
}
