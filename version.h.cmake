/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#ifndef VERSION_H
#define VERSION_H

/* KDE */
#include <KDE/KGlobal>
#include <KDE/KLocale>

/**
* XHTMLDBG Version String
*/
#define XHTMLDBG_VERSION_STRING "@XHTMLDBG_VERSION@"

/**
* Plugin Directory for Extensions and UI Designer Components
*/
#define XHTMLDBG_PLUGIN_PATH "@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@/xhtmldbg"

/**
* XHTMLDBG NPP Plugin Path
*/
#define XHTMLDBG_NPP_PLUGIN_PATH "@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@/xhtmldbg/browser-plugins"

/**
* GeoIP Database Location
*/
#define GEOIP_DATABASE_PATH "@GEOIP_DATABASE_PATH@"

/**
* ApplicationName
*/
#define XHTMLDBG_APPS_NAME "xhtmldbg"

/**
* OrganizationDomain
*/
#define XHTMLDBG_DOMAIN "hjcms.de"

/**
* Static Paths for HTML Pages
*/
#define XHTMLDBG_TEMPLATE_DIR "@CMAKE_INSTALL_PREFIX@/share/xhtmldbg/html"

#endif
