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

#include "plugger.h"
#include "interface.h"
#include "wininterface.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QPluginLoader>
#include <QtCore/QLibrary>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMetaObject>

namespace xhtmldbg
{
  Plugger::Plugger ( QObject * parent )
      : QObject ( parent )
      , p_dir ( QCoreApplication::applicationDirPath() )
  {
    setObjectName ( QLatin1String ( "xhtmldbg_plugin_loader" ) );

    QString pdn ( "xhtmldbg" ); // Plugin Verzeichnis Name
    QStringList paths; // Alle Suchverzeichnisse

    QString sp = p_dir.separator();
    paths << QString ( "%1%2%3" ).arg ( p_dir.absolutePath(), sp, QLatin1String ( "plugins" ) );

    // Für Win/Mac im Programm Verzeichnis
    paths << QString ( "%1%2%3" ).arg ( p_dir.absolutePath(), sp, pdn );

    // Im globalen Installations Verzeichniss
    paths << QString ( "%1%2%3" ).arg ( QLibraryInfo::location ( QLibraryInfo::LibrariesPath ), sp, pdn );

#ifdef Q_WS_X11
    // wird unterstützt von: Unix/Linux/Mac OS X/HP-UX
    filters << QString ( "*.so" );
#endif

#ifdef Q_OS_HPUX
    filters << QString ( "*.sl" );
#endif

#ifdef Q_OS_MAC
    filters << QString ( "*.dylib" ) << QString ( "*.bundle" );
#endif

#ifdef Q_WS_WIN
    filters << QString ( "*.dll" );
#endif

    p_dir.setSearchPaths ( "plugins", paths );
    p_dir.setNameFilters ( filters );
  }

  /**
  * Die Plugin Verzeichnisse durchgehen und nachsehen
  * ob Plugins vorhanden sind.
  */
  const QStringList Plugger::findPlugins ()
  {
    /* Verhindern das gleiche Plugins aus mehreren
    * Verzeichnissen dargestellt werden! */
    QStringList unique;
    QStringList plugins; // Liste der Plugins

    foreach ( QString path, p_dir.searchPaths ( "plugins" ) )
    {
      p_dir.setPath ( path );
      foreach ( QString p, p_dir.entryList ( filters, QDir::Files, QDir::Name ) )
      {
        QFileInfo info ( p );
        if ( QLibrary::isLibrary ( p ) && ! unique.contains ( info.baseName() ) )
        {
          unique << info.baseName();
          plugins << QString ( "%1%2%3" ).arg ( p_dir.path(), p_dir.separator(), p );
        }
      }
    }
    unique.clear();
    return plugins;
  }

  /** Suche mit Name aus @ref PlugInfo nach dem Pluginpfad
  * Wenn nicht gefunden wurde ist die Zeichenkette \b null
  */
  const QString Plugger::findPlugin ( const QString &find )
  {
    foreach ( QString plugin, findPlugins() )
    {
      if ( plugin.contains ( find, Qt::CaseInsensitive ) )
        return plugin;
    }
    return QString::null;
  }

  /**
  * Lese alle Plugin anhand der Typenangabe aus.
  */
  const QList<xhtmldbg::Interface*> Plugger::pluginsByType (
      QWidget * parent,
      xhtmldbg::PluginInfo::PluginType type )
  {
    // TODO pluginsByType
    QList<xhtmldbg::Interface*> list;
    foreach ( QString file, findPlugins() )
    {
      QPluginLoader loader ( file, this );
      QObject* plug = loader.instance();
      if ( plug )
      {
        xhtmldbg::Interface* m_interface = qobject_cast<xhtmldbg::Interface*> ( plug );
        if ( m_interface && m_interface->type() == type && m_interface->create ( parent ) )
          list << m_interface;
      }
      else
        qWarning ( "(XHTMLDBG) Pluginloader: %s", qPrintable ( loader.errorString() ) );
    }
    return list;
  }

  Plugger::~Plugger()
  {
    filters.clear();
  }
}
