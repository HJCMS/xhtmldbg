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

#include "xhtmldbgadaptor.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGenericReturnArgument>

XHtmldbgAdaptor::XHtmldbgAdaptor ( QObject *parent )
    : QDBusAbstractAdaptor ( parent )
{
  m_bus = new QDBusConnection ( QDBusConnection::sessionBus() );
  m_bus->registerService ( "de.hjcms.xhtmldbg" );
  m_bus->registerObject ( "/xhtmldbg", parent, QDBusConnection::ExportAdaptors );
  setAutoRelaySignals ( false );
}

bool XHtmldbgAdaptor::registerSubObject ( QObject * object )
{
  if ( object->objectName().isEmpty() )
    return false;

  QString path ( QLatin1String ( "/xhtmldbg/" ) );
  path.append ( object->objectName() );

  return m_bus->registerObject ( path, object, QDBusConnection::ExportAdaptors );
}

void XHtmldbgAdaptor::message ( const QString &mess )
{
  qDebug() << Q_FUNC_INFO << mess;
}

XHtmldbgAdaptor::~XHtmldbgAdaptor()
{
  if ( m_bus )
  {
    m_bus->unregisterObject ( "/xhtmldbg", QDBusConnection::UnregisterNode );
  }
}
