/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://xhtmldbg.hjcms.de, (C) 2007-2012
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
#include <iostream>
#include <cstdlib>

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

/* KDE */
#include <KDE/KCmdLineArgs>
#include <KDE/KCmdLineOptions>
#include <KDE/KLocale>
#include <KDE/KUniqueApplication>

/* project */
#include "aboutdata.h"
#include "xhtmldbgmain.h"

inline static void setPluginPaths()
{
  /* Overwrite QTWEBKIT_PLUGIN_PATH to XHTMLDBG PLUGIN Directory
  * This must forced to hide other plugins in global plugin-directories
  * !!! QtWebkit produces major fuckups with invalid plugins !!! */
  setenv ( "MOZ_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
  setenv ( "QTWEBKIT_PLUGIN_PATH", XHTMLDBG_NPP_PLUGIN_PATH, 1 );
}

int main ( int argc, char *argv[] )
{
  setPluginPaths();

  /* WARNING This class is used to store required programm information.
  * It is important to send out properties to KUniqueApplication */
  AboutData* about = new AboutData();

  KLocale::setMainCatalog ( "xhtmldbg" );
  KCmdLineArgs::init ( argc, argv, about, KCmdLineArgs::CmdLineArgKDE );

  KCmdLineOptions options;
  options.add ( "o <url>" );
  options.add ( "open <url>", ki18n ( "Open File from Path or URL" ) );
  KCmdLineArgs::addCmdLineOptions ( options );

  KUniqueApplication::addCmdLineOptions();
  if ( ! KUniqueApplication::start() )
  {
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    if ( args->allArguments().size() >= 1 )
    {
      QString srv ( "de.hjcms.xhtmldbg" );
      QString uri = args->getOption ( "o" );
      if ( uri.contains ( "://" ) )
      {
        QDBusInterface iface ( srv, "/xhtmldbg", srv );
        iface.call ( "open", uri );
      }
      uri = args->getOption ( "open" );
      if ( uri.contains ( "://" ) )
      {
        QDBusInterface iface ( srv, "/xhtmldbg", srv );
        iface.call ( "open", uri );
      }
    }
    args->clear();
    return EXIT_SUCCESS;
  }

  xhtmldbgmain* app = new xhtmldbgmain();
  return app->exec();
}
