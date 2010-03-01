/**
* This file is part of the QTidy project
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
#include <QtCore/QTranslator>

#include "xhtmldbg.h"

int main ( int argc, char *argv[] )
{
  xhtmldbg app ( argc, argv );
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
