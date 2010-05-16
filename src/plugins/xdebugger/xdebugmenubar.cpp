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

#include "xdebugmenubar.h"
#include "ledlabel.h"

/* QtCore */
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QPushButton>

XDebugMenuBar::XDebugMenuBar ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "debugclient" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QHBoxLayout* hLayout = new QHBoxLayout ( this );
  hLayout->setContentsMargins ( 5, 0, 5, 0 );
  hLayout->setSpacing ( 5 );

  m_ledLabel = new LedLabel ( this );
  hLayout->addWidget ( m_ledLabel );

  hLayout->addStretch ( 1 );

  QPushButton* btnStart = new QPushButton ( this );
  btnStart->setText ( "Start" );
  btnStart->setIcon ( QIcon::fromTheme ( QLatin1String ( "start-here" ) ) );
  btnStart->setToolTip ( "Start TCP Server" );
  hLayout->addWidget ( btnStart );

  QPushButton* btnStop = new QPushButton ( this );
  btnStop->setText ( "Stop" );
  btnStop->setIcon ( QIcon::fromTheme ( QLatin1String ( "process-stop" ) ) );
  btnStop->setToolTip ( "Stop TCP Server" );
  hLayout->addWidget ( btnStop );

//   QPushButton* btnSend = new QPushButton ( this );
//   btnSend->setText ( "TEST" );
//   btnSend->setIcon ( QIcon::fromTheme ( QLatin1String ( "process-working" ) ) );
//   btnSend->setToolTip ( "Send Message" );
//   hLayout->addWidget ( btnSend );

  setLayout ( hLayout );

  // Signals
  connect ( btnStart, SIGNAL ( clicked() ),
            this, SIGNAL ( startServer() ) );

  connect ( btnStop, SIGNAL ( clicked() ),
            this, SIGNAL ( shutdownServer() ) );

//   connect ( btnSend, SIGNAL ( clicked() ),
//             this, SIGNAL ( sendCommand() ) );
}

void XDebugMenuBar::setListenerLed ( bool b )
{
  m_ledLabel->setOn ( b );
}

XDebugMenuBar::~XDebugMenuBar()
{}
