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

#ifndef XHTMLDBGDBUSINTERFACE_H
#define XHTMLDBGDBUSINTERFACE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtDBus */
#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QDBusConnection>

class XHtmldbgDbusInterface: public QDBusAbstractInterface
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  public:
    static inline const char *staticInterfaceName()
    { return "de.hjcms.xhtmldbg"; }

  public:
    XHtmldbgDbusInterface ( const QString &service, const QString &path,
                            const QDBusConnection &connection, QObject *parent = 0 );

    ~XHtmldbgDbusInterface();

  Q_SIGNALS:
    void message ( const QString &mess );

};

#endif
