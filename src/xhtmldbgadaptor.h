/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef XHTMLDBGADAPTOR_H
#define XHTMLDBGADAPTOR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtDBus */
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusConnection>

class XHtmldbgAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.xhtmldbg" )
    Q_CLASSINFO ( "D-Bus Introspection", ""
"<interface name=\"de.hjcms.xhtmldbg\" >\n"
" <method name=\"message\">\n"
"   <arg direction=\"in\" type=\"s\" name=\"mess\" />\n"
"   <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\"/>\n"
" </method>\n"
" <method name=\"open\">\n"
"   <arg direction=\"in\" type=\"s\" name=\"url\" />\n"
"   <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"false\"/>\n"
" </method>\n"
" <method name=\"setUrl\">\n"
"   <arg direction=\"in\" type=\"s\" name=\"oldUrl\" />\n"
"   <arg direction=\"in\" type=\"s\" name=\"newUrl\" />\n"
"   <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"false\"/>\n"
" </method>\n"
" <method name=\"setFile\">\n"
"   <arg direction=\"in\" type=\"s\" name=\"url\" />\n"
"   <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"false\"/>\n"
" </method>\n"
" <method name=\"setSource\">\n"
"   <arg direction=\"in\" type=\"s\" name=\"xhtml\" />\n"
"   <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"false\"/>\n"
" </method>\n"
"</interface>\n"
"" )

  private:
    const QString service;
    QDBusConnection* m_bus;

  public:
    XHtmldbgAdaptor ( QObject *parent = 0 );
    bool registerSubObject ( QObject * object );
    const QString busService();
    const QDBusConnection busConnection();
    virtual ~XHtmldbgAdaptor();

  public Q_SLOTS:
    Q_NOREPLY void message ( const QString &mess );
    bool open ( const QString &url );
    bool setUrl ( const QString &oldUrl, const QString &newUrl );
    bool setFile ( const QString &url );
    bool setSource ( const QString &xhtml );

};

#endif
