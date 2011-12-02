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

#include "colorpreviewer.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QSizePolicy>

ColorPreviewer::ColorPreviewer ( QWidget * parent )
    : QWidget ( parent )
    , p_brush ( QBrush ( Qt::TextColorRole, Qt::CrossPattern ) )
    , p_pen ( QBrush ( Qt::TextColorRole, Qt::SolidPattern ), 1.0, Qt::SolidLine, Qt::FlatCap )
    , changedColor ( Qt::white )
{
  setObjectName ( QLatin1String ( "ColorPreviewer" ) );
  setBackgroundRole ( QPalette::NoRole );
  setContentsMargins ( 0, 0, 0, 0 );
  setAttribute ( Qt::WA_NoBackground, true );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_PaintOnScreen, true );
  setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
}

void ColorPreviewer::paintEvent ( QPaintEvent * event )
{
  QPainter painter ( this );
  painter.setBrush ( p_brush );
  painter.setBackgroundMode ( Qt::TransparentMode );
  painter.setPen ( p_pen );
  painter.drawRect ( event->rect() );
  painter.fillRect ( event->rect(), changedColor );
}

void ColorPreviewer::preview ( const QColor &col )
{
  changedColor = col;
}

ColorPreviewer::~ColorPreviewer()
{}
