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

#include "xhtmldbgadaptor.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QGenericReturnArgument>
#include <QtCore/QUrl>

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

/**
* Nachrichten übermittlung
*/
void XHtmldbgAdaptor::message ( const QString &mess )
{
  QMetaObject::invokeMethod ( parent(), "setApplicationMessage",
                              Q_ARG ( QString, mess ), Q_ARG ( bool, false ) );
}

/**
* Die übergebene Zeichenketten URL wird mit QUrl::StrictMode
* Importiert, dann weiter an die IDE (Wenn der import nicht
* fehlgeschlagen ist.) geleitet.
*/
bool XHtmldbgAdaptor::setUrl ( const QString &url )
{
  QUrl info ( url, QUrl::StrictMode );
  if ( info.isValid() && info.scheme().contains ( "http" ) )
  {
    QMetaObject::invokeMethod ( parent(), "openUrl", Q_ARG ( QUrl, info ) );
    return true;
  }
  return false;
}

/**
* Ein Datei öffnen in dem auf die existenz geprüft wird.
* Das file:// Scheme wird immer eingefügt!
*/
bool XHtmldbgAdaptor::setFile ( const QString &url )
{
  QFileInfo file ( url );
  if ( file.exists() && ! file.isExecutable() )
  {
    QUrl u ( file.absoluteFilePath() );
    u.setScheme( "file" );
    QMetaObject::invokeMethod ( parent(), "openFile", Q_ARG ( QUrl, u ) );
    return true;
  }
  return false;
}

/**
* XHMLT/HTML Quelltext an die Ansicht übergeben.
*/
bool XHtmldbgAdaptor::setSource ( const QString &xhtml )
{
  bool b = false;
  QMetaObject::invokeMethod ( parent(), "setSource", Q_RETURN_ARG ( bool, b ),
                              Q_ARG ( QString, xhtml ) );
  return b;
}

XHtmldbgAdaptor::~XHtmldbgAdaptor()
{
  if ( m_bus )
  {
    m_bus->unregisterObject ( "/xhtmldbg", QDBusConnection::UnregisterNode );
  }
}
