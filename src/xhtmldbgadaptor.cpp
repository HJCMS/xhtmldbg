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
#include <QtCore/QRegExp>
#include <QtCore/QUrl>

XHtmldbgAdaptor::XHtmldbgAdaptor ( QObject * parent )
    : QDBusAbstractAdaptor ( parent )
    , service ( "de.hjcms.xhtmldbg" )
{
  m_bus = new QDBusConnection ( QDBusConnection::sessionBus() );
  m_bus->registerService ( service );
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
* Den Aktuellen D-Bus Service zurück geben!
*/
const QString XHtmldbgAdaptor::busService()
{
  return service;
}

/**
* Aktuelle D-Bus Verbindung zurück geben!
*/
const QDBusConnection XHtmldbgAdaptor::busConnection()
{
  return m_bus->sessionBus();
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
* Die übergebene Zeichenketten URL wird mit QUrl::StrictMode Importiert.
* Diese Methode öffnet sendet einen Dialog an die IDE und fragt zuerst!
*/
bool XHtmldbgAdaptor::open ( const QString &url )
{
  bool b = false;
  QUrl u ( url, QUrl::StrictMode );
  if ( u.isValid() && u.scheme().contains ( "http" ) )
  {
    QMetaObject::invokeMethod ( parent(), "urlRequest", Q_RETURN_ARG ( bool, b ), Q_ARG ( QUrl, u ) );
    return b;
  }
  else if ( u.isValid() && u.scheme().contains ( "file" ) )
  {
    return setFile ( u.toString ( QUrl::RemoveScheme ) );
  }
  else if ( u.isValid() && u.scheme().contains ( "ftp" ) )
  {
    message ( trUtf8 ( "(XHTMLDBG) Reject \"%1\" FTP request!" ).arg ( u.toString() ) );
    return false;
  }
  return false;
}

/**
* Die übergebene Zeichenketten URL wird mit QUrl::StrictMode
* Importiert, dann weiter an die IDE (Wenn der import nicht
* fehlgeschlagen ist.) geleitet.
* Diese Methode öffnet eine NeueSeite wenn die alte URL nicht Vorhanden ist!
*/
bool XHtmldbgAdaptor::setUrl ( const QString &oldUrl, const QString &newUrl )
{
  bool b = false;
  QUrl info ( newUrl, QUrl::StrictMode );
  if ( info.isValid() && info.scheme().contains ( "http" ) )
  {
    QUrl old ( oldUrl );
    QMetaObject::invokeMethod ( parent(), "setPageUrl", Q_RETURN_ARG ( bool, b ),
                                Q_ARG ( QUrl, old ), Q_ARG ( QUrl, info ) );
    return b;
  }
  else if ( info.isValid() && info.scheme().contains ( "file" ) )
  {
    return setFile ( info.toString ( QUrl::RemoveScheme ) );
  }
  return false;
}

/**
* Ein Datei öffnen in dem auf die existenz geprüft wird.
* Das file:// Scheme wird immer eingefügt!
*/
bool XHtmldbgAdaptor::setFile ( const QString &url )
{
  bool b = false;
  QString buffer ( url );
  QFileInfo file ( buffer.remove ( "file://" ) );
  if ( file.exists() && ! file.isExecutable() )
  {
    QUrl u ( file.absoluteFilePath() );
    bool addtab = true;
    u.setScheme ( "file" );
    QMetaObject::invokeMethod ( parent(), "openUrl", Q_RETURN_ARG ( bool, b )
                                , Q_ARG ( QUrl, u ), Q_ARG ( bool, addtab ) );
    return b;
  }
  return false;
}

/**
* XHMLT/HTML Quelltext an die Ansicht übergeben.
*/
bool XHtmldbgAdaptor::setSource ( const QString &url, const QString &xhtml )
{
  bool b = false;
  QUrl sendUrl ( url, QUrl::StrictMode );
  QMetaObject::invokeMethod ( parent(), "setSource", Q_RETURN_ARG ( bool, b ),
                              Q_ARG ( QUrl, sendUrl ), Q_ARG ( QString, xhtml ) );
  return b;
}

XHtmldbgAdaptor::~XHtmldbgAdaptor()
{
  if ( m_bus )
  {
    m_bus->unregisterObject ( "/xhtmldbg", QDBusConnection::UnregisterNode );
  }
}
