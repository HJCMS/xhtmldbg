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
#include "sourcewidget.h"
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
  m_sourceWidget = new SourceWidget ( this );
  insertTab ( 0, m_sourceWidget, trUtf8 ( "Source" ) );

  // WebViewer
  m_webViewer = new WebViewer ( this );
  insertTab ( 1, m_webViewer, trUtf8 ( "Browser" ) );

  // TODO
  QFile fp ( "/home/heinemann/hjcms/QTidy/tests/in_588061.html" );
  if ( fp.open ( QFile::ReadOnly ) )
  {
    QTextStream rc ( &fp );
    QByteArray buffer = rc.device()->readAll();
    fp.close();
    if ( ! buffer.isEmpty() )
    {
      QTextCodec* codec = QTextCodec::codecForHtml ( buffer, QTextCodec::codecForName ( "UTF-8" ) );
      QString data = codec->toUnicode ( buffer );
      m_sourceWidget->setSource ( data );
    }
  }

  connect ( m_webViewer, SIGNAL ( urlChanged ( const QUrl & ) ),
            this, SIGNAL ( loadUrl ( const QUrl & ) ) );
}

void TabbedWidget::check()
{
  m_sourceWidget->check();
}

void TabbedWidget::format()
{
  m_sourceWidget->format();
}

void TabbedWidget::setUrl ( const QUrl &url )
{
  m_webViewer->setUrl ( url );
}

TabbedWidget::~TabbedWidget()
{
}
