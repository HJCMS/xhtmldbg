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
#include "webplugin.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QStringListIterator>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

PluginFactory::PluginFactory ( QObject * parent )
    : QWebPluginFactory ( parent )
{
  setObjectName ( QLatin1String ( "pluginfactory" ) );

  QSettings cfg ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "xhtmldbg", this );
  pluginPath = cfg.value ( "webkit_plugin_path", "/usr/lib/browser-plugins" ).toString();

  if ( ! pluginPath.isEmpty() )
    registerPlugins();
}

void PluginFactory::registerPlugins()
{
  pluginsList.clear();
  PluginsFinder* m_pluginsFinder = new PluginsFinder ( pluginPath, this );
  pluginsList.append ( m_pluginsFinder->plugins() );
  delete m_pluginsFinder;
}

QObject* PluginFactory::create ( const QString &mimeType, const QUrl &url,
                                 const QStringList &argumentNames,
                                 const QStringList &argumentValues ) const
{
  if ( mimeType.isEmpty() )
    return new QObject();

  QString items = QString ( "<dl style=\"overflow:auto;\"><dt>(XHTMLDBG) %1</dt>" )
                  .arg ( trUtf8 ( "Plugin found and initiated with predicates:" ) );
  items.append ( QString ( "<dd>type = %1</dd>" ).arg ( mimeType ) );
  items.append ( QString ( "<dd>source = %1</dd>" ).arg ( url.toString () ) );

  QStringListIterator it ( argumentValues );
  foreach ( QString p, argumentNames )
  {
    if ( ! p.isEmpty() && it.hasNext() )
      items.append ( QString ( "<dd>%1 = %2</dd>" ).arg ( p, it.next() ) );
  }
  items.append ( "</dl>" );

  QWidget* widget = new QWidget();
  widget->setStyleSheet ( "background-color:#FFFFCC;" );
  QVBoxLayout* layout = new QVBoxLayout ( widget );
  QLabel* label = new QLabel ( widget );
  label->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  label->setTextFormat ( Qt::RichText );
  label->setText ( items );
  layout->addWidget ( label );
  widget->setLayout ( layout );
  return widget;

//   foreach ( QWebPluginFactory::Plugin plugin, plugins() )
//   {
//     int size = plugin.mimeTypes.size();
//     if ( size >= 1 )
//     {
//       for ( int i = 0; i < size; i++ )
//       {
//         QString extension = url.toString( urlformat ).split( ".").last();
//         qDebug() << Q_FUNC_INFO << mimeType << url << argumentNames << argumentValues;
//         qDebug() << Q_FUNC_INFO << plugin.name  << extension << plugin.mimeTypes.at ( i ).fileExtensions;
//         if ( plugin.mimeTypes.at ( i ).fileExtensions.contains ( extension ) )
//         {
//           QWidget* widget = new QWidget();
//           widget->setStyleSheet( "background-color:yellow;" );
//           return widget;
//         }
//       }
//     }
//   }
//   return new QObject();
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
  registerPlugins();
  QWebPluginFactory::refreshPlugins();
}

PluginFactory::~PluginFactory()
{
  pluginsList.clear();
}
