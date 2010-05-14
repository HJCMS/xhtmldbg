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
  // 1 Sekunde = 1000 Millisekunden (SI-Einheitensystem)
  m_timer->setInterval ( 1000 );
  connect ( m_timer, SIGNAL ( timeout() ), this, SLOT ( timeStamp() ) );
}

/**
* Die Intervalzeit ist auf eine Sekunde eingestellt.
* Diese Methode erhöht bei jeden Aufruf den
* Inkrementalgeber @ref incrementer  um eine Sekunde.
* Ist der Wert von @ref reloadInSeconds erreicht wird
* das signal @ref reload abgestoßen und der
* Inkrementalgeber wieder auf 0 gesetzt.
* Gleichzeitig erfolgt bei jedem Aufruf das signal @ref status
* bei dem der Entpunkt-Faktor @ref reloadInSeconds und der
* Aktuelle Inkrementalgeber Status gesendet werden.
*/
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

/**
* Stoppe alle laufenden Zeitgeber und nehme
* die Intervalzeit in Sekunden entgegen.
* Setze den Inkrementalgeber @ref incrementer
* auf 0 und @ref reloadInSeconds erhält den neuen
* Wert mit dem der Zeitgeber neu gestartet wird.
* Wenn die Überreichte Zeit unterhalb von 5 liegt,
* wird der Zeitgeber abgeschaltet.
*/
void AutoReloader::setInterval ( int sek )
{
  if ( m_timer->isActive() )
    m_timer->stop();

  if ( sek < 5 )
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
