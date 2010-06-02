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

/* QtGui */
#include <QtGui/QScrollArea>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebView>

WebInspectorClient::WebInspectorClient ( QObject * parent )
    : QWebPage ( parent )
{
  setObjectName ( QLatin1String ( "webinspectorpage" ) );
  settings()->setAttribute ( QWebSettings::DeveloperExtrasEnabled, true );
}

WebInspectorClient::~WebInspectorClient ()
{
  settings()->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
}

WebInspector::WebInspector ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "webinspectorwidget" ) );
  setWindowTitle ( QLatin1String ( "Inspector" ) );
  setContentsMargins ( 1, 2, 1, 2 );

  QScrollArea* area = new QScrollArea ( this );
  area->setObjectName ( QLatin1String ( "webinspectorscrollarea" ) );
  area->setWidgetResizable ( true );

  /* NOTE This dummy WebPage must initialed before init QWebInspector */
  client = new WebInspectorClient ( this );

  // Now initial QWebInspector
  inspector = new QWebInspector ( area );
  inspector->setObjectName ( QLatin1String ( "webinspector" ) );
  inspector->setMinimumWidth ( 800 );
  inspector->setPage ( client );

  // Finalize layout
  area->setWidget ( inspector );
  setWidget ( area );
}

void WebInspector::toInspect ( const QUrl &url )
{
  if ( url.isValid() && url.scheme().contains ( "http" ) )
    client->mainFrame()->load ( url );
}

WebInspector::~WebInspector()
{}
