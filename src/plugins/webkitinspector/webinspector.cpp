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
#include "webinspectorclient.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>

/* QtGui */
#include <QtGui/QScrollArea>

WebInspector::WebInspector ( QWidget * parent )
    : QDockWidget ( parent )
    , documentUrl ( QUrl() )
{
  setObjectName ( QLatin1String ( "webinspectorwidget" ) );
  setWindowTitle ( QLatin1String ( "Inspector" ) );
  setContentsMargins ( 1, 2, 1, 2 );

  QScrollArea* area = new QScrollArea ( this );
  area->setObjectName ( QLatin1String ( "webinspectorscrollarea" ) );
  area->setWidgetResizable ( true );

  /* NOTE Die dummy WebPage muss vor QWebInspector initialisiert werden */
  client = new WebInspectorClient ( this );

  // jetzt QWebInspector initialisieren
  inspector = new QWebInspector ( area );
  inspector->setObjectName ( QLatin1String ( "webkitinspector" ) );
  // wir brauchen diese breite für eine volle darstellung :-/
  inspector->setMinimumWidth ( 800 );
  inspector->setPage ( client );

  // Layout abschliessen
  area->setWidget ( inspector );
  setWidget ( area );

#if QT_VERSION >= 0x040700
  /** FIXME Seit qt>=4.7.0 hat QWebInspector ein problem mit der Initialisierung!
  * Deshalb nicht sofort anzeigen, erst nach der URL übergabe anzeigen!
  * @see WebInspector::toInspect
  */
  inspector->hide();
#endif
}

void WebInspector::toInspect ( const QUrl &url )
{
  if ( inspector->isHidden() )
    inspector->show();

  documentUrl = url;
  client->load ( url );
}

void WebInspector::setHtml ( const QString &html )
{
  if ( inspector->isHidden() )
    inspector->show();

  client->setHtml ( html, documentUrl );
}

WebInspector::~WebInspector()
{}
