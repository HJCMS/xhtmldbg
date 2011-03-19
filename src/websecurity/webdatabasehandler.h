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
    const QList<WebSecurityItem*> getOrigins();
    void saveOrigins ( const QList<WebSecurityItem*> &list );
    bool hasOrigin ( const QString &hostname );
    WebSecurityItem* getOrigin ( const QString &hostname );
    void close();
};

#endif
