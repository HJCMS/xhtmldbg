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

#include "rfcbrowserplugin.h"
#include "rfcbrowser.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

/* KDE */
#include <KDE/KLocale>

bool RFCBrowserPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_rfcBrowser = new RFCBrowser ( parent );
    m_rfcBrowser->setObjectName ( QLatin1String ( "RFCBrowser" ) );
    return true;
  }
  return false;
}

QDockWidget* RFCBrowserPlugin::dockwidget()
{
  return new QDockWidget;
}

void RFCBrowserPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source )
}

void RFCBrowserPlugin::setUrl ( const QUrl &url )
{
  Q_UNUSED ( url )
}

void RFCBrowserPlugin::proccess ()
{
  m_rfcBrowser->open();
}

xhtmldbg::PluginInfo::PluginType RFCBrowserPlugin::type ()
{
  return xhtmldbg::PluginInfo::PopUp;
}

xhtmldbg::PluginInfo* RFCBrowserPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "RFC Browser" ) );
  inf->setGenericName ( i18n ( "RFC Document Browser" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( i18n ( "Browse RFC Documents" ) );
  inf->setAuthor ( i18n ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( RFCBrowser, RFCBrowserPlugin )
