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

#include "apacheloggerplugin.h"
#include "apachelogger.h"


/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

/* KDE */
#include <KDE/KLocale>

bool ApacheLoggerPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_editor = new ApacheLogger ( parent );
    m_editor->setObjectName ( QLatin1String ( "ApacheLogger" ) );
    return true;
  }
  return false;
}

QDockWidget* ApacheLoggerPlugin::dockwidget()
{
  return m_editor;
}

void ApacheLoggerPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source )
}

void ApacheLoggerPlugin::setUrl ( const QUrl &url )
{
  Q_UNUSED ( url )
}

void ApacheLoggerPlugin::proccess ()
{}

xhtmldbg::PluginInfo::PluginType ApacheLoggerPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dock;
}

xhtmldbg::PluginInfo* ApacheLoggerPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "ApacheLogger" ) );
  inf->setGenericName ( i18n ( "watch apache logs" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( i18n ( "logging apache error.log and other" ) );
  inf->setAuthor ( i18n ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( ApacheLogger, ApacheLoggerPlugin )
