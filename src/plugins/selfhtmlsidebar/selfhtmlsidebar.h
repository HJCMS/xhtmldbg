/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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

#ifndef SELFHTMLSIDEBAR_H
#define SELFHTMLSIDEBAR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QResizeEvent>
#include <QtGui/QWidget>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* QtWebKit */
#include <QtWebKit/QWebView>

class SelfHtmlSidebar : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QString service;
    QDBusConnection p_dbus;
    QSettings* cfg;
    QWebView* m_webView;
    const QUrl sideBarUrl() const;
    QUrl lastChanged;
    QLineEdit* searchLine;
    QLabel* m_titleLabel;
    QLabel* m_urlLabel;
    void openConfigDialog ();

  private Q_SLOTS:
    void openIndex ();
    void openConfig ();
    void openLinkClicked ( const QUrl &url );
    void findKeyword ( const QString &word = QString() );

  protected:
    void resizeEvent ( QResizeEvent * );

  public:
    SelfHtmlSidebar ( QWidget * parent = 0 );
    virtual ~SelfHtmlSidebar();
};

#endif
