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

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

CookieManager::CookieManager ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::database ( QString::fromUtf8 ( "xhtmldbg" ), false ) )
{
  setObjectName ( QLatin1String ( "cookiemanager" ) );
}

/**
* Nachrichten verarbeiten!
*/
void CookieManager::sqlMessage ( QueryType t, int l, const QString &m )
{
#ifndef XHTMLDBG_DEBUG
  Q_UNUSED ( t )
  Q_UNUSED ( l )
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
  return ( sql.isOpen() ? true : sql.open() );
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

  // Veraltet Cookies werden Später überschrieben!
  QString queryString ( "SELECT data FROM cookiesstorage WHERE (expiration>=strftime('%Y%m%d','now'));" );
  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().isValid() )
  {
    sqlMessage ( SELECT, __LINE__, sql.lastError().text() );
    return;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() >= 1 )
  {
    int index = rec.indexOf ( "data" );
    while ( query.next() )
    {
      QList<QNetworkCookie> dc = QNetworkCookie::parseCookies ( query.value ( index ).toByteArray () );
      if ( dc.size() > 0 )
        cookies.append ( dc );
    }
    query.finish();
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

    QString query ( "INSERT INTO cookiesstorage (domain,path,name,expiration,data) " );
    query.append ( "VALUES('" );
    query.append ( cookie.domain() );
    query.append ( "','" );
    query.append ( cookie.path() );
    query.append ( "','" );
    query.append ( cookie.name() );
    query.append ( "','" );
    query.append ( cookie.expirationDate().toString ( "yyyymmdd" ) );
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
* Zuerst wir die Tabelle gelehrt und dann neu geschrieben!
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
* Alle Cookies aus der Datenbank einlesen!
*/
const QList<QNetworkCookie> CookieManager::getCookies ()
{
  if ( cookies.size() < 1 )
    loadCookies();

  return cookies;
}

/**
* Keksliste von einer Domäne in die Datenbank schreiben!
*/
bool CookieManager::saveCookiesForDomain ( const QList<QNetworkCookie> &cookies, const QString &hostname )
{
  bool ok = false;
  QSqlQuery query;
  QString queryDeleteString ( "DELETE FROM cookiesstorage WHERE (domain='" );
  queryDeleteString.append ( hostname );
  queryDeleteString.append ( "' OR domain='." );
  queryDeleteString.append ( hostname );
  queryDeleteString.append ( "');" );

  query = sql.exec ( queryDeleteString );
  if ( query.lastError().isValid() )
  {
    sqlMessage ( DELETE, __LINE__, sql.lastError().text() );
    sqlMessage ( INSERT, __LINE__, queryDeleteString );
    return ok;
  }

  foreach ( QNetworkCookie cookie, cookies )
  {
    if ( cookie.isSessionCookie() )
      continue;

    QString queryString ( "INSERT INTO cookiesstorage (domain,path,name,expiration,data) " );
    queryString.append ( "VALUES('" );
    queryString.append ( cookie.domain() );
    queryString.append ( "','" );
    queryString.append ( cookie.path() );
    queryString.append ( "','" );
    queryString.append ( cookie.name() );
    queryString.append ( "','" );
    queryString.append ( cookie.expirationDate().toString ( "yyyymmdd" ) );
    queryString.append ( "','" );
    queryString.append ( cookie.toRawForm ( QNetworkCookie::Full ) );
    queryString.append ( "');" );

    query = sql.exec ( queryString );
    if ( query.lastError().isValid() )
    {
      sqlMessage ( INSERT, __LINE__, sql.lastError().text() );
      sqlMessage ( INSERT, __LINE__, queryString );
    }
    else
      ok = true;

    query.finish();
  }

  return ok;
}

/**
* Abfrage der Keksliste von einer Domäne!
* Wenn der Hostnamen
*/
const QList<QNetworkCookie> CookieManager::getCookiesForDomain ( const QString &domain,
        const QString &path )
{
  QList<QNetworkCookie> domainCookies;
  QString queryString ( "SELECT data FROM cookiesstorage WHERE ( (domain = '" );
  queryString.append ( domain );
  queryString.append ( "' OR domain = '." );
  queryString.append ( domain );
  queryString.append ( "') AND path = '" );
  queryString.append ( path );
  queryString.append ( "');" );

  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().isValid() )
  {
    sqlMessage ( SELECT, __LINE__, sql.lastError().text() );
    return domainCookies;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() >= 1 )
  {
    int index = rec.indexOf ( "data" );
    while ( query.next() )
    {
      QList<QNetworkCookie> dc = QNetworkCookie::parseCookies ( query.value ( index ).toByteArray () );
      if ( dc.size() > 0 )
        domainCookies.append ( dc );
    }
    query.finish();
  }

  return domainCookies;
}

CookieManager::~CookieManager()
{}
