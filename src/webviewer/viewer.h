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

#ifndef VIEWER_H
#define VIEWER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QContextMenuEvent>

/* QtWebKit */
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebHistory>
#include <QtWebKit/QWebPage>

class Page;

class Viewer : public QWebView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    Page* m_page;

  private Q_SLOTS:
    void cursorwait ();
    void cursorFinished ();
    void bookmark();

  protected:
    Viewer* createWindow ( QWebPage::WebWindowType );
    void contextMenuEvent( QContextMenuEvent * );

  Q_SIGNALS:
    void addBookmark ( const QUrl &, const QString & );

  public:
    Viewer ( QWidget * parent = 0 );
    ~Viewer();
};

#endif
