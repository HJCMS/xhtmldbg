/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "inspectorwidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QLayout>
#include <QtGui/QSizePolicy>

InspectorWidget::InspectorWidget ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "inspectorwidget" ) );
  setWindowTitle ( trUtf8 ( "Inspector" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContextMenuPolicy ( Qt::NoContextMenu );
  m_scrollArea = new QScrollArea ( this );
  m_scrollArea->setWidgetResizable ( true );

  m_webInspector = new QWebInspector ( m_scrollArea );
  m_webInspector->setMinimumWidth ( 800 );
  m_webInspector->setMinimumHeight ( 250 );
  m_webInspector->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  m_webInspector->setObjectName ( "domtree" );
  m_webInspector->setContextMenuPolicy ( Qt::NoContextMenu );
  m_scrollArea->setWidget ( m_webInspector );
  setWidget ( m_scrollArea );
}

void InspectorWidget::setPage ( QWebPage * page )
{
//   if ( page )
//   {
//     m_webInspector->setPage ( page );
//     QWebSettings* ws = m_webInspector->page()->settings ();
//     ws->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
//     m_webInspector->page()->action ( QWebPage::InspectElement )->activate ( QAction::Trigger );
//   }
}

InspectorWidget::~InspectorWidget()
{}
