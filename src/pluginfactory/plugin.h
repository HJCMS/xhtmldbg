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

#ifndef PLUGIN_H
#define PLUGIN_H

/* QtCore */
#include <QtCore/QString>

/* QtWebKit */
#include <QtWebKit/QWebPluginFactory>

/* NSPR */
extern "C"
{
#include <npapi.h>
#ifdef HAVE_OLD_NPUPP
# include <npupp.h>
#else
# include <npfunctions.h>
#endif
}

class Plugin
{
  private:
    const QString pluginFilePath;
    NPP_t m_npInstance;

    typedef NPError ( *_NP_ShutdownPtr ) ();
    _NP_ShutdownPtr NP_ShutdownPtr;

    typedef char* ( *_NP_GetMIMEDescriptionPtr ) ( void );
    _NP_GetMIMEDescriptionPtr NP_GetMIMEDescriptionPtr;

    typedef NPError ( *_NP_GetValuePtr ) ( void* future, NPPVariable variable, void* value );
    _NP_GetValuePtr NP_GetValuePtr;

  public:
    Plugin ( const QString &path );
    QWebPluginFactory::Plugin fetchInfo();
};

#endif
