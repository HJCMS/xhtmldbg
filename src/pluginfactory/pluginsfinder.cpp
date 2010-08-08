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

#include "pluginsfinder.h"
#include "plugin.h"

/* QtCore */
#include <QtCore/QDebug>

PluginsFinder::PluginsFinder ( const QString &path, QObject * parent )
    : QObject ( parent )
    , pluginDirectory ( path, QString::fromUtf8 ( "*.so" ), QDir::Name, QDir::Files )
{
  setObjectName ( QLatin1String ( "pluginsfinder" ) );
  if ( ! pluginDirectory.exists() )
    qWarning ( "(XHTMLDBG) Can not open Plugin \"%s\" Directory", qPrintable ( path ) );
}

/**
* Erstellt eine List mit allen auffindbaren Netscape Plugins
*/
QList<QWebPluginFactory::Plugin> PluginsFinder::plugins()
{
  QList<QWebPluginFactory::Plugin> list;
  if ( pluginDirectory.exists() )
  {
    foreach ( QString lib, pluginDirectory.entryList () )
    {
      Plugin plugin ( ( pluginDirectory.path() + pluginDirectory.separator() + lib ) );
      QWebPluginFactory::Plugin pl = plugin.fetchInfo();
      if ( ! pl.name.isEmpty() )
        list << pl;
    }
  }
  return list;
}
