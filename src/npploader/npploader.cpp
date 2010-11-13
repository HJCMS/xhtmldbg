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

#include "npploader.h"
#include "nppattributes.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

NPPLoader::NPPLoader ( QObject * parent )
    : QWebPluginFactory ( parent )
{
  setObjectName ( QLatin1String ( "NPPLoader" ) );
}

void NPPLoader::registerPlugins()
{
  pluginsList.clear();
//   PluginsFinder* m_pluginsFinder = new PluginsFinder ( pluginPath, this );
//   pluginsList.append ( m_pluginsFinder->plugins() );
//   delete m_pluginsFinder;
}

QObject* NPPLoader::create ( const QString &mimeType, const QUrl &url,
                             const QStringList &argumentNames,
                             const QStringList &argumentValues ) const
{
  if ( mimeType.isEmpty() )
    return new QObject();

  QUrl::FormattingOptions urlformat ( QUrl::RemoveAuthority | QUrl::RemoveScheme
                                      | QUrl::RemoveFragment | QUrl::RemoveQuery );

  NPPAttributes* m_nppAttributes = new NPPAttributes;
  m_nppAttributes->addItem ( trUtf8( "MimeType" ), mimeType );
  m_nppAttributes->addItem ( trUtf8( "Source" ), url.toString ( urlformat ) );

  QStringListIterator it ( argumentValues );
  foreach ( QString p, argumentNames )
  {
    if ( ! p.isEmpty() && it.hasNext() )
      m_nppAttributes->addItem ( p, it.next() );
  }

  return m_nppAttributes;
}

/**
* Sende eine Leere Plugin Liste damit FLASH Plugins \b NICHT abgerufen werden!
*/
QList<QWebPluginFactory::Plugin> NPPLoader::plugins () const
{
  return pluginsList;
}

void NPPLoader::refreshPlugins()
{
  registerPlugins();
  QWebPluginFactory::refreshPlugins();
}

NPPLoader::~NPPLoader()
{
  pluginsList.clear();
}
