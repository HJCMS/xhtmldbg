/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "widgetinterface.h"

WidgetInterface::WidgetInterface ( const QString &clName, const QString &objName )
    : pluginClassName ( clName )
    , pluginObjectName ( objName )
    , initialized ( false )
{}

bool WidgetInterface::isContainer() const
{
  return false;
}

bool WidgetInterface::isInitialized() const
{
  return initialized;
}

QIcon WidgetInterface::icon() const
{
  return  QIcon::fromTheme ( QLatin1String ( "xhtmldbg" ) );
}

QString WidgetInterface::codeTemplate() const
{
  return QString();
}

QString WidgetInterface::domXml() const
{
  return QString::fromUtf8 ( "<widget class=\"%1\" name=\"%2\">\n"
" <property name=\"geometry\">\n"
"  <rect>\n"
"   <x>0</x>\n"
"   <y>0</y>\n"
"   <width>250</width>\n"
"   <height>250</height>\n"
"  </rect>\n"
" </property>\n"
" <property name=\"toolTip\" >\n"
"  <string>%1 Plugin</string>\n"
" </property>\n"
"</widget>\n" ).arg ( pluginClassName, pluginObjectName );
}

QString WidgetInterface::group() const
{
  return QLatin1String ( "xhtmldbg" );
}

QString WidgetInterface::name() const
{
  return pluginClassName;
}

QString WidgetInterface::toolTip() const
{
  return QString::null;
}

void WidgetInterface::initialize ( QDesignerFormEditorInterface * formEditor )
{
  Q_UNUSED ( formEditor )
  if ( initialized )
    return;

  initialized = true;
}

// EOF
