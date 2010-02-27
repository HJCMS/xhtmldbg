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

#include "webviewer.h"
#include "viewer.h"
#include "historymanager.h"
#include "historyitem.h"

#include <QtCore/QDebug>

#include <QtWebKit/QWebFrame>

static inline const QString blank_html()
{
  QString html ( "<html><head><title>QTidy</title></head><body><div>New Document</div></body></html>" );
  return html;
}

WebViewer::WebViewer ( QWidget * parent )
    : QTabWidget ( parent )
{
  if ( objectName().isEmpty() )
    setObjectName ( "webviewer" );

  setContentsMargins ( 0, 0, 0, 0 );

  m_viewer = new Viewer ( this );
  addTab ( m_viewer, trUtf8 ( "blank" ) );

  setSignals ( m_viewer );

  connect ( this, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( pretended ( int ) ) );
}

void WebViewer::setSignals ( Viewer * view )
{
  if ( ! view )
    return;

  connect ( view, SIGNAL ( titleChanged ( const QString & ) ),
            this, SLOT ( updateTabTitle ( const QString & ) ) );

  connect ( view, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ),
            this, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ) );

  connect ( view, SIGNAL ( urlChanged ( const QUrl & ) ),
            this, SIGNAL ( urlChanged ( const QUrl & ) ) );

  connect ( view, SIGNAL ( loadFinished ( bool ) ),
            this, SIGNAL ( loadFinished ( bool ) ) );

  connect ( view, SIGNAL ( scriptConsoleMessage ( int, const QString & ) ),
            this, SIGNAL ( scriptConsoleMessage ( int, const QString & ) ) );

}

Viewer* WebViewer::activeView()
{
  QObject* w = currentWidget();
  while ( w )
  {
    if ( Viewer* mw = qobject_cast<Viewer*> ( w ) )
      return mw;

    w = w->parent();
  }
  return m_viewer;
}

void WebViewer::updateTabTitle ( const QString &title )
{
  int max = 20;
  int index = currentIndex();
  QString pr ( title );
  if ( pr.isEmpty() )
  {
    pr.append ( activeView()->url().host() );
  }
  else if ( title.size() >= max )
  {
    pr.resize ( max );
    pr.append ( "..." );
  }
  setTabText ( index, pr );
  setTabToolTip ( index, title );
  setTabWhatsThis ( index, title );
}

void WebViewer::pretended ( int index )
{
  Q_UNUSED ( index )
  emit urlChanged ( activeView()->url() );
}

void WebViewer::addNewViewerTab ( Viewer *view )
{
  if ( ! view )
    return;

  setSignals ( view );

  QUrl uri ( view->url() );
  QString title = uri.host().isEmpty() ? trUtf8 ( "Startpage" ) : uri.host();
  int index = addTab ( view, title );
  setCurrentIndex ( index );
}

void WebViewer::addEmptyViewerTab ()
{
  Viewer* view = new Viewer ( this );
  view->setHtml ( blank_html() );
  addNewViewerTab ( view );
}

void WebViewer::setUrl ( const QUrl &url )
{
  activeView()->setUrl ( url );
}

void WebViewer::keywords ( const QStringList &words )
{
  foreach ( QString w, words )
  {
    activeView()->findText ( w, QWebPage::HighlightAllOccurrences );
  }
}

void WebViewer::refresh ()
{
  activeView()->reload();
}

void WebViewer::back ()
{
  activeView()->back();
}

void WebViewer::forward ()
{
  activeView()->forward();
}

const QString WebViewer::toHtml()
{
  return activeView()->page()->currentFrame()->toHtml ();
}

const QWebElement WebViewer::toWebElement()
{
  return activeView()->page()->currentFrame()->documentElement ();
}

WebViewer::~WebViewer()
{
}
