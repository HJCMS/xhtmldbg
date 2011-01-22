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

#include "networkblocker.h"
#include "databaselocation.h"

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

NetworkBlocker::NetworkBlocker ( QObject * parent )
    : QObject ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", "networkblocker" ) )
{
  setObjectName ( QLatin1String ( "XHTMLDBG_NETWORKBLOCKER" ) );
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
}

/**
* Datenbank öffnen
*/
bool NetworkBlocker::open()
{
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  DatabaseLocation* locator = new DatabaseLocation ( lc, this );
  if ( ! locator->initCookieDatabase ( sql ) )
  {
    qWarning ( "(XHTMLDBG) SQL Create Table: %d %s", __LINE__, qPrintable ( sql.lastError().text() ) );
    return false;
  }

  if ( ! sql.open() )
  {
    qWarning ( "(XHTMLDBG) SQL Open Database: %d %s", __LINE__, qPrintable ( sql.lastError().text() ) );
    return false;
  }

  delete locator;
  return true;
}

/**
* Prüfen ob die datenbank geöffnet ist
*/
bool NetworkBlocker::isOpen()
{
  return open();
}

bool NetworkBlocker::isBlocked ( const QUrl &url )
{
  bool blocked = false;
  if ( ! url.isValid() || url.isRelative() )
    return false;

  QString host = url.host().remove ( QRegExp ( "^www\\." ) );
  QString queryString ( "SELECT Access FROM blocker WHERE (Host LIKE '%" );
  queryString.append ( host );
  queryString.append ( "') LIMIT 1;" );

  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().type() != QSqlError::NoError )
  {
    qWarning ( "(XHTMLDBG) SQL Query Database: %d %s", __LINE__, qPrintable ( sql.lastError().text() ) );
    return blocked;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() > 0 )
  {
    while ( query.next() )
    {
      switch ( query.value ( rec.indexOf ( "Access" ) ).toUInt() )
      {
        case 0:
          blocked = false;
          break;

        case 1:
          blocked = true;
          break;

        default:
          blocked = false;
          break;
      }
    }
  }
  query.finish();

  return blocked;
}

void NetworkBlocker::insert ( const QString &host )
{
  QString query ( "INSERT INTO blocker (Host,Access) " );
  query.append ( "VALUES('" );
  query.append ( host );
  query.append ( "',1);" );
  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().isValid() )
    qWarning ( "(XHTMLDBG) SQL Insert: %d %s", __LINE__, qPrintable ( sql.lastError().text() ) );

  result.finish();
}

void NetworkBlocker::remove ( const QString &host )
{
  QString query ( "DELETE FROM blocker WHERE (Host='" );
  query.append ( host );
  query.append ( "');" );

  QSqlQuery result = sql.exec ( query );
  if ( result.lastError().type() != QSqlError::NoError )
    qWarning ( "(XHTMLDBG) SQL Insert: %d %s", __LINE__, qPrintable ( sql.lastError().text() ) );

  result.finish();
}

NetworkBlocker::~NetworkBlocker()
{}
