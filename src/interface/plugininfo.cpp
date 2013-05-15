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

#include "plugininfo.h"
#include "wininterface.h"

namespace xhtmldbg
{
  PluginInfo::PluginInfo ( QObject * parent, PluginType type )
      : QObject ( parent )
      , pluginType ( type )
  {}

  /**
  * Den realen Namen abfragen
  */
  const QString PluginInfo::getName()
  {
    return Name;
  }

  /**
  * Menü Titel abfrage
  */
  const QString PluginInfo::getGenericName()
  {
    return GenericName;
  }

  /**
  * Versions abfrage.
  */
  const QString PluginInfo::getVersion()
  {
    return Version;
  }

  /**
  * Beschreibungs abfrage.
  */
  const QString PluginInfo::getDescription()
  {
    return Description;
  }

  /**
  * Plugin Autor abfrage
  */
  const QString PluginInfo::getAuthor()
  {
    return Author;
  }

  /**
  * Den realen Plugin Name setzen
  */
  void PluginInfo::setName ( const QString &n )
  {
    Name = n;
  }

  /**
  * Den Namen für den Menü Eintrag einfügen.
  */
  void PluginInfo::setGenericName ( const QString &n )
  {
    GenericName = n;
  }

  /**
  * Die Versions Nummer in from eines Textes einfügen.
  */
  void PluginInfo::setVersion ( const QString &v )
  {
    Version = v;
  }

  /**
  * Die Beschreibung einfügen
  */
  void PluginInfo::setDescription ( const QString &d )
  {
    Description = d;
  }

  /**
  * Den Autor einfügen
  */
  void PluginInfo::setAuthor ( const QString &a )
  {
    Author = a;
  }

  /**
  * Gibt die DBus Interface Schnittstelle zurück
  */
  xhtmldbg::WinInterface* PluginInfo::dbusInterface ( const QDBusConnection &dbus, QObject * parent )
  {
    return new WinInterface ( dbus, parent );
  }

} /* eof namespace xhtmldbg */
