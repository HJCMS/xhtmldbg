/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "hostinfo.h"
#include "hostinfodialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSize>

/* QtNetwork */
#include <QtNetwork/QHostAddress>

HostInfo::HostInfo ( QWidget * parent )
    : QProcess ( parent )
{
  setObjectName ( QLatin1String ( "HostInfo" ) );
  setProcessChannelMode ( QProcess::SeparateChannels );

  connect ( this, SIGNAL ( readyReadStandardOutput() ),
            this, SLOT ( readStandardOutput() ) );

  connect ( this, SIGNAL ( readyReadStandardError() ),
            this, SLOT ( readErrorOutput() ) );

  connect ( this, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );
}

/**
* Standard Ausgabe von dig einlesen!
**/
void HostInfo::readStandardOutput()
{
  QStringList data;
  setReadChannel ( QProcess::StandardOutput );
  QString buffer = readAll();
  if ( buffer.length() > 1 )
    data << buffer.split ( QRegExp ( "[\\n\\r]" ) );

  closeReadChannel ( QProcess::StandardOutput );

  if ( data.size() > 0 )
    emit listData ( data );
}

/**
* Fehler Ausgabe von dig im Terminal ausgeben
**/
void HostInfo::readErrorOutput()
{
  QByteArray buffer;
  setReadChannel ( QProcess::StandardError );
  buffer = readAll();
  closeReadChannel ( QProcess::StandardError );

  if ( buffer.size() > 1 )
    qWarning ( "(XHTMLDBG) Hostinfo Read Error: %s", buffer.constData() );

  terminate();
}

/**
* Wenn der Prozess sauber beendet wurde hier das Signal
* @ref closedNormal abstoßen.
**/
void HostInfo::exited ( int exitCode, QProcess::ExitStatus stat )
{
  Q_UNUSED ( exitCode )

  switch ( stat )
  {
    case QProcess::NormalExit:
      emit closedNormal ();
      break;

    case QProcess::CrashExit:
      break;

    default:
      return;
  }
}

/**
* Zurerst die Adressen Informationen für @ref labelData generieren!
* Danach den Befehl für "dig" Aufbereiten und den Prozess starten!
**/
void HostInfo::dig ( const QHostInfo &host )
{
  QString content = QString ( "<b>Host:</b> %1<br/>" ).arg ( host.hostName() );
  foreach ( QHostAddress address, host.addresses() )
  {
    if ( address.isNull() )
      continue; // Kein INADDR_ANY oder in6addr_any anzeigen.

    if ( address.toIPv4Address() )
    {
      content.append ( QString ( "<b>IPv4:</b> %1<br/>" ).arg ( address.toString() ) );
    }
    else
    {
      content.append ( QString ( "<b>IPv6:</b> %1<br/>" ).arg ( address.toString() ) );
    }
  }

  if ( ! content.isEmpty() )
    emit labelData ( content );

  // Zuerst eine Bind Anfrage senden
  QStringList args;
  args << "-x" << "-b" << host.hostName();
  start ( "dig", args, QIODevice::ReadWrite );
}

HostInfo::~HostInfo()
{}
