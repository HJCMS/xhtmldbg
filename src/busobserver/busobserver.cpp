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

#include "busobserver.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* KDE */
#include <KDE/KLocale>

static const QString _dbus_service()
{
  return QString::fromUtf8 ( "de.hjcms.xhtmldbg" );
}

BusObserver::BusObserver ( QObject * parent )
    : QDBusServiceWatcher ( _dbus_service(), QDBusConnection::sessionBus(),
                            QDBusServiceWatcher::WatchForOwnerChange, parent )
{
  setObjectName ( QLatin1String ( "BusObserver" ) );
  connection().registerObject ( "/BusObserver", this );
  connect ( this, SIGNAL ( serviceOwnerChanged ( const QString &, const QString &, const QString & ) ),
            this, SLOT ( ownerModifications ( const QString &, const QString &, const QString & ) ) );
}

void BusObserver::ownerModifications ( const QString &a, const QString &b, const QString &c )
{
  qDebug() << Q_FUNC_INFO << a << b << c;
}

BusObserver::~BusObserver()
{
  connection().unregisterObject ( "/BusObserver", QDBusConnection::UnregisterTree );
}
