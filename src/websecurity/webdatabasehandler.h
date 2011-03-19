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

#ifndef WEBDATABASEHANDLER_H
#define WEBDATABASEHANDLER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtSql */
#include <QtSql/QSqlDatabase>

/* websecurity */
#include "websecurityitem.h"

class WebDatabaseHandler
{
  private:
    QSqlDatabase db;
    const QString database;
    void initDatabaseDirectories ( const QStringList &origins );

  public:
    WebDatabaseHandler ( const QSqlDatabase &other, const QString &name = QLatin1String ( "WebSecurity" ) );

    /** get all \b "Origins" from SQLite3:Databases:Origins */
    const QList<WebSecurityItem*> getOrigins();

    /** clear SQLite3:Databases:Origins and INSERT this \param list */
    void saveOrigins ( const QList<WebSecurityItem*> &list );

    /** Find \b "Origin" with given \param hostname */
    bool hasOrigin ( const QString &hostname );

    /** Find \b "Origin" with given \param hostname
    * @note If not found, a dummy Item with \b "file_localhost_0" content returned */
    WebSecurityItem* getOrigin ( const QString &hostname );

    /** close activ db connection */
    void close();
};

#endif
