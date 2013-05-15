/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

/* QtWebKit */
#include <QtWebKit/QWebDatabase>
#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebSecurityOrigin>

/** NOTE Dieser Pfad wird von QtWebKit vorgegeben!
* Kann deshalb nicht geändert werden!
*/
static inline const QString webkitDatbasePath()
{
  QString p ( "/Databases.db" );
  p.prepend ( QWebSettings::offlineStoragePath () );
  return p;
}

/** Suche nach Existierender Datenbank Verbindung!
* Wenn nicht gefunden lege eine Kopie von der Übergebenen Verbindung an! */
static inline const QSqlDatabase uniqueSQLConnection ( const QSqlDatabase &d, const QString &n )
{
  QSqlDatabase db = QSqlDatabase::database ( n, false );
  if ( db.isValid() )
    return db;

  return QSqlDatabase::cloneDatabase ( d, n );
}

/**
* Erstellt ein Neues Datenbank Objekt mit der Bestehenden @b xhtmldbg Verbindung an.
* @li Setzt Gleichzeitig die Rechte der Datenbank auf Paranoid!
* @li Wenn nicht gefunden wird eine Warnung auf dem Terminal ausgegeben!
*/
WebDatabaseHandler::WebDatabaseHandler ( const QSqlDatabase &other, const QString &name )
    : db ( uniqueSQLConnection ( other, name ) )
    , database ( webkitDatbasePath() )
{
  QFileInfo info ( database );
  if ( info.exists() )
  {
    QFile ( info.absoluteFilePath() ).setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
    db.setDatabaseName ( info.absoluteFilePath() );
    db.open();
  }
  else
    qWarning ( "(XHTMLDBG) Database \"%s\" does not exists!", qPrintable ( database ) );
}

/**
* Erstelle von jeder Konfigurierten "Origin" ein Verzeichnis im Datenbank Verzeichnis an!
* NOTE Normalerweise sollte das QtWebKit machen - weiss auch nicht was das soll :-/
*/
void WebDatabaseHandler::initDatabaseDirectories ( const QStringList &origins )
{
  QDir dir ( QWebSettings::offlineStoragePath () );
  QFile::Permissions perms = ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner );
  foreach ( QString name, origins )
  {
    if ( dir.mkpath ( name ) )
      QFile ( dir.absoluteFilePath ( name ) ).setPermissions ( perms );
  }
}

/**
* Lese alle "Origin's" aus der Tabelle @b "Origins" und schreibe diese
* in QList&lt;WebSecurityItem*&gt;
*/
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

/**
* Alle @b "Origins" aus der Liste Speichern!
* @note Es wird zuvor die Tabelle geleert!
*/
void WebDatabaseHandler::saveOrigins ( const QList<WebSecurityItem*> &list )
{
  QStringList dirs;
  if ( list.size() < 1 )
    return;

  QSqlQuery query = db.exec ( "DELETE FROM Origins;" );
  if ( query.lastError().isValid() )
    return;

  int size = list.size();
  for ( int r = 0; r < size; ++r )
  {
    WebSecurityItem* it = list.at ( r );
    QString set = QString::fromUtf8 ( "%1_%2_%3" ).arg ( it->scheme(), it->host(), QString::number ( it->port() ) );
    QString sql = QString::fromUtf8 ( "INSERT INTO Origins (origin,quota) VALUES ('%1',%2);" )
                  .arg ( set, QString::number ( it->quota() ) );

    if ( it->port() >= 80 ) // NOTE Port "0" bedeuted LocalStorage !!!
      dirs.append ( set );

    db.exec ( sql );
  }
  query.finish();
  initDatabaseDirectories ( dirs );
}

/**
* Sucht an Hand des @e hostnamen nach einem @b "Origin", gibt bei Erfolg @b true zurück!
*/
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

/**
* Hole mit @e hostnamen einen @b "Origin"
* @note Wenn nicht gefunden wird ein dummy mit @b "file_localhost_0" erstellt!
*/
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

/**
* Offene SQL Verbindung schließen!
*/
void WebDatabaseHandler::close()
{
  if ( db.isOpen() )
    db.close();
}

/**
* Komplett aufräumen!
* @li Es werden zuerst alle *.localstorage Datenbank Dateien entfernt.
* @li Danach wird QWebDatabase::removeAllDatabases aufgerufen.
*/
void WebDatabaseHandler::removeAllDatabases()
{
  QDir d ( Settings::webLocalStoragePath(), QLatin1String ( "*.localstorage" ), QDir::IgnoreCase );
  foreach ( QString db, d.entryList ( QDir::Files ) )
  {
    QString fp ( Settings::webLocalStoragePath() );
    fp.append ( d.separator() );
    fp.append ( db );
    QFile ( fp ).remove();
  }
  QWebDatabase::removeAllDatabases ();
}
