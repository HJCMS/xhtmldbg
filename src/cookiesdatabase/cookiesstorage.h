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

#ifndef COOKIESSTORAGE_H
#define COOKIESSTORAGE_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QVariant>

/* QtSql */
#include <QtSql/QSqlDatabase>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class CookiesStorage : public QThread
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QSqlDatabase sql;
    QMutex mutex;
    QList<QNetworkCookie> cookies;
    void loadCookies ();
    void saveCookies();

  public:
    CookiesStorage ( QObject * parent = 0 );
    void setCookiesList ( const QList<QNetworkCookie> & );
    const QList<QNetworkCookie> getCookieByDomain ( const QString & );
    const QList<QNetworkCookie> getCookies ();
    void run();
    virtual ~CookiesStorage();
};

#endif
