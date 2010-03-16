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
#include "xhtmldbg.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "cookieacceptdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QHelpEvent>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QToolTip>

/* QtWebKit */
#include <QtWebKit/QWebHitTestResult>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

Viewer::Viewer ( QWidget * parent )
    : QWebView ( parent )
    , cookieAlreadyAdd ( false )
{
  if ( objectName().isEmpty() )
    setObjectName ( "webviewer" );

  setContentsMargins ( 0, 0, 0, 0 );
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  NetworkAccessManager* netManager = xhtmldbg::instance()->networkAccessManager();
  cookieManager = xhtmldbg::instance()->cookieManager();

  m_page = new Page ( netManager, this );
  setPage ( m_page );

  connect ( cookieManager, SIGNAL ( cookiesRequest ( const QUrl & ) ),
            this, SLOT ( cookiesRequest ( const QUrl & ) ) );

  connect ( this, SIGNAL ( linkClicked ( const QUrl & ) ),
            netManager, SLOT ( setUrl ( const QUrl & ) ) );

  connect ( this, SIGNAL ( loadStarted () ),
            this, SLOT ( cursorwait () ) );

  connect ( this, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( cursorFinished ( bool ) ) );

  connect ( m_page, SIGNAL ( linkHovered ( const QString &, const QString &, const QString & ) ),
            this, SLOT ( linkInfos ( const QString &, const QString &, const QString & ) ) );
}

void Viewer::openCookieRequestDialog ( const QUrl &cookieUrl )
{
  if ( ! cookieManager )
    return;

  CookieAcceptDialog cookiediag ( cookieUrl, this );
  if ( cookiediag.exec() )
    cookieManager->reload();
}

void Viewer::findChildNode ( const QPoint &p )
{
  if ( ! p.isNull() )
    emit hitTestResult ( page()->currentFrame()->hitTestContent ( p ).element() );
}

void Viewer::cursorwait ()
{
  setCursor ( Qt::WaitCursor );
}

void Viewer::cursorFinished ( bool )
{
  setCursor ( Qt::ArrowCursor );
}

void Viewer::bookmark()
{
  emit addBookmark ( url(), title() );
}

void Viewer::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* menu = m_page->createStandardContextMenu();
  QAction* add = menu->addAction ( trUtf8 ( "Bookmark" ) );
  add->setObjectName ( QLatin1String ( "addbookmarkaction" ) );
  add->setIcon ( QIcon::fromTheme ( QLatin1String ( "bookmark-new" ) ) );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( bookmark() ) );
  menu->exec ( e->globalPos() );
  delete menu;
}

void Viewer::mousePressEvent ( QMouseEvent * ev )
{
  findChildNode ( ev->pos() );
  QWebView::mousePressEvent ( ev );
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
      mw->addViewerTab ( view );
      return view;
    }
    w = w->parent();
  }
  return this;
}

void Viewer::unsupportedContent ( QNetworkReply *reply )
{
  qDebug() << "Unsupported Content:" << reply->url();
  if ( reply->error() == QNetworkReply::NoError )
    return;
}

void Viewer::cookiesRequest ( const QUrl &u )
{
  QString pageHost ( url().host() );
  QString cookieHost ( u.host() );

  if ( pageHost.contains ( cookieHost ) && ! cookieAlreadyAdd )
  {
    cookieAlreadyAdd = true;
    openCookieRequestDialog ( u );
  }
}

void Viewer::findKeyword ( const QString &word )
{
  findText ( word, QWebPage::HighlightAllOccurrences );
}

void Viewer::openUrl ( const QUrl &url )
{
  setUrl ( url );
}

const QString Viewer::source()
{
  return m_page->xhtmlSource();
}

/** TODO Extra ToolTips for more Information about links in WebPages */
void Viewer::linkInfos ( const QString &link, const QString &title, const QString & )
{
  if ( ! link.isEmpty() )
    emit statusBarMessage ( link );

  if ( title.isEmpty() )
    return;

  // qDebug() << Q_FUNC_INFO << point;
  //   QHelpEvent ev ( QEvent::ToolTip, point, pos() );
  // link << content;
  QPoint point = cursor ().pos();
  QToolTip::showText ( point, title, this, QRect ( point, QSize ( 20, 20 ) ) );
}

Viewer::~Viewer()
{
}
