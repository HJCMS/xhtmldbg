/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "settings.h"

/* std */
#include <iostream>
#include <cstdlib>

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QDesktopServices>
#include <QtGui/QX11Info>

static inline void default_file_permissions ( const QString &dir )
{
  QFile fd ( dir );
  if ( fd.exists() )
    fd.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
}

static inline void default_directory_permissions ( const QString &dir )
{
  QFile fd ( dir );
  if ( fd.exists() )
    fd.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) );
}

/**
* @class Settings
* Hauptklasse für alle Einstellungen ab zu fragen!
*/
Settings::Settings ( QObject * parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, XHTMLDBG_DOMAIN, XHTMLDBG_APPS_NAME, parent )
{
  setObjectName ( QLatin1String ( "settings" ) );
  setDataPaths();
  setCacheDefaults();

  default_file_permissions ( fileName () );
}

/** Standard Webspeicher Verzeichnis */
const QString Settings::cacheLocation()
{
  QFileInfo p ( QDesktopServices::storageLocation ( QDesktopServices::CacheLocation ) );
  // qDebug() << Q_FUNC_INFO << p.absoluteFilePath ();
  return p.absoluteFilePath ();
}

/** Default Data Location */
const QString Settings::dataLocation()
{
  QFileInfo p ( QDesktopServices::storageLocation ( QDesktopServices::DataLocation ) );
  return p.absoluteFilePath();
}

/** Temporäres Verzeichnis für die LogDateien */
const QString Settings::tempDir ( const QString &subdir )
{
  QDir d ( QDir::tempPath() );
  QString p ( QDir::tempPath() );
  p.append ( d.separator() );
  p.append ( QLatin1String ( XHTMLDBG_APPS_NAME ) );
  QByteArray uid = qgetenv ( "USER" );
  if ( ! uid.isEmpty() )
  {
    p.append ( "-" );
    p.append ( QLatin1String ( uid ) );
  }

  if ( ! subdir.isEmpty() )
  {
    p.append ( d.separator() );
    p.append ( subdir );
  }

  if ( d.exists ( p ) )
  {
    default_directory_permissions ( p );
    return p;
  }

  if ( ! d.mkpath ( p ) )
    qWarning ( "(XHTMLDBG) can not create \"%s\" directory", qPrintable ( p ) );
  else
    default_directory_permissions ( p );

  return p;
}

/** Wir verwenden keinen Cache! */
void Settings::setCacheDefaults()
{
  setValue ( "SourceIsFromCacheAttribute", false );
  setValue ( "CacheSaveControlAttribute", true );
  setValue ( "DoNotBufferUploadDataAttribute", true );
}

/**
* Qt4 Programme starten schneller wenn diese Pfadliste kleiner ist!
*/
void Settings::setIconTheme()
{
  QStringList searchPaths = value ( "iconthemepaths", QIcon::themeSearchPaths() ).toStringList();
  QIcon::setThemeSearchPaths ( searchPaths );
  QIcon::setThemeName ( value ( "icontheme", "oxygen" ).toString() );
}

/**
* BUG KDE4 >= 4.4*
* KDE 4 erstellt beim neu Initialisieren der Programmpfade nicht die
* oberen fehlenden Verzeichnisse z.B: ~/.local/share/data/xhtmldbg fehlt!
*/
void Settings::setDataPaths()
{
  QString path = dataLocation();
  QDir d ( QDesktopServices::storageLocation ( QDesktopServices::HomeLocation ) );
  if ( d.mkpath ( path ) )
    default_directory_permissions ( path );
}

/**
* Alle entscheidenden Einstellungen für den Start, Zurück setzen!
*/
void Settings::setSaveMode()
{
  qWarning ( "(XHTMLDBG) Starting in savemode" );
  setValue ( QLatin1String ( "RecentUrl" ), QVariant ( "http://www.hjcms.de" ) );
  setValue ( QLatin1String ( "PluginsEnabled" ), false );
  remove ( QLatin1String ( "webkit_plugin_path" ) );
}

/**
* Pfad zur HTML5 Grafiken Datenbank
* @note Dieses Verzeichnis muss vorhanden sein damit diese Abhängigkeit Funktioniert!
*/
const QString Settings::webDatabasePath()
{
  QDir d ( dataLocation() );
  QString path = d.path() + d.separator() + QLatin1String ( "Databases" );
  if ( d.mkpath ( path ) )
    default_directory_permissions ( d.absoluteFilePath ( path ) );

  return path;
}

/**
* Pfad zur Temporären Daten Speicherung
* @note Dieses Verzeichnis muss vorhanden sein damit diese Abhängigkeit Funktioniert!
*/
const QString Settings::webLocalStoragePath()
{
  QDir d ( dataLocation() );
  QString path = d.path() + d.separator() + QLatin1String ( "LocalStorage" );
  if ( d.mkpath ( path ) )
    default_directory_permissions ( d.absoluteFilePath ( path ) );

  return path;
}

/**
* Url Pfad zur Historien XML Datei
*/
const QUrl Settings::historyXml()
{
  QUrl u;
  u.setScheme ( "file" );

  QString p = dataLocation();
  p.append ( QDir::separator() );
  p.append ( "history.xml" );
  u.setPath ( p );

  return u;
}

/**
* Url Pfad zur Lesezeichen XBEL Datei
*/
const QUrl Settings::bookmarkXbel()
{
  QUrl u;
  u.setScheme ( "file" );

  QString p = dataLocation();
  p.append ( QDir::separator() );
  p.append ( "bookmarks.xbel" );
  u.setPath ( p );

  return u;
}

/**
* speichere das zueletzt verwendete Verzeichnis
*/
void Settings::setRecentDirectory ( const QString &d )
{
  QDir dir ( d );
  if ( dir.exists() )
    setValue ( "RecentDirectory", d );
}

/**
* get last used directory, if empty HOME is set
*/
const QString Settings::getRecentDirectory()
{
  return value ( "RecentDirectory", QDir::homePath() ).toString();
}

/**
* setze URL für RecentUrl
*/
void Settings::setRecentUrl ( const QUrl &u )
{
  if ( u.isValid() && ! u.isRelative() )
    setValue ( "RecentUrl", u );
  else
    setValue ( "RecentUrl", QUrl ( "http://localhost" ) );
}

/**
* zuletzt verwendete URL
*/
const QUrl Settings::getRecentUrl()
{
  return value ( "RecentUrl", QUrl ( "http://localhost" ) ).toUrl();
}

/**
* Vereinfachung : Zahlen Werte abfragen
*/
int Settings::intValue ( const QString &p, int i )
{
  return value ( p, i ).toUInt();
}

/**
* Vereinfachung : Boolische Werte abfragen
*/
bool Settings::boolValue ( const QString &p, bool b )
{
  return value ( p, b ).toBool();
}

/**
* Vereinfachung : Zeichenketten abfragen
*/
const QString Settings::strValue ( const QString &p, const QString &d )
{
  return value ( p, d ).toString();
}

Settings::~Settings()
{}
