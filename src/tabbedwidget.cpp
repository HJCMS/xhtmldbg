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

#include "tabbedwidget.h"
#include "sourceview.h"
#include "webviewer.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

TabbedWidget::TabbedWidget ( QWidget * parent )
    : QTabWidget ( parent )
{
  setObjectName ( QLatin1String ( "tabbedwidget" ) );
  setWindowTitle ( trUtf8 ( "Inspector" ) );
  setTabPosition ( QTabWidget::South );

  // Show XHTML Source
  m_sourceView = new SourceView ( this );
  insertTab ( 0, m_sourceView, trUtf8( "Source" ) );

  // WebViewer
  m_webViewer = new WebViewer ( this );
  insertTab ( 1, m_webViewer, trUtf8( "Browser" ) );

}

void TabbedWidget::addwWebViewerTab ( WebViewer* view )
{
}

TabbedWidget::~TabbedWidget()
{
}
