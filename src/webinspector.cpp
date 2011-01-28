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

#include "webinspector.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QScrollArea>

WebInspector::WebInspector ( QWebPage * startPage, QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "WebInspectorDockWidget" ) );
  setWindowTitle ( trUtf8 ( "Inspector" ) );
  setContentsMargins ( 1, 2, 1, 2 );
  // Kein Maus Menü zulassen
  setContextMenuPolicy ( Qt::NoContextMenu );

  QScrollArea* area = new QScrollArea ( this );
  area->setObjectName ( QLatin1String ( "WebInspectorScrollArea" ) );
  area->setWidgetResizable ( true );

  // jetzt QWebInspector initialisieren
  inspector = new QWebInspector ( area );
  inspector->setObjectName ( QLatin1String ( "WebInspector" ) );
  // wir brauchen diese breite für eine volle darstellung :-/
  inspector->setMinimumWidth ( 780 );
  /* Es ist unbedingt notwendig das hier eine Page gesetzt wird damit
  * beim ersten Start und einem Prüfen im Webviewer sich kein Popup
  * Fenster öffnet. Das hängt sehr wahrscheinlich mit dem QSharedPointer
  * in QWebInspector zusammmen. */
  inspector->setPage ( startPage );

  // Layout abschliessen
  area->setWidget ( inspector );
  setWidget ( area );
}

void WebInspector::contextMenuRequested ( const QPoint & )
{}

void WebInspector::setPage ( QWebPage * page )
{
  if ( ! toggleViewAction()->isChecked() || ! isVisible() )
    return;

  inspector->setPage ( page );
}

WebInspector::~WebInspector()
{}
