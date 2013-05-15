/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "xoggvideo.h"
#include "xoggvideoplugin.h"

/* QtCore */
#include <QtCore/QDebug>

XOggVideoPlugin::XOggVideoPlugin ( QObject * parent )
    : QObject ( parent )
    , WidgetInterface ( QLatin1String ( "XOggVideo" ), QLatin1String ( "xOggVideo" ) )
{}

QString XOggVideoPlugin::includeFile() const
{
  return "designer/xoggvideo.h";
}

QString XOggVideoPlugin::whatsThis() const
{
  return QString::fromUtf8 ( "%1 - XQPluginWidgets" ).arg ( "XOggVideo" );
}

QWidget* XOggVideoPlugin::createWidget ( QWidget * parent )
{
  return new XOggVideo ( parent );
}
