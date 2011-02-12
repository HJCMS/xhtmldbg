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

#include "helloworldplugin.h"
#include "helloworld.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

/* KDE */
#include <KDE/KLocale>

bool HelloWorldPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_helloWorld = new HelloWorld ( parent );
    return true;
  }
  return false;
}

QDockWidget* HelloWorldPlugin::dockwidget()
{
  return new QDockWidget;
}

void HelloWorldPlugin::setContent ( const QString &source )
{
  p_content = source;
}

void HelloWorldPlugin::setUrl ( const QUrl &url )
{
  p_url = url;
}

void HelloWorldPlugin::proccess ()
{
  if ( m_helloWorld && p_url.scheme().contains ( "http" ) )
  {
    m_helloWorld->setLabelUrl ( p_url.toString() );
    m_helloWorld->setLabelContent ( p_content );
    m_helloWorld->exec();
  }
}

xhtmldbg::PluginInfo::PluginType HelloWorldPlugin::type ()
{
  return xhtmldbg::PluginInfo::PopUp;
}

xhtmldbg::PluginInfo* HelloWorldPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "HelloWorld" ) );
  inf->setGenericName ( i18n ( "Information for this Plugin" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( i18n ( "long description for this plugin" ) );
  inf->setAuthor ( i18n ( "Your Name" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( HelloWorld, HelloWorldPlugin )
