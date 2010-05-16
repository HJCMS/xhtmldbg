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

#include "xdebugserver.h"
#include "xdebugclient.h"

#include <cstdlib>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QString>

/* QtNetwork */
#include <QtNetwork/QLocalSocket>

XDebugServer::XDebugServer ( QObject * parent )
    : QTcpServer ( parent )
    , timeOut ( ( 30 * 1000 ) )
{
  setObjectName ( QLatin1String ( "xdebugserver" ) );

  /* Die Maximale Bitfeldgröße der Betriebssysteme ist unterschiedlich,
  * dadurch weichen auch die Angaben der Descriptoren stark ab,
  * zum Beispiel Linux ca. 1024 und Windows nur ca. 64 Descriptoren.
  * Wir benötigen aber ohnehin nicht so viele Klienten deshalb
  * setze wir beim start die Anzahl auf Max 5.
  */
  setMaxPendingConnections ( 5 );

  /* Setze das Time Out auf 30 Sekunden
  * 1 Sekunde = 1000 Millisekunden (SI-Einheitensystem) */
  waitForNewConnection ( timeOut );
}

void XDebugServer::incomingConnection ( int descriptor )
{
  XDebugClient* m_socket = new XDebugClient ( this );
  m_socket->setSocketDescriptor ( descriptor );
  emit newConnection ( m_socket );
}

XDebugServer::~XDebugServer()
{
  if ( isListening() )
    close();
}
