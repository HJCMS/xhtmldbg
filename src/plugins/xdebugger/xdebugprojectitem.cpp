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

#include "xdebugprojectitem.h"

#include <QtCore/QDebug>

XDebugProjectItem::XDebugProjectItem ()
    : Port ( QLatin1String ( "9000" ) )
    , Name ( QLatin1String ( "xhtmldbg" ) )
    , RemoteHost ( QLatin1String ( "localhost" ) )
    , SessionId ( QLatin1String ( "xhtmldbg" ) )
{}

XDebugProjectItem::XDebugProjectItem ( const XDebugProjectItem &other )
    : Port ( other.Port )
    , Name ( other.Name )
    , RemoteHost ( other.RemoteHost )
    , SessionId ( other.SessionId )
{}

void XDebugProjectItem::setPort ( const QString &str )
{
  uint p = str.toUInt();
  if ( p >= 1024 && p <= 65535 )
    Port = str;
}

void XDebugProjectItem::setName ( const QString &str )
{
  if ( str.isEmpty() )
    return;

  Name = str;
}

void XDebugProjectItem::setRemoteHost ( const QString &str )
{
  if ( str.isEmpty() )
    return;

  RemoteHost = str;
}

void XDebugProjectItem::setSessionId ( const QString &str )
{
  if ( str.isEmpty() )
    return;

  SessionId = str;
}

const QString XDebugProjectItem::getPort()
{
  return Port;
}

const QString XDebugProjectItem::getName()
{
  return Name;
}

const QString XDebugProjectItem::getSessionId()
{
  return SessionId;
}

const QString XDebugProjectItem::getRemoteHost()
{
  return RemoteHost;
}

XDebugProjectItem::~XDebugProjectItem()
{}
