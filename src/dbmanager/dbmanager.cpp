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

#include "dbmanager.h"
#include "dblocator.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

DBManager::DBManager ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", "xhtmldbg" ) )
{
  setObjectName ( QLatin1String ( "xhtmldbg_database_manager" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
}

/** Nachrichten verarbeiten! */
void DBManager::setError ( QueryType t, int l, const QString &m )
{
  qWarning ( "SQL Error Line %d (%s)", l, qPrintable ( m ) );

  QString line = QString::number ( l );
  switch ( t )
  {
    case ACTION:
      emit error ( trUtf8 ( "SQL Invalid Operation: %1 %2" ).arg ( line, m ) );
      break;

    case OPEN:
      emit error ( trUtf8 ( "SQL Open Database: %1 %2" ).arg ( line, m ) );
      break;

    case CREATE:
      emit error ( trUtf8 ( "SQL Create Table: %1 %2" ).arg ( line, m ) );
      break;

    case SELECT:
      emit error ( trUtf8 ( "SQL Select: %1 %2" ).arg ( line, m ) );
      break;

    case DELETE:
      emit error ( trUtf8 ( "SQL Delete: %1 %2" ).arg ( line, m ) );
      break;

    case UPDATE:
      emit error ( trUtf8 ( "SQL Update: %1 %2" ).arg ( line, m ) );
      break;

    case INSERT:
      emit error ( trUtf8 ( "SQL Insert: %1 %2" ).arg ( line, m ) );
      break;

    case QUERY:
      emit error ( trUtf8 ( "SQL Query: %1 %2" ).arg ( line, m ) );
      break;

    default:
      emit error ( trUtf8 ( "SQL: %1 %2" ).arg ( line, m ) );
      break;
  }
}

/** Datenbank öffnen */
bool DBManager::open()
{
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  DBLocator* locator = new DBLocator ( lc, this );
  if ( ! locator->initDatabase ( sql ) )
  {
    setError ( CREATE, __LINE__, sql.lastError().text() );
    return false;
  }

  if ( ! sql.open() )
  {
    setError ( OPEN, __LINE__, sql.lastError().text() );
    return false;
  }
  delete locator;

  return true;
}

void DBManager::shutdown()
{
  if ( sql.isOpen() )
    sql.close();
}

/** Prüfen ob die datenbank geöffnet ist */
bool DBManager::init()
{
  return open();
}

/** SQLite Databases handle */
const QSqlDatabase DBManager::handle()
{
  return sql;
}

/** SQL INSERT Statements */
bool DBManager::insert ( const QStringList &queries )
{
  bool ok = true;
  QSqlQuery result;
  foreach ( QString query, queries )
  {
    if ( ! query.contains ( "INSERT INTO ", Qt::CaseInsensitive ) )
    {
      setError ( ACTION, __LINE__, query );
      ok = false;
      continue;
    }

    result = sql.exec ( query );
    if ( result.lastError().type() != QSqlError::NoError )
    {
      setError ( INSERT, __LINE__, sql.lastError().text() );
      ok = false;
      break;
    }
    result.finish();
  }
  return ok;
}

/** SQL SELECT Statements */
QSqlQuery DBManager::select ( const QString &query )
{
  QSqlQuery result;
  if ( ! query.contains ( "SELECT FROM ", Qt::CaseInsensitive ) )
  {
    setError ( ACTION, __LINE__, query );
    return result;
  }

  result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
    setError ( SELECT, __LINE__, sql.lastError().text() );

  return result;
}

/** SQL UPDATE Statements */
bool DBManager::update ( const QStringList &queries )
{
  bool ok = true;
  QSqlQuery result;
  foreach ( QString query, queries )
  {
    if ( ! query.contains ( "UPDATE ", Qt::CaseInsensitive ) )
    {
      setError ( ACTION, __LINE__, query );
      ok = false;
      continue;
    }

    result = sql.exec ( query );
    if ( result.lastError().type() != QSqlError::NoError )
    {
      setError ( UPDATE, __LINE__, sql.lastError().text() );
      ok = false;
      break;
    }
    result.finish();
  }
  return ok;
}

/** SQL DELETE Statements */
bool DBManager::remove ( const QStringList &queries )
{
  bool ok = true;
  QSqlQuery result;
  foreach ( QString query, queries )
  {
    if ( ! query.contains ( "DELETE FROM ", Qt::CaseInsensitive ) )
    {
      setError ( ACTION, __LINE__, query );
      ok = false;
      continue;
    }

    result = sql.exec ( query );
    if ( result.lastError().type() != QSqlError::NoError )
    {
      setError ( DELETE, __LINE__, sql.lastError().text() );
      ok = false;
      break;
    }
    result.finish();
  }
  return ok;
}

DBManager::~DBManager()
{
  shutdown();
}
