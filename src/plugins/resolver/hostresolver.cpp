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

#include "hostresolver.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QHostAddress>

HostResolver::HostResolver ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "HostResolver" ) );
}

void HostResolver::prepareHostInfo ( const QHostInfo &host )
{
  QStringList info;
  QString content = QString ( "<b>Host:</b> %1<br/>" ).arg ( host.hostName() );
  foreach ( QHostAddress address, host.addresses() )
  {
    if ( address == QHostAddress::LocalHost )
      continue;

    if ( address.toIPv4Address() )
      content.append ( QString ( "<b>IPv4:</b> %1<br/>" ).arg ( address.toString() ) );
    else
      content.append ( QString ( "<b>IPv6:</b> %1<br/>" ).arg ( address.toString() ) );
  }
  QMessageBox::information ( 0x0, trUtf8 ( "Host Information" ), content );
}

void HostResolver::setDomain ( const QUrl &url )
{
  QHostInfo hostInfo = QHostInfo::fromName ( url.host() );
  prepareHostInfo ( hostInfo );
}

HostResolver::~HostResolver()
{}
