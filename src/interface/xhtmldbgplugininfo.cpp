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

#include "xhtmldbgplugininfo.h"

namespace xhtmldbg
{
  PluginInfo::PluginInfo ( QObject * parent, PluginType type )
      : QObject ( parent )
      , pluginType ( type )
  {}

  const QString PluginInfo::getName()
  {
    return Name;
  }

  const QString PluginInfo::getVersion()
  {
    return Version;
  }

  const QString PluginInfo::getDescription()
  {
    return Description;
  }

  const QString PluginInfo::getAuthor()
  {
    return Author;
  }

  void PluginInfo::setName ( const QString &n )
  {
    Name = n;
  }

  void PluginInfo::setVersion ( const QString &v )
  {
    Version = v;
  }

  void PluginInfo::setDescription ( const QString &d )
  {
    Description = d;
  }

  void PluginInfo::setAuthor ( const QString &a )
  {
    Author = a;
  }

} /* eof namespace xhtmldbg */
