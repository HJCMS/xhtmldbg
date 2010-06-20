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

#ifndef NETWORKCOOKIE_H
#define NETWORKCOOKIE_H

/* QtCore */
#include <QtCore/QMutex>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QUrl>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class NetworkSettings;
class AutoSaver;

class NetworkCookie : public QNetworkCookieJar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_ENUMS ( CookieArrangement )

  private:
    NetworkSettings* m_netcfg;
    AutoSaver* m_autoSaver;
    QMutex mutex;
    QString iniFile;
    QStringList cookiesBlocked;
    QStringList cookiesAllowed;
    QStringList cookiesSession;
    const QString cookieDomainFromHost ( const QUrl & ) const;
    bool validateDomainAndHost ( const QString &, const QUrl & );
    const QDateTime cookieLifeTime();
    void load();

  Q_SIGNALS:
    void cookieNotice ( const QString & );
    void cookieRejected ( const QString & );
    void cookieRequest ( const QUrl & );
    void cookieChanged ();

  public Q_SLOTS:
    void save();
    void reload();

  public:
    enum CookieArrangement { Session = 0, Blocked = 1, Allowed = 2 };
    NetworkCookie ( NetworkSettings * settings, QObject * parent = 0 );
    QList<QNetworkCookie> cookiesForUrl ( const QUrl & ) const;
    bool setCookiesFromUrl ( const QList<QNetworkCookie> &, const QUrl & );
    virtual ~NetworkCookie();

};

#endif
