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
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

StatusBar::StatusBar ( QStatusBar * parent )
    : QStatusBar ( parent )
{
  setObjectName ( QLatin1String ( "statusbar" ) );
  setContentsMargins ( 2, 5, 2, 2 );


  QIcon cacheIcon = QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cache" ) );
  QPixmap pic = cacheIcon.pixmap ( 16, QIcon::Disabled, QIcon::On );

  // Display Browser ViewPort Width
  m_viewNetworkInfo = new QLabel ( this );
  m_viewNetworkInfo->setObjectName ( QLatin1String ( "viewportinfolabel" ) );
  m_viewNetworkInfo->setFrameShape ( QFrame::NoFrame );
  m_viewNetworkInfo->setContentsMargins ( 5, 2, 5, 2 );
  m_viewNetworkInfo->setPixmap ( pic );
  m_viewNetworkInfo->setScaledContents ( true );
  m_viewNetworkInfo->setToolTip ( trUtf8 ( "Display Information about to read from Cache or not." ) );
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
  QString infotxt;
  QIcon::Mode mode;
  QIcon cacheIcon = QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cache" ) );
  if ( b )
  {
    infotxt = trUtf8 ( "Read rendered Browser Source (no cache availably)" );
    mode = QIcon::Normal;
  }
  else
  {
    infotxt = trUtf8 ( "Read Source from Cache" );
    mode = QIcon::Disabled;
  }
  m_viewNetworkInfo->setPixmap ( cacheIcon.pixmap ( 16, mode ) );
  m_viewNetworkInfo->setStatusTip ( infotxt );
  showMessage ( infotxt, 1024 );
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
