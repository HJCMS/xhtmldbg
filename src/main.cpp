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
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTranslator>

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "xhtmldbgmain.h"

#ifdef Q_WS_X11
/**
* Weil verschiedene Distribution der Meinung sind,
* Sie müssten den Plugin Pfad hart in WebKit einsetzen.
* An dieser Stelle der Versuch dies zu umgehen!
* QWebKit hat massive Probleme mit Plugins die nicht
* sauber Initialisiert werden können!
*
* @see http://doc.qt.nokia.com/4.6/webintegration.html
* @see http://trac.webkit.org/wiki/QtWebKitPlugins
*/
inline void xhtmldbg_restore_plugin_paths()
{
  setenv ( "MOZILLA_HOME", XHTMLDBG_PLUGIN_PATH, 1 );
  setenv ( "MOZ_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
  setenv ( "QTWEBKIT_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
}
#endif

int main ( int argc, char *argv[] )
{
#ifdef Q_WS_X11
  xhtmldbg_restore_plugin_paths();
#endif

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

  if ( app.arguments().contains ( QLatin1String ( "--help" ) )
          || app.arguments().contains ( QLatin1String ( "-h" ) ) )
  {
    app.printOptionsHelp();
    return EXIT_SUCCESS;
  }

  app.newMainWindow();
  return app.exec();
}
