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

/**
* Wenn der Klient Desktop ein Xinerama/TwinView ist.
* Dann - An den Programm-Namen die Screen Nummer anhängen!
*/
static inline const QString xApplicationName()
{
  QString name ( XHTMLDBG_APPS_NAME );
  int screen = QX11Info::appScreen();
  if ( screen != 0 )
  {
    qWarning ( "(XHTMLDBG) Xinerama detected generate configuration with suffix -<num>" );
    name.append ( "-" );
    name.append ( QString::number ( screen ) );
  }
  return name;
}

/**
* @class Settings
* Hauptklasse für alle Einstellungen ab zu fragen!
*/
Settings::Settings ( QObject * parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, XHTMLDBG_DOMAIN, xApplicationName(), parent )
    , DefaultDirPermissons ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) )
{
  setObjectName ( QLatin1String ( "settings" ) );
  setCacheDefaults();

  QFile ( fileName () ).setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
}

/** Standard Webspeicher Verzeichnis */
const QString Settings::cacheLocation()
{
  return QDesktopServices::storageLocation ( QDesktopServices::CacheLocation );
}

/** Temporäres Verzeichnis für die LogDateien */
const QString Settings::tempDir ( const QString &subdir )
{
  QDir d ( QDir::tempPath() );
  QString p ( QDir::tempPath() );
  p.append ( d.separator() );
  p.append ( xApplicationName() );
  p.append ( "-" );
  char* uid = getenv ( "USER" );
  if ( sizeof ( uid ) > 1 )
    p.append ( QLatin1String ( uid ) );

  if ( ! subdir.isEmpty() )
  {
    p.append ( d.separator() );
    p.append ( subdir );
  }

  if ( d.exists ( p ) )
    return p;

  if ( ! d.mkpath ( p ) )
    qWarning ( "(XHTMLDBG) can not create \"%s\" directory", qPrintable ( p ) );

  return p;
}

/** Wir verwenden keinen Cache! */
void Settings::setCacheDefaults()
{
  setValue ( "OfflineStorageDatabaseEnabled", true );
  setValue ( "OfflineWebApplicationCacheEnabled", true );
  setValue ( "SourceIsFromCacheAttribute", false );
  setValue ( "CacheSaveControlAttribute", false );
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
  QString path = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  QDir d ( QDesktopServices::storageLocation ( QDesktopServices::HomeLocation ) );
  if ( d.mkpath ( path ) )
    QFile ( path ).setPermissions ( DefaultDirPermissons );
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
  return QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
}

/**
* Pfad zur Temporären Daten Speicherung
* @note Dieses Verzeichnis muss vorhanden sein damit diese Abhängigkeit Funktioniert!
*/
const QString Settings::webLocalStoragePath()
{
  QDir d ( webDatabasePath() );
  QString path = d.path() + d.separator() + QLatin1String ( "Storage" );

  if ( d.mkpath ( QLatin1String ( "Storage" ) ) )
    QFile ( path ).setPermissions ( DefaultDirPermissons );

  return path;
}

/**
* Url Pfad zur Historien XML Datei
*/
const QUrl Settings::historyXml()
{
  QUrl u;
  u.setScheme ( "file" );

  QString p = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
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

  QString p = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
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
