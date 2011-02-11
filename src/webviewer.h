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

#ifndef WEBVIEWER_H
#define WEBVIEWER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>

/* QtWebKit */
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebHistory>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebElement>

class Viewer;
class WebSettings;

class WebViewer : public QTabWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_PROPERTY ( QUrl url READ getUrl WRITE setUrl )

  private:
    WebSettings* m_wcfg;
    const QIcon tabIcon;
    Viewer* m_viewer;
    Viewer* activeView ( int index = -1 );
    int getIndexWithUrl ( const QUrl & );
    void setTabCornerButton();
    int realPageIndex ( int index = -1 );

  private Q_SLOTS:
    void updateTab ( const QUrl &, const QString & );
    void pretended ( int );
    void pageChanged ( int index = -1 );
    void setFavicon ( int index = -1 );

  protected:
    virtual void tabInserted ( int index );

  Q_SIGNALS:
    void loadStarted ();
    void bytesLoaded ( qint64 );
    void urlChanged ( const QUrl & );
    void addBookmark ( const QUrl &, const QString & );
    void statusBarMessage ( const QString & );
    void elementsTree ( const QUrl &, const QWebElement & );
    void hitTestResult ( const QWebElement & );
    /** send currently used Page */
    void pageEntered ( QWebPage * );

  public Q_SLOTS:
    void addViewerTab ( Viewer *, bool move = true );
    void addViewerTab ();
    Q_SCRIPTABLE void closeViewerTab ( int );
    Q_SCRIPTABLE void keywords ( const QStringList & );
    Q_SCRIPTABLE void refresh ();
    Q_SCRIPTABLE void back ();
    Q_SCRIPTABLE void forward ();
    void setUrl ( const QUrl & );
    void setWebFocus();
    void zoomFactor ( int );
    void setViewerWidth ( int );

  public:
    WebViewer ( QWidget * parent = 0 );
    QWebPage* startPage();
    bool setViewerTabByUrl ( const QUrl &oldUrl, const QUrl &newUrl );
    bool addViewerUrlTab ( const QUrl &url );
    const QUrl getUrl ();
    const QList<QUrl> getPageUrls();
    const QString blank();
    Q_SCRIPTABLE void setAboutPage ( const QString & );
    virtual ~WebViewer();
};

#endif
