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

#include "cookieshandle.h"
#include "cookiesdatabaselocation.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

// trUtf8 ( "Session" ) << trUtf8 ( "Blocked" ) << trUtf8 ( "Allowed" )
/*
CREATE TABLE "cookieshandle" (
  "Hostname" TEXT PRIMARY KEY ASC,
  "AccessType" INTEGER default 0,
  "AllowThirdParty" INTEGER default 0,
  "RFC2109" INTEGER default 0
);
*/
CookiesHandle::CookiesHandle ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", QString::fromUtf8 ( "CookiesHandle" ) ) )
{
  setObjectName ( QLatin1String ( "cookieshandle" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  CookiesDatabaseLocation* locator = new CookiesDatabaseLocation ( lc, this );
  if ( locator->initDatabase ( sql, objectName() ) )
  {
    sql.setDatabaseName ( locator->databasePath ( objectName() ) );
    if ( ! sql.open() )
      qWarning ( "(XHTMLDBG) Database: %s", qPrintable ( sql.lastError().text() ) );
  }
  delete locator;
}

/**
* Nachsehen ob dieser Hostname ein Session oder erlaubt enthält.
*/
bool CookiesHandle::isAccepted ( const QString &hostname )
{
  QString query ( "SELECT AccessType FROM cookieshandle WHERE (Hostname='" );
  query.append ( hostname );
  query.append ( "' AND (AccessType=0 OR AccessType=2) ) LIMIT 1;" );

  QSqlQuery result;
  result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
  {
#ifdef XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) SQL:SELECT - %s", qPrintable ( result.lastError().text() ) );
#endif
    return false;
  }

  QSqlRecord sqlrec = result.record();
  if ( sqlrec.count() == 1 )
  {
    if ( result.value ( sqlrec.indexOf ( "AccessType" ) ).toUInt() != 1 )
      return true;
  }
  result.finish();

  return false;
}

/**
* Nachsehen ob dieser Hostname geblockt ist
*/
bool CookiesHandle::isBlocked ( const QString &hostname )
{
  QString query ( "SELECT AccessType FROM cookieshandle WHERE (Hostname='" );
  query.append ( hostname );
  query.append ( "' AND AccessType=1) LIMIT 1;" );

  QSqlQuery result;
  result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
  {
#ifdef XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) SQL:SELECT - %s", qPrintable ( result.lastError().text() ) );
#endif
    return true;
  }

  // INSERT INTO "cookieshandle" VALUES(0,'klettern-ettringen.jh',0,1);
  QSqlRecord sqlrec = result.record();
  if ( sqlrec.count() == 1 )
  {
    if ( result.value ( sqlrec.indexOf ( "AccessType" ) ).toUInt() == 1 )
      return true;
  }
  result.finish();

  return false;
}

CookiesHandle::~CookiesHandle()
{
  if ( sql.isOpen() )
    sql.close();
}
