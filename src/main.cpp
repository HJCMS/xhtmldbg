/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://xhtmldbg.hjcms.de, (C) 2007-2010
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

#include <cstdlib>

#include <QtCore/QCoreApplication>
#include <QtCore/QGlobalStatic>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTranslator>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QIcon>

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "xhtmldbgmain.h"

int main ( int argc, char *argv[] )
{
  // Setze das Grafiksystem immer auf Native bei KDE 3!
  QProcessEnvironment env ( QProcessEnvironment::systemEnvironment () );
  if ( env.contains ( "KDE_SESSION_VERSION" ) && ( env.value ( "KDE_SESSION_VERSION", "3" ) == "3" ) )
    QApplication::setGraphicsSystem ( QLatin1String ( "native" ) );

  /**
  * HACK QTWEBKIT_PLUGIN_PATH
  * QWebKit kackt des öffteren ab wenn ein versuch Plugins zu laden fehlschlägt!
  * Leider bringen die Optionen mit @ref QWebSettings nicht viel :-/
  * Hier ein Hack zum absoluten abschalten in dem die Globalen Variablen
  * von Mozilla und WebKit gelöscht werden!
  * @link http://doc.qt.nokia.com/4.6/webintegration.html
  */
  QSettings cfg ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "xhtmldbg" );
  if ( ! cfg.value ( QLatin1String ( "PluginsEnabled" ), false ).toBool() )
  {
    env.remove ( QLatin1String ( "MOZILLA_HOME" ) );
    env.remove ( QLatin1String ( "MOZ_PLUGIN_PATH" ) );
    env.remove ( QLatin1String ( "QTWEBKIT_PLUGIN_PATH" ) );
  }

  // Qt4 Programme starten schneller wenn diese Pfade liste kleiner ist!
  QStringList iconSearchPaths;
  iconSearchPaths << QString ( OXYGEN_THEME_PATH ) << "/opt/kde4/share/icons";
  QIcon::setThemeSearchPaths ( cfg.value ( "iconthemepath", iconSearchPaths ).toStringList() );
  QIcon::setThemeName ( cfg.value ( "icontheme", "oxygen" ).toString() );

  xhtmldbgmain app ( argc, argv );
  if ( ! app.isRunning() )
    return EXIT_SUCCESS;

  QStringList trPaths ( QCoreApplication::applicationDirPath() );
  trPaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator ( 0 );
  foreach ( QString tf, trPaths )
  {
    if ( translator.load ( QString ( "xhtmldbg_%1.qm" ).arg ( QLocale().name() ), tf, "_" ) )
      break;
  }
  app.installTranslator ( &translator );

  app.newMainWindow();
  return app.exec();
}
