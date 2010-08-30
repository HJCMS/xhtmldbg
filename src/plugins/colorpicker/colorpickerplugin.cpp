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

#include "colorpickerplugin.h"
#include "colorpicker.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool ColorPickerPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_colorPicker = new ColorPicker ( parent );
    return true;
  }
  return false;
}

QDockWidget* ColorPickerPlugin::dockwidget()
{
  return m_colorPicker;
}

void ColorPickerPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source );
}

void ColorPickerPlugin::setUrl ( const QUrl &url )
{
  Q_UNUSED( url );
}

void ColorPickerPlugin::proccess ()
{}

xhtmldbg::PluginInfo::PluginType ColorPickerPlugin::type ()
{
  return xhtmldbg::PluginInfo::Dock;
}

xhtmldbg::PluginInfo* ColorPickerPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "ColorPicker" ) );
  inf->setGenericName ( trUtf8 ( "Color picker" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( trUtf8 ( "This color picker will help you fine tune the hex color codes for your sites." ) );
  inf->setAuthor ( trUtf8 ( "Juergen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( ColorPicker, ColorPickerPlugin )
