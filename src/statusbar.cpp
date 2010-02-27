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

#include "statusbar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QFrame>

StatusBar::StatusBar ( QStatusBar * parent )
    : QStatusBar ( parent )
{
  setObjectName ( QLatin1String ( "statusbar" ) );

  // Display Browser ViewPort Width
  m_viewPortInfo = new QLabel ( this );
  m_viewPortInfo->setObjectName ( QLatin1String ( "viewportinfolabel" ) );
  m_viewPortInfo->setFrameShape ( QFrame::NoFrame );
  m_viewPortInfo->setToolTip ( trUtf8( "Display Browser Dimension Width x Height with Pixel." ) );
  m_viewPortInfo->setStatusTip ( trUtf8( "Display Browser Dimension Width x Height with Pixel." ) );
  m_viewPortInfo->setText ( QString() );
  insertPermanentWidget ( 0, m_viewPortInfo );

}

void StatusBar::displayBrowserWidth ( const QSize &s )
{
  QString w = QString::number( s.width() );
  QString h = QString::number( s.height() );
  QString txt = trUtf8 ( "Browser %1x%2 px" ).arg ( w, h );
  m_viewPortInfo->setText ( txt );
}

StatusBar::~StatusBar()
{
}
