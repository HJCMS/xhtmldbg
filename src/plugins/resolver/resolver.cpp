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

#include "resolver.h"
#include "hostresolver.h"

/* QtCore */
#include <QtCore>

bool ResolverPlugin::create ( QObject * parent )
{
  if ( parent )
  {
    m_resolver = new HostResolver ( parent );
    m_resolver->setObjectName ( QLatin1String ( "Resolver" ) );
    return true;
  }
  return false;
}

void ResolverPlugin::setContent ( const QString &source )
{
  p_content = source;
}

void ResolverPlugin::setUrl ( const QUrl &url )
{
  p_url = url;
}

void ResolverPlugin::proccess ()
{
  if ( m_resolver && p_url.scheme().contains ( "http" ) )
    m_resolver->setDomain ( p_url );
}

xhtmldbg::PluginInfo::PluginType ResolverPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dialog;
}

xhtmldbg::PluginInfo* ResolverPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "Resolver" ) );
  inf->setGenericName ( trUtf8( "IP Address" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( trUtf8 ( "Resolve IP Addresses from Hostname with given Url" ) );
  inf->setAuthor ( trUtf8 ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( Resolver, ResolverPlugin )
