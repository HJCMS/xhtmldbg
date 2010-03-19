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

#ifndef WEBVIEWER_H
#define WEBVIEWER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>

/* QtWebKit */
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebHistory>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebElement>

class Viewer;

class WebViewer : public QTabWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( QUrl url READ getUrl WRITE setUrl )

  private:
    Viewer* m_viewer;
    Viewer* activeView();
    QUrl url;
    void updateWebSettings();

  private Q_SLOTS:
    void updateTabTitle ( const QString & );
    void pretended ( int );
    void setFavicon();

  Q_SIGNALS:
    void bytesLoaded ( qint64 );
    void urlChanged ( const QUrl & );
    void addBookmark ( const QUrl &, const QString & );
    void loadFinished ( bool );
    void statusBarMessage ( const QString & );
    void hitTestResult ( const QWebElement & );

  public Q_SLOTS:
    void addViewerTab ( Viewer * );
    void addViewerTab ();
    void closeViewerTab ( int );
    void keywords ( const QStringList & );
    void refresh ();
    void back ();
    void forward ();
    void setUrl ( const QUrl & );
    void setWebFocus();

  public:
    WebViewer ( QWidget * parent = 0 );
    const QUrl getUrl();
    const QString toHtml();
    const QWebElement toWebElement();
    static const QString blank();
    ~WebViewer();
};

#endif
