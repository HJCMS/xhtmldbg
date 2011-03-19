/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "webdatabasehandler.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

/* QtWebKit */
#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebSecurityOrigin>

static inline const QString dbPath()
{
  // Dieser Pfad wird von Qt vorgegeben!
  QString p ( "/Databases.db" );
  p.prepend ( QWebSettings::offlineStoragePath () );
  return p;
}

WebDatabaseHandler::WebDatabaseHandler ( const QSqlDatabase &other, const QString &name )
    : db ( QSqlDatabase::cloneDatabase ( other, name ) )
    , database ( dbPath() )
{
  QFileInfo info ( database );
  if ( info.exists() )
  {
    db.setDatabaseName ( database );
    db.open();
  }
  else
    qWarning ( "(XHTMLDBG) Database \"%s\" does not exists!", qPrintable ( database ) );
}

const QList<WebSecurityItem*> WebDatabaseHandler::getOrigins()
{
  QList<WebSecurityItem*> list;
  QSqlQuery query = db.exec ( "SELECT origin,quota FROM Origins WHERE ( origin IS NOT NULL);" );
  if ( query.lastError().isValid() )
  {
    qWarning ( "(XHTMLDBG) WebSecurity SQL %s", qPrintable ( query.lastError().text() ) );
    return list;
  }

  QSqlRecord rec = query.record();
  if ( rec.count() >= 1 )
  {
    while ( query.next() )
    {
      QString origin = query.value ( rec.indexOf ( "origin" ) ).toString();
      if ( origin.isEmpty() )
        continue;

      qint64 quota = query.value ( rec.indexOf ( "quota" ) ).toUInt();
      list.append ( new WebSecurityItem ( origin, quota ) );
    }
    query.finish();
  }
  return list;
}

void WebDatabaseHandler::saveOrigins ( const QList<WebSecurityItem*> &list )
{
  if ( list.size() < 1 )
    return;

  QSqlQuery query = db.exec ( "DELETE FROM Origins;" );
  if ( query.lastError().isValid() )
    return;

  int size = list.size();
  for ( int r = 0; r < size; ++r )
  {
    WebSecurityItem* it = list.at ( r );
    QString sql = QString::fromUtf8 ( "INSERT INTO Origins (origin,quota) VALUES ('%1_%2_%3',%4);" )
                  .arg ( it->scheme(), it->host(), QString::number ( it->port() ), QString::number ( it->quota() ) );
    db.exec ( sql );
  }
  query.finish();
}

bool WebDatabaseHandler::hasOrigin ( const QString &hostname )
{
  bool out = false;
  QString sql ( "SELECT origin FROM Origins WHERE(origin LIKE '%_" );
  sql.append ( hostname );
  sql.append ( "_%') LIMIT 1;" );

  QSqlQuery query = db.exec ( sql );
  if ( query.lastError().isValid() )
    return false;

  out = ( query.record().count() > 0 ) ? true : false;
  query.finish();
  return out;
}

WebSecurityItem* WebDatabaseHandler::getOrigin ( const QString &hostname )
{
  WebSecurityItem* item = new  WebSecurityItem ( "file_localhost_0", ( 3 * 1024 ) );
  QString sql ( "SELECT origin FROM Origins WHERE(origin LIKE '%_" );
  sql.append ( hostname );
  sql.append ( "_%') LIMIT 1;" );

  QSqlQuery query = db.exec ( sql );
  if ( query.lastError().isValid() )
    return item;

  QSqlRecord rec = query.record();
  if ( rec.count() >= 1 )
  {
    while ( query.next() )
    {
      QString origin = query.value ( rec.indexOf ( "origin" ) ).toString();
      if ( origin.isEmpty() )
        continue;

      qint64 quota = query.value ( rec.indexOf ( "quota" ) ).toUInt();
      item = new WebSecurityItem ( origin, quota );
    }
    query.finish();
  }
  return item;
}

void WebDatabaseHandler::close()
{
  if ( db.isOpen() )
    db.close();
}
