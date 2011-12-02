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

#include "selfhtmlsidebarplugin.h"
#include "selfhtmlsidebar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

/* KDE */
#include <KDE/KLocale>

bool SelfHtmlSidebarPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_sideBar = new SelfHtmlSidebar ( parent );
    return true;
  }
  return false;
}

QDockWidget* SelfHtmlSidebarPlugin::dockwidget()
{
  return m_sideBar;
}

void SelfHtmlSidebarPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source );
}

void SelfHtmlSidebarPlugin::setUrl ( const QUrl &url )
{
  Q_UNUSED ( url );
}

void SelfHtmlSidebarPlugin::proccess ()
{}

xhtmldbg::PluginInfo::PluginType SelfHtmlSidebarPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dock;
}

xhtmldbg::PluginInfo* SelfHtmlSidebarPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "SelfHtmlSidebar" ) );
  inf->setGenericName ( i18n ( "SELFHTML" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( i18n ( "Sidebars for HTML, CSS, JavaScript/DOM, XSLT/XPath, Perl and SELFHTML." ) );
  inf->setAuthor ( QString::fromUtf8 ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( SelfHtmlSidebar, SelfHtmlSidebarPlugin )
