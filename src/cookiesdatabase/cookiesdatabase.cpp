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

#include "cookiesdatabase.h"
#include "cookiesdatabaselocation.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

/* QtGui */
// #include <QtGui/QVBoxLayout>
#include <QtGui/QDesktopServices>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

CookiesDatabase::CookiesDatabase ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", QString::fromUtf8 ( "CookiesDatabase" ) ) )
{
  setObjectName ( QLatin1String ( "cookiesdatabase" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1;PRAGMA foreign_keys=OFF;" ) );
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  m_dbLocator = new CookiesDatabaseLocation ( lc, this );
  if ( m_dbLocator->initDatabase ( sql, "cookiesstorage" ) )
  {
    sql.setDatabaseName ( m_dbLocator->databasePath ( "cookiesstorage" ) );
    if ( ! sql.open() )
      qWarning ( "(XHTMLDBG) %s", qPrintable ( sql.lastError().text() ) );
  }
}

/**
* Alle Cookie Speichern!
* Zuerst wir die Tabelle geleert und dann Neu geschrieben!
*/
void CookiesDatabase::saveCookies ( const QList<QNetworkCookie> &cookiesList )
{
  foreach ( QNetworkCookie cookie, cookiesList )
  {
    if ( cookie.isSessionCookie() )
      continue;

    QSqlQuery result;
    result = sql.exec ( "DELETE FROM cookiesstorage WHERE (domain!='');" );
    if ( result.lastError().type() != QSqlError::NoError )
    {
      qWarning ( "(XHTMLDBG) %s", qPrintable ( result.lastError().text() ) );
      return;
    }

    QString query ( "INSERT INTO cookiesstorage (domain,name,expirationDate,data) " );
    query.append ( "VALUES('" );
    query.append ( cookie.domain() );
    query.append ( "','" );
    query.append ( cookie.name() );
    query.append ( "','" );
    query.append ( cookie.expirationDate().toString() );
    query.append ( "','" );
    query.append ( cookie.toRawForm ( QNetworkCookie::Full ) );
    query.append ( "');" );

    result = sql.exec ( query );
    if ( result.lastError().type() != QSqlError::NoError )
    {
      qWarning ( "(XHTMLDBG) %s", qPrintable ( result.lastError().text() ) );
      return;
    }
    result.finish();
  }
}

/**
* Ein Cookie abfragen!
*/
const QNetworkCookie CookiesDatabase::getCookie ( const QString &domain )
{
  QNetworkCookie cookie;

  QString query ( "SELECT FROM cookiesstorage WHERE (domain='" );
  query.append ( domain );
  query.append ( "') LIMIT 1;" );

  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
  {
    qWarning ( "(XHTMLDBG) %s", qPrintable ( result.lastError().text() ) );
    return cookie;
  }

  QSqlRecord sqlrec = result.record();
  if ( sqlrec.count() >= 1 )
  {
    int data = sqlrec.indexOf ( "data" );
    cookie = QNetworkCookie::parseCookies ( result.value ( data ).toByteArray() ).first();
  }
  result.finish();

  return cookie;
}

const QList<QNetworkCookie> CookiesDatabase::loadCookies ()
{
  QList<QNetworkCookie> cookies;

  QString query ( "SELECT data FROM cookiesstorage WHERE (domain!='');" );

  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
  {
    qWarning ( "(XHTMLDBG) %s", qPrintable ( result.lastError().text() ) );
    return cookies;
  }

  QSqlRecord sqlrec = result.record();
  if ( sqlrec.count() >= 1 )
  {
    while ( result.next() )
    {
      int data = sqlrec.indexOf ( "data" );
      cookies << QNetworkCookie::parseCookies ( result.value ( data ).toByteArray() );
    }
    result.finish();
  }
  return cookies;
}

CookiesDatabase::~CookiesDatabase()
{
  if ( sql.isOpen() )
    sql.close();
}
