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

CookiesHandle::CookiesHandle ( QObject * parent, const QString &dbName )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", dbName ) )
{
  setObjectName ( QLatin1String ( "cookieshandle" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
}

bool CookiesHandle::open()
{
  if ( sql.isOpen() )
    return true;

  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  CookiesDatabaseLocation* locator = new CookiesDatabaseLocation ( lc, this );
  if ( locator->initDatabase ( sql, objectName() ) )
  {
    sql.setDatabaseName ( locator->databasePath ( objectName() ) );
    if ( ! sql.open() )
    {
      qWarning ( "(XHTMLDBG) Database: %s", qPrintable ( sql.lastError().text() ) );
      return false;
    }
  }
  delete locator;
  return true;
}

/**
* Informationen für ein Cookie abfragen!
*/
const CookiesHandle::CookiesAccessItem CookiesHandle::getCookieAccess ( const QString &hostname )
{
  CookiesAccessItem item;
  item.Access = BLOCKED;
  item.Hostname = hostname;
  item.AllowThirdParty = false;
  item.RFC2109 = true;

  QString queryString ( "SELECT AccessType,AllowThirdParty,RFC2109 FROM cookieshandle WHERE (Hostname='" );
  queryString.append ( hostname );
  queryString.append ( "') LIMIT 1;" );

  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().type() != QSqlError::NoError )
  {
#ifdef XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) SQL:SELECT - %s", qPrintable ( query.lastError().text() ) );
#endif
    return item;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() > 0 )
  {
#ifdef XHTMLDBG_DEBUG_VERBOSE
    qDebug ( "(XHTMLDBG) SQL:QUERIES - %s", qPrintable ( queryString ) );
#endif
    while ( query.next() )
    {
      // AccessType
      switch ( query.value ( rec.indexOf ( "AccessType" ) ).toUInt() )
      {
        case 0:
          item.Access = SESSION;
          break;

        case 1:
          item.Access = BLOCKED;
          break;

        case 2:
          item.Access = ALLOWED;
          break;

        default:
          item.Access = BLOCKED;
          break;
      }
      // AllowThirdParty
      item.AllowThirdParty = query.value ( rec.indexOf ( "AllowThirdParty" ) ).toBool();

      // RFC2109
      item.RFC2109 = query.value ( rec.indexOf ( "RFC2109" ) ).toBool();
    }
  }
  query.finish();

  return item;
}

CookiesHandle::~CookiesHandle()
{
  if ( sql.isOpen() )
    sql.close();
}
