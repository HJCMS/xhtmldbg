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

#include "hostinfoplugin.h"
#include "hostinfo.h"

/* QtCore */
#include <QtCore/QDebug>

bool HostInfoPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_hostInfo = new HostInfo ( parent );
    m_hostInfo->setObjectName ( QLatin1String ( "Resolver" ) );
    return true;
  }
  return false;
}

QDockWidget* HostInfoPlugin::dockwidget()
{
  return new QDockWidget;
}

void HostInfoPlugin::setContent ( const QString &source )
{
  p_content = source;
}

void HostInfoPlugin::setUrl ( const QUrl &url )
{
  p_url = url;
}

void HostInfoPlugin::proccess ()
{
  if ( m_hostInfo && p_url.scheme().contains ( "http" ) )
    m_hostInfo->setDomain ( p_url );
}

xhtmldbg::PluginInfo::PluginType HostInfoPlugin::type ()
{
  return xhtmldbg::PluginInfo::PopUp;
}

xhtmldbg::PluginInfo* HostInfoPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "Hostinfo" ) );
  inf->setGenericName ( trUtf8 ( "Host Information" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( trUtf8 ( "Resolve IP Addresses and Host Information with given Url" ) );
  inf->setAuthor ( trUtf8 ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( HostInfo, HostInfoPlugin )
