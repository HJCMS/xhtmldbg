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

#include "pluginfactory.h"
#include "pluginsfinder.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QProcessEnvironment>

PluginFactory::PluginFactory ( QObject * parent )
    : QWebPluginFactory ( parent )
{
  setObjectName ( QLatin1String ( "pluginfactory" ) );

  QSettings cfg ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "xhtmldbg", this );
  pluginPath = cfg.value ( "webkit_plugin_path" ).toString();

  /*
  * QTWEBKIT_PLUGIN_PATH
  * @link http://doc.qt.nokia.com/4.6/webintegration.html
  */
  if ( ! cfg.value ( QLatin1String ( "PluginsEnabled" ), false ).toBool() )
  {
    QProcessEnvironment env ( QProcessEnvironment::systemEnvironment () );
    env.insert ( QLatin1String ( "MOZ_PLUGIN_PATH" ), pluginPath );
    env.insert ( QLatin1String ( "QTWEBKIT_PLUGIN_PATH" ), pluginPath );
  }

  if ( ! pluginPath.isEmpty() )
    registerPlugins();
}

void PluginFactory::registerPlugins()
{
  PluginsFinder* m_pluginsFinder = new PluginsFinder ( pluginPath, this );
  pluginsList << m_pluginsFinder->plugins();
  delete m_pluginsFinder;
}

QObject* PluginFactory::create ( const QString &mimeType, const QUrl &url,
                                 const QStringList &argumentNames,
                                 const QStringList &argumentValues ) const
{
  // qDebug() << "TODO" << Q_FUNC_INFO << mimeType << url << argumentNames << argumentValues;
  Q_UNUSED ( url )
  Q_UNUSED ( argumentNames )
  Q_UNUSED ( argumentValues )
  qWarning ( "(XHTMLDBG) Currently no plugin support for \"%s\"", qPrintable ( mimeType ) );
  return new QObject();
}

/**
* Sende eine Leere Plugin Liste damit FLASH Plugins \b NICHT abgerufen werden!
*/
QList<QWebPluginFactory::Plugin> PluginFactory::plugins () const
{
  return pluginsList;
}

void PluginFactory::refreshPlugins()
{
  pluginsList.clear();
  registerPlugins();
  QWebPluginFactory::refreshPlugins();
}

PluginFactory::~PluginFactory()
{}
