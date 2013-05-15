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

#include "webinspector.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QScrollArea>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

/* KDE */
#include <KDE/KLocale>

WebInspector::WebInspector ( QWebPage * startPage, QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "WebInspectorDockWidget" ) );
  setWindowTitle ( i18n ( "WebInspector" ) );
  // Kein Maus Menü zulassen
  setContextMenuPolicy ( Qt::NoContextMenu );
  setContentsMargins ( 1, 2, 1, 2 );

  QScrollArea* area = new QScrollArea ( this );
  area->setObjectName ( QLatin1String ( "WebInspectorScrollArea" ) );
  area->setWidgetResizable ( true );

  // jetzt QWebInspector initialisieren
  m_inspector = new QWebInspector ( area );
  m_inspector->setObjectName ( QLatin1String ( "Inspector" ) );
  m_inspector->setContextMenuPolicy ( Qt::NoContextMenu );
  m_inspector->setMinimumWidth ( 780 );
  m_inspector->setAttribute ( Qt::WA_UnderMouse, 0 );
  m_inspector->setShortcutEnabled ( false );
  /* Es ist unbedingt notwendig das hier eine Page gesetzt wird damit
  * beim ersten Start und einem Prüfen im Webviewer sich kein Popup
  * Fenster öffnet. Das hängt sehr wahrscheinlich mit dem QSharedPointer
  * in QWebInspector zusammmen. */
  m_inspector->setPage ( startPage );

  // Layout abschliessen
  area->setWidget ( m_inspector );
  setWidget ( area );
}

void WebInspector::setPage ( QWebPage * page )
{
  if ( ! toggleViewAction()->isChecked() || ! isVisible() )
    return;

  m_inspector->setPage ( page );
}

WebInspector::~WebInspector()
{}
