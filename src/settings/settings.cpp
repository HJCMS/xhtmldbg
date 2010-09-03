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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "settings.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QDesktopServices>
#include <QtGui/QX11Info>

/**
* Wenn Xinerama Aktiv - dann an den Namen das Display hängen!
*/
static inline const QString xApplicationName()
{
  QString name ( XHTMLBG_APPS_NAME );
  int screen = QX11Info::appScreen();
  if ( screen != 0 )
  {
    qWarning ( "(XHTMLDBG) Xinerama detected generate configuration with suffix -<num>" );
    name.append ( "-" );
    name.append ( QString::number ( screen ) );
  }
  return name;
}

Settings::Settings ( QObject * parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, XHTMLBG_DOMAIN, xApplicationName(), parent )
    , cacheLocation ( QDesktopServices::storageLocation ( QDesktopServices::CacheLocation ) )
{
  setObjectName ( QLatin1String ( "settings" ) );
}

/**
* Qt4 Programme starten schneller wenn diese Pfade liste kleiner ist!
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
    QFile ( path ).setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) );
}

void Settings::setSaveMode()
{
  qWarning ( "(XHTMLDBG) Starting in savemode" );
  setValue ( QLatin1String ( "RecentUrl" ), QVariant ( "http://www.hjcms.de" ) );
  setValue ( QLatin1String ( "PluginsEnabled" ), false );
  remove ( QLatin1String ( "webkit_plugin_path" ) );
}

const QString Settings::webIconDatabasePath()
{
  QDir d ( cacheLocation );
  d.mkpath ( QLatin1String ( "icons" ) );
  return QString ( cacheLocation + d.separator() + QLatin1String ( "icons" ) );
}

const QString Settings::webLocalStoragePath()
{
  QDir d ( cacheLocation );
  d.mkpath ( QLatin1String ( "storage" ) );
  return QString ( cacheLocation + d.separator() + QLatin1String ( "storage" ) );
}

bool Settings::boolValue ( const QString &p, bool b )
{
  return value ( p, b ).toBool();
}

const QString Settings::strValue ( const QString &p, const QString &d )
{
  return value ( p, d ).toString();
}

Settings::~Settings()
{}