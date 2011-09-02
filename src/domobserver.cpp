/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "domobserver.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QUrl>

/* QtGui */
// #include <QtGui/QAction>
#include <QtGui/QTextEdit>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebHitTestResult>
#include <QtWebKit/QWebPage>

/* KDE */
#include <KDE/KWebView>
#include <KDE/KLocale>
#include <KDE/KIcon>

DomObserver::DomObserver ( QWebPage * page, QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "DomObserver" ) );
  setWindowTitle ( i18n ( "Observer" ) );
  setContentsMargins ( 1, 2, 1, 2 );
  setContextMenuPolicy ( Qt::NoContextMenu );

  QTextEdit* m_textView = new QTextEdit ( this );
  setWidget ( m_textView );

  m_textView->setPlainText ( page->mainFrame()->renderTreeDump() );
}

void DomObserver::startObserver()
{}

void DomObserver::stopObserver()
{}

void DomObserver::setPage ( QWebPage* page )
{
  if ( page->mainFrame()->documentElement().isNull() )
    return;

  // m_textView->setPlainText ( page->mainFrame()->renderTreeDump() );
}

DomObserver::~DomObserver()
{}
