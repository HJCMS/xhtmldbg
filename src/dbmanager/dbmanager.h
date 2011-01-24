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

#ifndef DBMANAGER_H
#define DBMANAGER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtSql */
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class DBManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QSqlDatabase sql;
    enum QueryType { ACTION, OPEN, CREATE, SELECT, DELETE, UPDATE, QUERY, INSERT };

  private Q_SLOTS:
    void setError ( QueryType t, int l, const QString &m );

  protected:
    bool open();
    virtual void shutdown();

  Q_SIGNALS:
    void error ( const QString & );

  public:
    explicit DBManager ( QObject * parent = 0 );

    /** Initial Databases */
    bool init();

    /** SQLite Databases handle */
    const QSqlDatabase handle();

    /** SQL SELECT Statements */
    QSqlQuery select ( const QString &query );

    /** SQL INSERT Statements */
    bool insert ( const QStringList &queries );

    /** SQL UPDATE Statements */
    bool update ( const QStringList &queries );

    /** SQL DELETE Statements */
    bool remove ( const QStringList &queries );

    virtual ~DBManager();
};

#endif
