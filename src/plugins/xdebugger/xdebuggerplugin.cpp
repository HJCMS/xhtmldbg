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

#include "xdebuggerplugin.h"
#include "xdebugger.h"

/* QtCore */
#include <QtCore>

bool XDebuggerPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_xdebugger = new XDebugger ( parent );
    m_xdebugger->setObjectName ( QLatin1String ( "plugin_xdebugger_dockwidget" ) );
    return true;
  }
  return false;
}

QDockWidget* XDebuggerPlugin::dockwidget()
{
  return  m_xdebugger;
}

void XDebuggerPlugin::setContent ( const QString &source )
{
  p_content = source;
}

void XDebuggerPlugin::setUrl ( const QUrl &url )
{
  p_url = url;
}

void XDebuggerPlugin::proccess ()
{
  if ( m_xdebugger && p_url.scheme().contains ( "http" ) )
    m_xdebugger->openSessionWithIdKey ( p_url.toString() );
}

xhtmldbg::PluginInfo::PluginType XDebuggerPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dock;
}

xhtmldbg::PluginInfo* XDebuggerPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "XDebugger" ) );
  inf->setGenericName ( trUtf8( "XDebug" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( trUtf8 ( "XDebugger Plugin" ) );
  inf->setAuthor ( trUtf8 ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( XDebugger, XDebuggerPlugin )
