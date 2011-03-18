/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#ifndef COOKIEMANAGER_H
#define COOKIEMANAGER_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtSql */
#include <QtSql/QSqlDatabase>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class CookieManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_ENUMS ( AccessType )

  private:
    QSqlDatabase sql;
    enum QueryType { OPEN, CREATE, SELECT, DELETE, UPDATE, QUERY, INSERT };
    QList<QNetworkCookie> cookies;
    void sqlMessage ( QueryType t, int l, const QString &m );
    bool open();
    void loadCookies ();
    void saveCookies();

  public:
    enum AccessType { SESSION, BLOCKED, ALLOWED, UNKNOWN };

    typedef struct Q_DECL_EXPORT
    {
      AccessType Access;
      QString Hostname;
      bool AllowThirdParty;
      bool RFC2109;
    } CookiesAccessItem;

    CookieManager ( QObject * parent = 0 );

    bool isOpen();

    const CookiesAccessItem getCookieAccess ( const QString & );

    void saveCookiesList ( const QList<QNetworkCookie> & );

    const QList<QNetworkCookie> getCookies ();

    bool saveCookiesForDomain ( const QList<QNetworkCookie> &, const QString & );

    const QList<QNetworkCookie> getCookiesForDomain ( const QString &,
            const QString &path = QString ( "/" ) );

    virtual ~CookieManager();
};

Q_DECLARE_METATYPE ( CookieManager::CookiesAccessItem )

#endif
