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

#include "xdebugclient.h"

/* QtCore */
#include <QtCore/QIODevice>
#include <QtCore/QString>

XDebugClient::XDebugClient ( QObject * parent )
    : QTcpSocket ( parent )
{
  setObjectName ( QLatin1String ( "xdebugclient" ) );
  setOpenMode ( QIODevice::ReadWrite );

  connect ( this, SIGNAL ( readyRead () ), this, SLOT ( readyForReading () ) );

  connect ( this, SIGNAL ( error ( QAbstractSocket::SocketError ) ),
            this, SLOT ( socketError ( QAbstractSocket::SocketError ) ) );

  connect ( this, SIGNAL ( stateChanged ( QAbstractSocket::SocketState ) ),
            this, SLOT ( socketState ( QAbstractSocket::SocketState ) ) );

  // connect ( this, SIGNAL ( disconnected () ), this, SLOT ( deleteLater () ) );
}

void XDebugClient::readyForReading()
{
  emit readyForDebugging ( this );
}

bool XDebugClient::push ( const QByteArray &data )
{
  write ( data );
  if ( flush() )
  {
    waitForBytesWritten ( 500 );
    return true;
  }
  return false;
}

void XDebugClient::socketError ( QAbstractSocket::SocketError err )
{
  QString status;
  switch ( err )
  {
    case QAbstractSocket::RemoteHostClosedError:
      status = errorString();
      break;

    case QAbstractSocket::UnsupportedSocketOperationError:
      status = errorString();
      break;

    case QAbstractSocket::HostNotFoundError:
      status = errorString();
      break;

    case QAbstractSocket::ConnectionRefusedError:
      status = errorString();
      break;

    default:
      status = trUtf8 ( "Error occurred:\n%1" ).arg ( errorString() );
      break;
  }

  if ( ! status.isEmpty() )
    emit clientStatus ( status );

#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG_VERBOSE
  qWarning ( "(XHTMLDBG) Client Socket Error -- %s", qPrintable ( status ) );
#endif
}

void XDebugClient::socketState ( QAbstractSocket::SocketState state )
{
  QString status;
  switch ( state )
  {
    case QAbstractSocket::UnconnectedState:
      status = "Unconnected";
      break;

    case QAbstractSocket::HostLookupState:
      status = "HostLookup";
      break;

    case QAbstractSocket::ConnectingState:
      status = "Connecting";
      break;

    case QAbstractSocket::ConnectedState:
      status = "Connected";
      break;

    case QAbstractSocket::BoundState:
      status = "Bound";
      break;

    case QAbstractSocket::ClosingState:
      status = "Closing";
      break;

    case QAbstractSocket::ListeningState:
      status = "Listening";
      break;

    default:
      return;
  }
#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG_VERBOSE
  qWarning ( "(XHTMLDBG) Client Socket State -- %s", qPrintable ( status ) );
#endif
}

XDebugClient::~XDebugClient()
{}
