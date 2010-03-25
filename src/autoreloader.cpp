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

#include "autoreloader.h"

#include <unistd.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

AutoReloader::AutoReloader ( QObject * parent )
    : QObject ( parent )
    , reloadInSeconds ( 0 )
    , incrementer ( 0 )
{
  setObjectName ( QLatin1String ( "autoreloader" ) );
  m_timer = new QTimer ( this );
  m_timer->setInterval ( 1000 );
  connect ( m_timer, SIGNAL ( timeout() ), this, SLOT ( timeStamp() ) );
}

void AutoReloader::timeStamp()
{
  ++incrementer;
  if ( reloadInSeconds <= incrementer )
  {
    emit reload();
    incrementer = 0;
  }
  emit status ( reloadInSeconds, incrementer );
}

void AutoReloader::setInterval ( int sek )
{
  if ( m_timer->isActive() )
    m_timer->stop();

  if ( sek < 15 )
  {
    emit status ( 0, 0 );
    return;
  }

  reloadInSeconds = sek;
  incrementer = 0;
  m_timer->start ();
}

AutoReloader::~AutoReloader()
{
  if ( m_timer->isActive() )
    m_timer->stop();
}
