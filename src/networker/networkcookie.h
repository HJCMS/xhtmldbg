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

#ifndef NETWORKCOOKIE_H
#define NETWORKCOOKIE_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QObject>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class NetworkSettings;

class NetworkCookie : public QNetworkCookieJar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_ENUMS ( CookieArrangement )

  private:
    NetworkSettings* m_netcfg;
    QStringList cookiesBlocked;
    QStringList cookiesAllowed;
    QStringList cookiesSession;
    void load();

  Q_SIGNALS:
    void cookiesAdd ( bool );

  public Q_SLOTS:
    void save();

  public:
    enum CookieArrangement { Session = 0, Blocked = 1, Allowed = 2 };
    NetworkCookie ( QObject * parent = 0 );
    QList<QNetworkCookie> cookiesForUrl ( const QUrl & ) const;
    bool setCookiesFromUrl ( const QList<QNetworkCookie> &, const QUrl & );
    virtual ~NetworkCookie();

};

#endif