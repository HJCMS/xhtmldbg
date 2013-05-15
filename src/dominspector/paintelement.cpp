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

#include "paintelement.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>

PaintElement::PaintElement ( QWidget * parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
{
  setObjectName ( QLatin1String ( "PaintElement" ) );
  setUpdatesEnabled ( true );
  setAutoFillBackground ( false );
  setContentsMargins ( 0, 0, 0, 0 );
  setAttribute ( Qt::WA_NoBackground, true );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_PaintOnScreen, false );
}

void PaintElement::initStyleOption ( QStyleOptionRubberBand * option ) const
{
  if ( option )
    option->initFrom ( this );
}

void PaintElement::paintEvent ( QPaintEvent * event )
{
  QPainter painter ( this );
  painter.setBrush ( Qt::red );
  painter.setBackgroundMode ( Qt::TransparentMode );
  painter.setPen ( Qt::NoPen );
  painter.drawRect ( event->rect() );
}

PaintElement::~PaintElement()
{}
