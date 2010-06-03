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

#include "xdebugsocket.h"
#include "qbytearrayfifo.h"
#include "xdebugprojectitem.h"
#include "xdebugclient.h"
#include "xdebugserver.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

/* QtNetwork */
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

static const quint16 localPort = 9000;

XDebugSocket::XDebugSocket ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "xdebugsocket" ) );

  m_Timer = new QTimer ( this );
  m_Timer->setInterval ( ( 2 * 1000 ) );

  m_server = new XDebugServer ( this );

  m_udpSocket = new QUdpSocket ( this );

  connect ( m_server, SIGNAL ( newConnection ( XDebugClient * ) ),
            this, SLOT ( steppingIn ( XDebugClient * ) ) );

  connect ( m_Timer, SIGNAL ( timeout() ),
            this, SLOT ( checkStatus() ) );

  connect ( m_udpSocket, SIGNAL ( readyRead() ),
            this, SLOT ( readPendingDatagrams() ) );
}

/**
* Wenn eine Antwort eingeht diese hier auslesen!
*/
void XDebugSocket::steppingIn ( XDebugClient * socket )
{
  int bytes;
  long m_datalen = -1;
  QString data;
  QByteArrayFifo m_fifo;
  if ( socket->waitForReadyRead () )
  {
    while ( socket->bytesAvailable() > 0 || m_fifo.length() >= static_cast<unsigned long> (m_datalen) )
    {
      if ( socket->bytesAvailable() > 0 )
      {
        bytes = socket->bytesAvailable();
        char* buffer = new char[bytes];
        socket->read ( buffer, bytes );

        m_fifo.append ( buffer, bytes );

        delete[] buffer;
      }
      // qDebug() << __LINE__;
      while ( 1 )
      {
        if ( m_datalen == -1 )
        {
          bytes = m_fifo.find ( '\0' );
          if ( bytes < 0 )
            break;

          data = m_fifo.retrieve();
          m_datalen = data.toLong();

        }
        if ( m_datalen != -1 && static_cast<long> ( m_fifo.length() ) >= m_datalen + 1 )
        {
          data = m_fifo.retrieve();
          m_datalen = -1;
          emit dataChanged ( data );
        }
        else
          break;
      }
    }
  }
  /*
  * FIXME So lange die Kommunikation mit dem Xdebug Socket nicht
  * richtig funktioniert. Macht es keinen sinn die Verbindung
  * an dieser stelle aufrecht zu erhalten.
  * Es würde sich nur die Komplette IDE Aufhängen :-/
  */
  socket->disconnectFromHost();
}

void XDebugSocket::readPendingDatagrams ()
{
  while ( m_udpSocket->hasPendingDatagrams() )
  {
    QByteArray data;
    data.resize ( m_udpSocket->pendingDatagramSize() );
    QHostAddress ip;
    quint16 port;
    m_udpSocket->readDatagram ( data.data(), data.size(), &ip, &port );

    if ( port == m_server->serverPort() )
    {
      XDebugClient* socket = new XDebugClient ( this );
      connect ( socket, SIGNAL ( readyForDebugging ( XDebugClient * ) ),
                this, SLOT ( steppingIn ( XDebugClient * ) ) );
      connect ( socket, SIGNAL ( clientStatus ( const QString & ) ),
                this, SIGNAL ( statusMessage ( const QString & ) ) );

      socket->connectToHost ( ip, port );
      if ( socket->waitForConnected ( 500 ) )
        socket->push ( data );

      socket->disconnectFromHost();
      qDebug() << Q_FUNC_INFO << data;
    }
  }
}

void XDebugSocket::checkStatus ()
{
  emit connected ( m_server->isListening() );
}

/**
* Starte eine Session mit den Projekt Parametern.
*/
void XDebugSocket::start ( XDebugProjectItem * proj )
{
  if ( m_server->isListening() )
  {
    emit statusMessage ( trUtf8 ( "XDebugger is busy!" ) );
    return;
  }

  QString ms;
  bool b = false;
  uint port = proj->getPort().toUInt ( &b, 10 );
  if ( ! b )
    port = 9000;

  QHostAddress ip ( proj->getRemoteHost() );

  if ( m_server->listen ( ip, port ) )
  {
    ms = trUtf8 ( "XDebugger startet at: %1:%2\nWaiting for debug server to connect..." )
         .arg ( proj->getRemoteHost(), proj->getPort() );

    // Binde den Lokalen UDP Server an den TCP Server
    QUdpSocket::BindMode mode = ( QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint );
    b = m_udpSocket->bind ( m_server->serverAddress(), localPort, mode );
  }
  else
  {
    ms = trUtf8 ( "Unable to start the XDebugger!\n%1" ).arg ( m_server->errorString() );
    b = false;
  }

  if ( b )
    m_Timer->start();

  emit statusMessage ( ms );
  emit connected ( b );
}

/**
* Wenn der Server am lauschen ist dann runterfahren.
*/
void XDebugSocket::stop ()
{
  if ( m_server->isListening() )
    m_server->close();

  if ( m_Timer->isActive() )
    m_Timer->stop();

  emit connected ( false );
  emit statusMessage ( trUtf8 ( "XDebugger shutdown..." ) );
}

bool XDebugSocket::sendMessage ( const QByteArray &mess )
{
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
  if ( ! m_server->isListening() )
    return false;

  m_udpSocket->writeDatagram ( mess.data(), m_server->serverAddress(),
                               m_server->serverPort() );
  return true;
}

XDebugSocket::~XDebugSocket()
{
  if ( m_Timer->isActive() )
    m_Timer->stop();
}
