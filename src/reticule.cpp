/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "reticule.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QPainter>
#include <QtGui/QPen>

/**
* Mit dieser Klasse werden HTML Elemente hervorgehoben!
* Wenn in @class DomInspector das signal @ref DomInspector::highlight
* gesendet wird zeichnet @class Reticule einen Rahmen oder ein GitterNetz
* um das entsprechende Element.
*/
Reticule::Reticule ( QWidget * parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
{
  setObjectName ( QLatin1String ( "reticule" ) );
  setAutoFillBackground ( false );
  setContentsMargins ( 0, 0, 0, 0 );
  setAttribute ( Qt::WA_NoBackground, true );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_PaintOnScreen, true );

  m_settings = new Settings ( this );

  /*
  enableHighlightBackground=true
  enableHighlightBorder=false
  highlightColor=yellow
  highlightBorder=red
  */
}

/**
* Lege die aktuelle Hervorhebung fest
*/
const QBrush Reticule::currentBrush()
{
  QBrush brush;
  brush.setStyle ( Qt::SolidPattern );
  brush.setColor ( Qt::red );
  return brush;
}

/**
* Hier wird mit den Einstellungen von m_settings
* der rahmen oder das Netz gezeichnet.
*/
void Reticule::paintEvent ( QPaintEvent * event )
{
  QPainter painter ( this );
  painter.fillRect ( event->rect(), currentBrush() );
  painter.beginNativePainting();
  painter.endNativePainting();
}

/**
* Wird von @class WebView aufgerufen um das Element
* hervor zu heben.
*/
void Reticule::setRegion ( const QRect &r )
{
  if ( isVisible() )
    setGeometry ( r );
}

Reticule::~Reticule()
{}
