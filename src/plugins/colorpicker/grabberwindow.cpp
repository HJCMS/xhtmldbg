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

#include "grabberwindow.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QPoint>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QCursor>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
// #include <QtGui/QX11Info>

GrabberWindow::GrabberWindow ( QWidget * parent )
    : QWidget ( parent )
    , intervalId ( -1 )
    , fixedSize ( 100 )
    , penSize ( 1 )
    , zoomFactor ( 2 )
    , crossPen ( Qt::DashDotLine )
    , framePen ( Qt::SolidLine )
{
  setObjectName ( QLatin1String ( "grabberwindow" ) );
  setMouseTracking ( false ); // Wird nicht benötigt
  setMinimumWidth ( fixedSize );
  setMinimumHeight ( fixedSize );
  crossPen.setWidth ( penSize );
  crossPen.setBrush ( Qt::black );
  framePen.setWidth ( penSize );
  framePen.setBrush ( Qt::black );
}

/**
* Timer für @ref record
*/
void GrabberWindow::timerEvent ( QTimerEvent * event )
{
  if ( event->timerId() == intervalId )
    listener();
}

/**
* @ref m_bufferPixmap auslesen und Hintergrund neu Zeichnen
*/
void GrabberWindow::paintEvent ( QPaintEvent * )
{
  QPainter p ( this );
  p.save();
  p.scale ( zoomFactor, zoomFactor );
  p.drawPixmap ( 0, 0, m_bufferPixmap );
  qreal f = static_cast<qreal> ( ( 1 / zoomFactor ) );
  p.scale ( f, f );
  p.restore();
  p.setPen ( crossPen );
  p.drawLine ( ( fixedSize / 2 ), 0, ( fixedSize / 2 ), fixedSize );
  p.drawLine ( 0, ( fixedSize / 2 ), fixedSize, ( fixedSize / 2 ) );
  p.setPen ( framePen );
  p.drawRect ( 0, 0, ( fixedSize - penSize ), ( fixedSize - penSize ) );
}

/**
* Dimensionen der Maus abgreifen.
*/
void GrabberWindow::listener()
{
  int f = qRound ( fixedSize / 2 );
  QPoint mousePos = QCursor::pos();
  int x = mousePos.x() - ( f / 2 );
  int y = mousePos.y() - ( f / 2 );
  m_bufferPixmap = QPixmap::grabWindow ( qApp->desktop()->winId(), x, y, f, f );
  update();
}

/**
* Aufnahme Starten/Stoppen und timerId setzen.
*/
void GrabberWindow::startRecording ( bool b )
{
  if ( b )
    intervalId = startTimer ( 30 );
  else
    killTimer ( intervalId );
}

GrabberWindow::~GrabberWindow()
{
  startRecording ( false );
}
