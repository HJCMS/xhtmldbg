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
  setContentsMargins ( 2, 5, 2, 2 );

  // Display Browser ViewPort Width
  m_viewNetworkInfo = new QLabel ( this );
  m_viewNetworkInfo->setObjectName ( QLatin1String ( "viewportinfolabel" ) );
  m_viewNetworkInfo->setFrameShape ( QFrame::NoFrame );
  m_viewNetworkInfo->setContentsMargins ( 5, 2, 5, 2 );
  m_viewNetworkInfo->setText ( trUtf8 ( "Read Source from Cache" ) );
  QString info0 = trUtf8 ( "Display Information about the Network Manager works." );
  m_viewNetworkInfo->setToolTip ( info0 );
  m_viewNetworkInfo->setStatusTip ( info0 );
  insertPermanentWidget ( 0, m_viewNetworkInfo );

  // Display Browser ViewPort Width
  m_viewPortInfo = new QLabel ( this );
  m_viewPortInfo->setObjectName ( QLatin1String ( "viewportinfolabel" ) );
  m_viewPortInfo->setFrameShape ( QFrame::NoFrame );
  m_viewPortInfo->setContentsMargins ( 5, 2, 5, 2 );
  QString info1 = trUtf8 ( "Display Browser Dimension Width x Height with Pixel." );
  m_viewPortInfo->setToolTip ( info1 );
  m_viewPortInfo->setStatusTip ( info1 );
  insertPermanentWidget ( 1, m_viewPortInfo );
}

void StatusBar::slotWarningNoCache ( bool b )
{
  if ( b )
  {
    m_viewNetworkInfo->setText ( trUtf8 ( "(No Cache) Source is from WebKit" ) );
    m_viewNetworkInfo->setForegroundRole ( QPalette::LinkVisited );
  }
  else
  {
    m_viewNetworkInfo->setText ( trUtf8 ( "Read Source from Cache" ) );
    m_viewNetworkInfo->setForegroundRole ( QPalette::Text );
  }
}

void StatusBar::displayBrowserWidth ( const QSize &s )
{
  QString w = QString::number ( s.width() );
  QString h = QString::number ( s.height() );
  QString txt = trUtf8 ( "Browser %1x%2 px" ).arg ( w, h );
  m_viewPortInfo->setText ( txt );
}

StatusBar::~StatusBar()
{
}
