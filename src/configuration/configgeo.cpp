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

#include "configgeo.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
// #include <QtGui/QVBoxLayout>

ConfigGeo::ConfigGeo ( QWidget * parent )
    : PageWidget ( trUtf8( "Geological Addresses" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_geo" ) );
  setNotice ( true );
  setCheckable ( false );
}

void ConfigGeo::load ( QSettings * )
{}

void ConfigGeo::save ( QSettings * )
{}

void ConfigGeo::defaults()
{}

ConfigGeo::~ConfigGeo()
{}
