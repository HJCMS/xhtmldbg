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

#include "cookiemanager.h"
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

CookieManager::CookieManager ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", "cookies" ) )
{
  setObjectName ( QLatin1String ( "cookiemanager" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
}

/**
* Nachrichten verarbeiten!
*/
void CookieManager::sqlMessage ( QueryType t, int l, const QString &m )
{
#ifndef XHTMLDBG_DEBUG
  Q_UNUSED ( t )
  Q_UNUSED ( m )
#else
  switch ( t )
  {
    case OPEN:
      qWarning ( "(XHTMLDBG) SQL Open Database: %d %s", l, qPrintable ( m ) );
      break;

    case CREATE:
      qWarning ( "(XHTMLDBG) SQL Create Table: %d %s", l, qPrintable ( m ) );
      break;

    case SELECT:
      qWarning ( "(XHTMLDBG) SQL Select: %d %s", l, qPrintable ( m ) );
      break;

    case DELETE:
      qWarning ( "(XHTMLDBG) SQL Delete: %d %s", l, qPrintable ( m ) );
      break;

    case UPDATE:
      qWarning ( "(XHTMLDBG) SQL Update: %d %s", l, qPrintable ( m ) );
      break;

    case INSERT:
      qWarning ( "(XHTMLDBG) SQL Insert: %d %s", l, qPrintable ( m ) );
      break;

    case QUERY:
      qWarning ( "(XHTMLDBG) SQL Query: %d %s", l, qPrintable ( m ) );
      break;

    default:
      qWarning ( "(XHTMLDBG) SQL: %d %s", l, qPrintable ( m ) );
      break;
  }
#endif
}

/**
* Datenbank öffnen
*/
bool CookieManager::open()
{
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  CookiesDatabaseLocation* locator = new CookiesDatabaseLocation ( lc, this );
  if ( ! locator->initCookieDatabase ( sql ) )
  {
    sqlMessage ( CREATE, __LINE__, sql.lastError().text() );
    return false;
  }

  if ( ! sql.open() )
  {
    sqlMessage ( OPEN, __LINE__, sql.lastError().text() );
    return false;
  }

  delete locator;
  return true;
}

/**
* Prüfen ob die datenbank geöffnet ist
*/
bool CookieManager::isOpen()
{
  return open();
}

/**
* Informationen für ein Cookie abfragen!
* Wenn kein Eintrag gefunden werden konnet wird @ref AccessType::UNKNOWN
* zurück gegeben!
*/
const CookieManager::CookiesAccessItem CookieManager::getCookieAccess ( const QString &hostname )
{
  CookiesAccessItem item;
  item.Access = UNKNOWN;
  item.Hostname = hostname;
  item.AllowThirdParty = false;
  item.RFC2109 = true;

  QString queryString ( "SELECT AccessType,AllowThirdParty,RFC2109 FROM cookieshandle WHERE (Hostname='" );
  queryString.append ( hostname );
  queryString.append ( "') LIMIT 1;" );

  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().type() != QSqlError::NoError )
  {
    sqlMessage ( SELECT, __LINE__, sql.lastError().text() );
    return item;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() > 0 )
  {
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

/**
* Alle Cookies aus der Datenbank einlesen!
*/
void CookieManager::loadCookies ()
{
  cookies.clear();
  QString query ( "SELECT data FROM cookiesstorage WHERE (domain!='');" );
  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().isValid() )
  {
    sqlMessage ( SELECT, __LINE__, sql.lastError().text() );
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
void CookieManager::saveCookies()
{
  QSqlQuery result;
  result = sql.exec ( "DELETE FROM cookiesstorage WHERE (domain!='');" );
  if ( result.lastError().type() != QSqlError::NoError )
  {
    sqlMessage ( DELETE, __LINE__, sql.lastError().text() );
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
    if ( result.lastError().isValid() )
    {
      sqlMessage ( INSERT, __LINE__, sql.lastError().text() );
      sqlMessage ( INSERT, __LINE__, query );
    }
    result.finish();
  }
}

/**
* Alle Cookies Speichern!
* Zuerst wir die Tabelle geleert und dann Neu geschrieben!
*/
void CookieManager::saveCookiesList ( const QList<QNetworkCookie> &cookiesList )
{
  if ( cookiesList.size() < 1 )
    return;

  cookies.clear();

  cookies << cookiesList;

  saveCookies();
}

/**
* Ein einzelnes Cookie mit der Domäne abfragen!
*/
const QList<QNetworkCookie> CookieManager::getCookieByDomain ( const QString &domain )
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
const QList<QNetworkCookie> CookieManager::getCookies ()
{
  if ( cookies.size() < 1 )
    loadCookies();

  return cookies;
}

CookieManager::~CookieManager()
{
}
