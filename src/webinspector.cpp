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
#include "websettings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QScrollArea>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

WebInspector::WebInspector ( QWidget * parent )
    : QDockWidget ( parent )
    , m_wcfg ( new WebSettings ( parent ) )
{
  setObjectName ( QLatin1String ( "webinspectorwidget" ) );
  setWindowTitle ( QLatin1String ( "Inspector" ) );
  setContentsMargins ( 1, 2, 1, 2 );

  QScrollArea* area = new QScrollArea ( this );
  area->setObjectName ( QLatin1String ( "webinspectorscrollarea" ) );
  area->setWidgetResizable ( true );

  // jetzt QWebInspector initialisieren
  inspector = new QWebInspector ( area );
  inspector->setObjectName ( QLatin1String ( "webkitinspector" ) );
  // wir brauchen diese breite für eine volle darstellung :-/
  inspector->setMinimumWidth ( 780 );

  // Layout abschliessen
  area->setWidget ( inspector );
  setWidget ( area );
}

void WebInspector::setPage ( QWebPage * page )
{
  if ( ! toggleViewAction()->isChecked() || ! isVisible() )
    return;

  if ( page->currentFrame()->url().isEmpty() )
  {
    emit errorMessage ( trUtf8 ( "WebInspector: Update rejected - no valide url given!" ) );
    return;
  }

  if ( ! m_wcfg->attribute ( "DeveloperExtrasEnabled" ) )
    m_wcfg->inspector ( true );

  inspector->setPage ( page );
}

WebInspector::~WebInspector()
{}
