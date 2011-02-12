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

#include <iostream>
#include <cstdlib>

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QGlobalStatic>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTranslator>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>

/* KDE */
#include <KDE/KAboutData>
#include <KDE/KAboutPerson>
#include <KDE/KCmdLineArgs>
#include <KDE/KCmdLineOptions>
#include <KDE/KGlobal>
#include <KDE/KLocale>

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "aboutdata.h"
#include "xhtmldbgmain.h"

inline static void setPluginPaths()
{
  /* Overwrite QTWEBKIT_PLUGIN_PATH to XHTMLDBG PLUGIN Directory
  * This must forced to hide other plugins in global plugin-directories
  * !!! QtWebkit produces major fuckups with invalid plugins !!!
  */
  setenv ( "MOZ_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
  setenv ( "QTWEBKIT_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
}

int main ( int argc, char *argv[] )
{
  setPluginPaths();

  AboutData about;

  // Initialize command line args
  KCmdLineArgs::init ( argc, argv, &about );

  // Define the command line options
  KCmdLineOptions options;
  options.add ( "o <url>" ).add ( "open <url>", ki18n ( "Open File from Path or URL" ) );
  options.add ( "f" ).add ( "failsafe", ki18n ( "Disable Plugins and loading the Default Url" ) );

  // Register the supported options
  KCmdLineArgs::addCmdLineOptions ( options );

  // start application
  xhtmldbgmain app ( argc, argv );
  if ( ! app.isRunning() )
    return EXIT_SUCCESS;

  QStringList trPaths ( app.applicationDirPath() );
  trPaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator ( &app );
  foreach ( QString tf, trPaths )
  {
    if ( translator.load ( QString ( "xhtmldbg_%1.qm" ).arg ( QLocale().name() ), tf, "_" ) )
      break;
  }
  app.installTranslator ( &translator );

  KGlobal::locale()->insertCatalog (XHTMLDBG_APPS_NAME );

  app.newMainWindow();
  return app.exec();
}
