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

#include "webkitinspectorplugin.h"
#include "webinspector.h"

/* QtCore */
#include <QtCore/QDebug>

bool WebKitInspectorPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_webInspector = new WebInspector ( parent );
    return true;
  }
  return false;
}

QDockWidget* WebKitInspectorPlugin::dockwidget()
{
  return m_webInspector;
}

void WebKitInspectorPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source );
}

void WebKitInspectorPlugin::setUrl ( const QUrl &url )
{
  m_webInspector->toInspect ( url );
}

void WebKitInspectorPlugin::proccess ()
{}

xhtmldbg::PluginInfo::PluginType WebKitInspectorPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dock;
}

xhtmldbg::PluginInfo* WebKitInspectorPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "WebKit Inspector" ) );
  inf->setGenericName ( trUtf8 ( "Inspector" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( trUtf8 ( "QWebKit's Inspector Backend" ) );
  inf->setAuthor ( QString::fromUtf8 ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( WebInspector, WebKitInspectorPlugin )
