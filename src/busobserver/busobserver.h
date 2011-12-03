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

#ifndef BUSOBSERVER_H
#define BUSOBSERVER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QObject>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusServiceWatcher>

class BusObserver : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.xhtmldbg" )

  private:
    const QString serv;
    QDBusServiceWatcher* m_watcher;

  public Q_SLOTS:
    Q_NOREPLY void message ( const QString &mess );
    /** \return true if URL accepted otherwise false */
    bool open ( const QString &url );
    /** \return true if URL accepted otherwise false */
    bool setUrl ( const QString &oldUrl, const QString &newUrl );
    /** \return true if URL accepted otherwise false */
    bool setFile ( const QString &url );
    /** \return true if URL accepted otherwise false */
    bool setSource ( const QString &url, const QString &xhtml );
    /** \return true if URL accepted otherwise false */
    bool checkStyleSheet ( const QString &url );

  public:
    BusObserver ( const QDBusConnection dbus, QObject * parent = 0 );
    const QDBusConnection connection();
    const QString busService();
    void watchService ( const QString &service );
    virtual ~BusObserver();
};

#endif
