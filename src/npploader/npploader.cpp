/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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
#include "nppplugin.h"
#include "nppattributes.h"
#include "nppfailurewidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

/* QtWebKit */
#include <QtWebKit/QWebPluginFactory>

/* KDE */
#include <KDE/KLocale>

NPPLoader::NPPLoader ( QObject * parent )
    : KWebPluginFactory ( parent )
{
  setObjectName ( QLatin1String ( "NPPLoader" ) );
  pluginsList.clear();
}

void NPPLoader::registerPlugins()
{
  pluginsList.clear();
}

QObject* NPPLoader::create ( const QString &mimeType, const QUrl &url,
                             const QStringList &argumentNames,
                             const QStringList &argumentValues ) const
{
  if ( mimeType.isEmpty() )
    return new NPPFailureWidget ( i18n ( "Missing Mime-Type Declaration!" ) );
  else if ( ! url.isValid() )
    return new NPPFailureWidget ( i18n ( "Invalid URL for this Plugin Request!" ) );

  QUrl::FormattingOptions urlformat ( QUrl::RemoveAuthority | QUrl::RemoveScheme
                                      | QUrl::RemoveFragment | QUrl::RemoveQuery );

  NPPAttributes* m_nppAttributes = new NPPAttributes;
  m_nppAttributes->addItem ( i18n ( "MimeType" ), mimeType );
  m_nppAttributes->addItem ( i18n ( "Source" ), url.toString ( urlformat ) );

  QStringListIterator it ( argumentValues );
  foreach ( QString p, argumentNames )
  {
    if ( ! p.isEmpty() && it.hasNext() )
      m_nppAttributes->addItem ( p, it.next() );
  }

#ifdef DEBUG_VERBOSE
  qDebug() << Q_FUNC_INFO <<  mimeType;
#endif

  return m_nppAttributes;
}

/**
* Sende eine Leere Plugin Liste damit FLASH Plugins \b NICHT abgerufen werden!
*/
QList<KWebPluginFactory::Plugin> NPPLoader::plugins () const
{
  return pluginsList;
}

void NPPLoader::refreshPlugins()
{
  registerPlugins();
  KWebPluginFactory::refreshPlugins();
}

bool NPPLoader::supportsExtension (Extension extension) const
{
#ifdef DEBUG_VERBOSE
  qDebug() << Q_FUNC_INFO <<  extension;
#else
	Q_UNUSED (extension)
#endif
  return false;
}

NPPLoader::~NPPLoader()
{
  pluginsList.clear();
}
