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

#include "cookiesstorage.h"
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

CookiesStorage::CookiesStorage ( QObject * parent )
    : QThread ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", QString::fromUtf8 ( "CookiesStorage" ) ) )
{
  setObjectName ( QLatin1String ( "cookiesstorage" ) );
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
* Alle Cookies aus der Datenbank einlesen!
*/
void CookiesStorage::loadCookies ()
{
  cookies.clear();
  QString query ( "SELECT data FROM cookiesstorage WHERE (domain!='');" );
  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
  {
#ifdef XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) SQL:SELECT - %s", qPrintable ( result.lastError().text() ) );
#endif
    return;
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
}

/**
* Cookies in die Datenbank Speichern
*/
void CookiesStorage::saveCookies()
{
  mutex.lock();
  QSqlQuery result;
  result = sql.exec ( "DELETE FROM cookiesstorage WHERE (domain!='');" );
  if ( result.lastError().type() != QSqlError::NoError )
  {
#ifdef XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) SQL:DELETE - %s", qPrintable ( result.lastError().text() ) );
#endif
    return;
  }

  foreach ( QNetworkCookie cookie, cookies )
  {
    if ( cookie.isSessionCookie() )
      continue;

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
#ifdef XHTMLDBG_DEBUG
      qWarning ( "(XHTMLDBG) SQL:INSERT - %s", qPrintable ( result.lastError().text() ) );
#endif
      return;
    }
    result.finish();
  }
  mutex.unlock();
}

/**
* Alle Cookies Speichern!
* Zuerst wir die Tabelle geleert und dann Neu geschrieben!
*/
void CookiesStorage::setCookiesList ( const QList<QNetworkCookie> &cookiesList )
{
  cookies.clear();
  cookies << cookiesList;
}

/**
* Ein einzelnes Cookie mit der Domäne abfragen!
*/
const QList<QNetworkCookie> CookiesStorage::getCookieByDomain ( const QString &domain )
{
  QList<QNetworkCookie> cookies;
  foreach ( QNetworkCookie cookie, cookies )
  {
    if ( cookie.domain() == domain )
      return QNetworkCookie::parseCookies ( cookie.value () );
  }
  return cookies;
}

/**
* Alle Cookies aus der Datenbank einlesen!
*/
const QList<QNetworkCookie> CookiesStorage::getCookies ()
{
  if ( cookies.size() < 1 )
    loadCookies();

  return cookies;
}

void CookiesStorage::run()
{
  if ( cookies.size() >= 1 )
    saveCookies();
}

CookiesStorage::~CookiesStorage()
{
  if ( sql.isOpen() )
    sql.close();
}
