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

#ifndef TABBEDWIDGET_H
#define TABBEDWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>

class SourceWidget;
class WebViewer;

class TabbedWidget : public QTabWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    SourceWidget* m_sourceWidget;
    WebViewer* m_webViewer;

  Q_SIGNALS:
    void loadUrl ( const QUrl & );

  public Q_SLOTS:
    void addwWebViewerTab ( WebViewer* view = 0 );
    void setUrl ( const QUrl & );
    void check();
    void format();

  public:
    TabbedWidget ( QWidget * parent = 0 );
    ~TabbedWidget();
};

#endif
