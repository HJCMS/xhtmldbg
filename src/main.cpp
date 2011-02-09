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

/* KDE */
#include <KDE/KAboutData>
#include <KDE/KAboutPerson>
#include <KDE/KCmdLineArgs>
#include <KDE/KGlobal>
#include <KDE/KLocale>

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "xhtmldbgmain.h"

inline static const QByteArray aboutMail()
{
  QByteArray m ( "nospam" );
  m.append ( "@" );
  m.append ( "hjcms.de" );
  return m;
}

inline static const QByteArray homePage()
{
  return QByteArray ( "http://xhtmldbg.hjcms.de" );
}

inline static const QByteArray appsName()
{
  return QByteArray ( XHTMLDBG_APPS_NAME );
}

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
  KLocalizedString maintainer = ki18n ( "Juergen Heinemann (Undefined)" );
  KAboutData about ( appsName(), appsName(), ki18n ( appsName() ),
                     QByteArray ( XHTMLDBG_VERSION_STRING ),
                     ki18n ( "A XHTML/HTML Debugger" ),
                     KAboutData::License_LGPL_V3,
                     ki18n ( "Copyright (C) 2007-2011, Juergen Heinemann (Undefined)" ),
                     ki18n ( "XHTML/HTML Debugger for KDEv4" ),
                     homePage(), aboutMail() );

  about.setOrganizationDomain ( XHTMLDBG_DOMAIN );
  about.setProgramIconName ( appsName() );
  about.setProductName ( appsName() );
  about.setVersion ( QByteArray ( XHTMLDBG_VERSION_STRING ) );
  about.addAuthor ( maintainer, ki18n ( "developer" ), aboutMail(), homePage(), QByteArray ( "undefined" ) );
  about.setTranslator ( maintainer, ki18n ( aboutMail() ) );
  KCmdLineArgs::init ( argc, argv, &about );

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

  KGlobal::locale()->insertCatalog ( appsName() );

  if ( app.arguments().contains ( QLatin1String ( "--help" ) )
          || app.arguments().contains ( QLatin1String ( "-h" ) ) )
  {
    app.printOptionsHelp();
    return EXIT_SUCCESS;
  }

  app.newMainWindow();
  return app.exec();
}
