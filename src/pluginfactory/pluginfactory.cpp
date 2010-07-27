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

/* QtCore */
#include <QtCore/QDebug>

PluginFactory::PluginFactory ( QObject * parent )
    : QWebPluginFactory ( parent )
{
  setObjectName ( QLatin1String ( "pluginfactory" ) );
}

QObject* PluginFactory::create ( const QString &mimeType, const QUrl &url,
                                 const QStringList &argumentNames,
                                 const QStringList &argumentValues ) const
{
#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "TODO" << Q_FUNC_INFO << mimeType << url << argumentNames << argumentValues;
#else
  Q_UNUSED ( url )
  Q_UNUSED ( argumentNames )
  Q_UNUSED ( argumentValues )
  qWarning ( "(XHTMLDBG) Currently no plugin support for \"%s\"", qPrintable ( mimeType ) );
#endif
  return 0x00;
}

/**
* Sende eine Leere Plugin Liste damit FLASH Plugins \b NICHT abgerufen werden!
*/
QList<QWebPluginFactory::Plugin> PluginFactory::plugins () const
{
  QList<QWebPluginFactory::Plugin> pluginList;
#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "TODO" << Q_FUNC_INFO;
#endif
  return pluginList;
}

PluginFactory::~PluginFactory()
{}
