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

#include "plugin.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QLibrary>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

typedef NPError ( *_NP_ShutdownPtr ) ();
_NP_ShutdownPtr NP_ShutdownPtr;

typedef char* ( *_NP_GetMIMEDescriptionPtr ) ( void );
_NP_GetMIMEDescriptionPtr NP_GetMIMEDescriptionPtr;

typedef NPError ( *_NP_GetValuePtr ) ( void* future, NPPVariable variable, void* value );
_NP_GetValuePtr NP_GetValuePtr;

/**
* @class Plugin
*/
Plugin::Plugin ( const QString &path )
    : pluginFilePath ( path )
{
  memset ( &m_npInstance, 0, sizeof ( NPP_t ) );
  m_npInstance.ndata = this;
}

/**
* Lese Plugin Informationen und schließe danach wieder!
*/
QWebPluginFactory::Plugin Plugin::fetchInfo()
{
  char* pluginName = 0;
  char* pluginDesc = 0;
  QWebPluginFactory::Plugin info;
  QList<QWebPluginFactory::MimeType> p_mimeTypes;

  QLibrary nsplugin ( pluginFilePath );
  NP_GetMIMEDescriptionPtr = ( _NP_GetMIMEDescriptionPtr ) ( nsplugin.resolve ( "NP_GetMIMEDescription" ) );
  NP_GetValuePtr = ( _NP_GetValuePtr ) nsplugin.resolve ( "NP_GetValue" );
  NP_ShutdownPtr = ( _NP_ShutdownPtr ) nsplugin.resolve ( "NP_Shutdown" );
  if ( ! NP_GetMIMEDescriptionPtr || ! NP_GetValuePtr || ! NP_ShutdownPtr )
  {
    qWarning ( "(XHTMLDBG) probably \"%s\" not a netscape plugin", qPrintable ( pluginFilePath ) );
    return info;
  }

  NP_GetValuePtr ( &m_npInstance, NPPVpluginNameString, ( void * ) &pluginName );
  info.name = QString ( pluginName );

  NP_GetValuePtr ( &m_npInstance, NPPVpluginDescriptionString, ( void * ) &pluginDesc );
  info.description = QString ( pluginDesc );

  QString pluginInfo ( NP_GetMIMEDescriptionPtr() );
  foreach ( QString desc, pluginInfo.split ( ";" ) )
  {
    QStringList array = desc.split ( QRegExp ( ":" ) );
    QWebPluginFactory::MimeType mType;
    mType.name = array.first();
    mType.description = array.last();

    if ( array.size() == 3 )
      mType.fileExtensions = array.at ( 1 ).split ( "," );

    p_mimeTypes.append ( mType );
  }
  info.mimeTypes.append ( p_mimeTypes );

  NP_ShutdownPtr();
  return info;
}

// EOF
