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

#include "databaselocation.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

DatabaseLocation::DatabaseLocation ( const QString &storageDirectory, QObject * parent )
    : QObject ( parent )
    , p_dbDir ( storageDirectory )
{
  setObjectName ( QLatin1String ( "DatabaseLocation" ) );
}

/**
* SQL Tabellen Context.
*/
const QString DatabaseLocation::sqlTableStatement () const
{
  QString statement ( "CREATE TABLE \"blocker\" (\n" );
  statement.append ( "\"Host\" TEXT  UNIQUE PRIMARY KEY ASC,\n" );
  statement.append ( "\"Access\" INTEGER default 0\n);\n" );
  return statement;
}

/**
* Sucht nach den SQL Datebanken
* @li \b cookiesstorage.db : Verwaltet die CookieJars
* @li \b cookieshandle.db  : Verwaltet die Hostnamen Behandlung
* Wenn diese nicht vorhanden sind wird eine Leere Datei angelegt!
* Zum schluss werden die Berichtigungen neu gesetzt.
*/
const QString DatabaseLocation::databasePath ( const QString &dbName )
{
  QFileInfo info ( p_dbDir, QString ( "%1.db" ).arg ( dbName ) );
  if ( info.exists() ) // existiert dann austeigen
    return info.absoluteFilePath();

  QString dbPath = info.absoluteFilePath();
  QFile file ( dbPath );
  if ( file.open ( QIODevice::WriteOnly ) )
  {
    file.close();
    file.setPermissions ( dbPath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup );
  }
  else
    qWarning ( "(XHTMLDBG) Can not write %s Database - Permission Denied!", qPrintable ( dbPath ) );

  return dbPath;
}

/**
* Öffnet eine Datenbank, wenn nicht vorhanden wird versucht eine
* Neue Leere Datenbank mit @param driver an zulegen.
*/
bool DatabaseLocation::initCookieDatabase ( const QSqlDatabase &driver )
{
  QSqlDatabase db = driver.database ( driver.connectionName(), false );

  if ( db.isOpen() )
    return true;

  db.setDatabaseName ( databasePath ( driver.connectionName() ) );
  if ( db.open() )
  {
    db.transaction();
    QSqlQuery query = db.exec ( sqlTableStatement () );
    if ( query.lastError().type() == QSqlError::NoError )
    {
      // alles OK dann austeigen
    }
    else if ( query.lastError().type() == QSqlError::StatementError )
    {
      // Wenn die Tabelle schon angelegt ist - ist alles OK!
#ifdef XHTMLDBG_DEBUG_VERBOSE
      qWarning ( "(XHTMLDBG) SQL DB CREATE %s", qPrintable ( query.lastError().text() ) );
#endif
    }
    db.commit();

    query.finish();
    db.close();
    return true;
  }

#ifdef XHTMLDBG_DEBUG_VERBOSE
  qWarning ( "(XHTMLDBG) SQL MESSAGES %s", qPrintable ( db.lastError().text() ) );
#endif

  return false;
}

DatabaseLocation::~DatabaseLocation()
{}
