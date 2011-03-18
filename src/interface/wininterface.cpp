/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "wininterface.h"

#include <QtCore/QRegExp>

#define IFACE QString::fromUtf8("de.hjcms.xhtmldbg")

namespace xhtmldbg
{

  WinInterface::WinInterface ( const QDBusConnection &dbus, QObject* parent )
      : QDBusInterface ( "de.hjcms.xhtmldbg", "/xhtmldbg", IFACE, dbus, parent )
      , interface ( IFACE ), bus ( dbus )
  {
    setObjectName ( "de.hjcms.xhtmldbg" );
  }

  void WinInterface::hasErrors ()
  {
    QDBusError err = lastError();
    if ( err.isValid() )
      emit error ( err.name(), err.message() );
  }

  bool WinInterface::isValid ( const QUrl &url ) const
  {
    return url.scheme().contains ( QRegExp ( "^(http[s?]|file)$" ) );
  }

  void WinInterface::noticeMessage ( const QString &message )
  {
    if ( message.isEmpty() )
      return;

    call ( "setApplicationMessage", message, false );
    hasErrors();
  }

  void WinInterface::errorMessage ( const QString &message )
  {
    if ( message.isEmpty() )
      return;

    call ( "setApplicationMessage", message, true );
    hasErrors();
  }

  void WinInterface::open ( const QUrl &url, bool addtab )
  {
    if ( ! isValid ( url ) )
      return;

    call ( "openUrl", url, addtab );
    hasErrors();
  }

  void WinInterface::setUrl ( const QUrl &oldUrl, const QUrl &newUrl )
  {
    if ( ! isValid ( oldUrl ) || ! isValid ( newUrl ) )
      return;

    call ( "setPageUrl", oldUrl, newUrl );
    hasErrors();
  }

  void WinInterface::setSource ( const QUrl &url, const QString &src )
  {
    if ( ! isValid ( url ) || src.isEmpty() )
      return;

    call ( "setSource", url, src );
    hasErrors();
  }

  WinInterface::~WinInterface()
  {
    if ( bus.isConnected() )
      QDBusConnection::disconnectFromBus ( bus.name() );
  }

} // eof namespace
