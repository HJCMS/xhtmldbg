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

#include "viewer.h"
#include "webviewer.h"
#include "page.h"

Viewer::Viewer ( QWidget * parent )
    : QWebView ( parent )
{
  if ( objectName().isEmpty() )
    setObjectName ( "webviewer" );

  setContentsMargins ( 0, 0, 0, 0 );

  m_page = new Page ( this );
  setPage ( m_page );

  setUrl( QUrl( "http://webmast.jh" ) );

  connect ( this, SIGNAL ( loadStarted () ),
            this, SLOT ( cursorwait () ) );

  connect ( this, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( cursorFinished () ) );
}

void Viewer::cursorwait ()
{
  setCursor ( Qt::WaitCursor );
}

void Viewer::cursorFinished ()
{
  setCursor ( Qt::ArrowCursor );
}

Viewer* Viewer::createWindow ( QWebPage::WebWindowType t )
{
  if ( QWebPage::WebModalDialog == t )
    return this;

  QObject* w = this->parent();
  while ( w )
  {
    if ( WebViewer* mw = qobject_cast<WebViewer*> ( w ) )
    {
      Viewer *view = new Viewer ( mw );
      mw->addNewViewerTab ( view );
      return view;
    }
    w = w->parent();
  }
  return this;
}

Viewer::~Viewer()
{
}
