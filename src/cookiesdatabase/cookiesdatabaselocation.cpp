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

#include "cookiesdatabaselocation.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

CookiesDatabaseLocation::CookiesDatabaseLocation ( const QString &storageDirectory, QObject * parent )
    : QObject ( parent )
    , p_dbDir ( storageDirectory )
{
  setObjectName ( QLatin1String ( "CookiesDatabaseLocation" ) );
}

/**
* Sucht in den Ressoursen nach den SQL Dokumenten.
* @li \b cookiesstorage.sql : Verwaltet die CookieJars
* @li \b cookieshandle.sql  : Verwaltet die Hostnamen Behandlung
*/
const QString CookiesDatabaseLocation::sqlTableStatement ( const QString &dbName )
{
  QString statement;
  Q_INIT_RESOURCE ( cookiesdatabase );
  QFile file ( QString ( ":/%1.sql" ).arg ( dbName ) );
  if ( file.open ( QIODevice::ReadOnly ) )
  {
    QTextStream out ( &file );
    statement = out.readAll();
    file.close();
  }
  return statement;
}

/**
* Sucht nach den SQL Datebanken
* @li \b cookiesstorage.db : Verwaltet die CookieJars
* @li \b cookieshandle.db  : Verwaltet die Hostnamen Behandlung
* Wenn diese nicht vorhanden sind wird eine Leere Datei angelegt!
* Zum schluss werden die Berichtigungen neu gesetzt.
*/
const QString CookiesDatabaseLocation::databasePath ( const QString &dbName )
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
* Öffnet eine Datenbank mit @param dbName, wenn nicht vorhanden
* wird versucht eine Neue Leere Datenbank mit @param driver an zulegen.
*/
bool CookiesDatabaseLocation::initDatabase ( const QSqlDatabase &driver, const QString &dbName )
{
  QSqlDatabase db = driver.database ( driver.connectionName(), false );
  db.setDatabaseName ( databasePath ( dbName ) );
  if ( db.open() )
  {
    db.transaction();
    QSqlQuery query = db.exec ( sqlTableStatement ( dbName ) );
    if ( query.lastError().type() == QSqlError::NoError )
    {
      // alles OK dann austeigen
      db.commit();
      query.finish();
      db.close();
      return true;
    }
    else if ( query.lastError().type() == QSqlError::StatementError )
    {
      // Wenn die Tabelle schon angelegt ist - ist alles OK!
#ifdef XHTMLDBG_DEBUG_VERBOSE
      qWarning ( "(XHTMLDBG) %s", qPrintable ( query.lastError().text() ) );
#endif
      return true;
    }
  }

#ifdef XHTMLDBG_DEBUG_VERBOSE
  qWarning ( "(XHTMLDBG) %s", qPrintable ( db.lastError().text() ) );
#endif

  return false;
}

CookiesDatabaseLocation::~CookiesDatabaseLocation()
{}
