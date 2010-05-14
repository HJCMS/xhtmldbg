/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef VIEWER_H
#define VIEWER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebHistory>
#include <QtWebKit/QWebPage>

class Page;
class NetworkCookie;

class Viewer : public QWebView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    bool cookieAlreadyAdd;
    Page* m_page;
    NetworkCookie* cookieManager;
    void openCookieRequestDialog ( const QUrl & );
    void findChildNode ( const QPoint & );

  private Q_SLOTS:
    void cursorwait ();
    void cursorFinished ( bool );
    void bookmark();
    void cookiesRequest ( const QUrl & );
    void checkingStyleSheet();
    void linkInfos ( const QString &, const QString &, const QString & );

  protected:
    Viewer* createWindow ( QWebPage::WebWindowType );
    void contextMenuEvent ( QContextMenuEvent * );
    void mousePressEvent ( QMouseEvent * );

  Q_SIGNALS:
    void totalBytes ( qint64 );
    void addBookmark ( const QUrl &, const QString & );
    void hitTestResult ( const QWebElement & );

  public Q_SLOTS:
    void findKeyword ( const QString & );
    void openUrl ( const QUrl & );

  public:
    Viewer ( QWidget * parent = 0 );
    const QString bodyContent();
    const QString source();
    ~Viewer();
};

#endif
