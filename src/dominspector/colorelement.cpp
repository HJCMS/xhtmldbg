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

#include "colorelement.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtDBus */
#include <QtDBus/QDBusConnection>

ColorElement::ColorElement ( const QWebElement &element, Type type, QWidget * parent )
    : KColorDialog ( parent )
    , colorType ( type )
    , htmlElement ( element )
{
  setObjectName ( QLatin1String ( "ColorElement" ) );

  connect ( this, SIGNAL ( colorSelected ( const QColor & ) ),
            this, SLOT ( manipBackground ( const QColor & ) ) );
}

void ColorElement::manipBackground ( const QColor &color )
{
  switch ( colorType )
  {
    case FOREGROUND:
      htmlElement.setStyleProperty ( "color", color.name() );
      break;

    default:
      htmlElement.setStyleProperty ( "background-color", color.name() );
      break;
  };
}

ColorElement::~ColorElement()
{}
