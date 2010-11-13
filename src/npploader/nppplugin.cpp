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

#include "nppplugin.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>

// https://developer.mozilla.org/en/Gecko_Plugin_API_Reference

NPPPlugin::NPPPlugin ( const QString &realPath )
    : m_isEnabled ( false )
    , m_isLoaded ( false )
    , m_loadCount ( 0 )
    , m_pluginInfo ( QFileInfo ( realPath ) )
{
}

void NPPPlugin::initializeBrowserFuncs()
{
  memset ( &m_browserFuncs, 0, sizeof ( m_browserFuncs ) );
  m_browserFuncs.size = sizeof ( m_browserFuncs );
  m_browserFuncs.version = NP_VERSION_MINOR;
  m_browserFuncs.geturl = NPN_GetURL;
  m_browserFuncs.posturl = NPN_PostURL;
  m_browserFuncs.requestread = NPN_RequestRead;
  m_browserFuncs.newstream = NPN_NewStream;
  m_browserFuncs.write = NPN_Write;
  m_browserFuncs.destroystream = NPN_DestroyStream;
  m_browserFuncs.status = NPN_Status;
  m_browserFuncs.uagent = NPN_UserAgent;
  m_browserFuncs.memalloc = NPN_MemAlloc;
  m_browserFuncs.memfree = NPN_MemFree;
  m_browserFuncs.memflush = NPN_MemFlush;
  m_browserFuncs.reloadplugins = NPN_ReloadPlugins;
  m_browserFuncs.geturlnotify = NPN_GetURLNotify;
  m_browserFuncs.posturlnotify = NPN_PostURLNotify;
  m_browserFuncs.getvalue = NPN_GetValue;
  m_browserFuncs.setvalue = NPN_SetValue;
  m_browserFuncs.invalidaterect = NPN_InvalidateRect;
  m_browserFuncs.invalidateregion = NPN_InvalidateRegion;
  m_browserFuncs.forceredraw = NPN_ForceRedraw;
  m_browserFuncs.pluginthreadasynccall = NPN_PluginThreadAsyncCall;
}

bool NPPPlugin::load()
{
  if ( ! m_isEnabled )
    return false;

  if ( m_isLoaded )
  {
    m_loadCount++;
    return true;
  }

  return false;
}

void NPPPlugin::unload()
{
}

bool NPPPlugin::isEnabled() const
{
  return m_isEnabled;
}

void NPPPlugin::setEnabled ( bool b )
{
  m_isEnabled = b;
}

NPPPlugin::~NPPPlugin()
{}
