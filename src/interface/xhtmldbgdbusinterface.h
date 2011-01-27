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
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

class Q_DECL_EXPORT XHtmldbgDbusInterface: public QDBusInterface
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.xhtmldbg" )

  protected:
    const QString interface;
    const QDBusConnection bus;

  Q_SIGNALS:
    void message ( const QString & );
    void open ( const QUrl & );
    void setUrl ( const QUrl &, const QUrl & );
    void setSource ( const QUrl &, const QString & );

  public:
    XHtmldbgDbusInterface ( const QString &iface,
                            const QString &path,
                            const QDBusConnection &dbus,
                            QObject *parent = 0 );

    virtual ~XHtmldbgDbusInterface();
};

namespace de
{
  namespace hjcms
  {
    typedef ::XHtmldbgDbusInterface xhtmldbg;
  }
}

#endif

