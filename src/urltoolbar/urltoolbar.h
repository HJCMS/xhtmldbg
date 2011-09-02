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

#ifndef URLTOOLBAR_H
#define URLTOOLBAR_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KUrl>

class UrlLineEdit;

class UrlToolBar : public QToolBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    UrlLineEdit* m_urlLineEdit;

  private Q_SLOTS:
    void moveUpUrl ();
    void urlReloadPage();

  Q_SIGNALS:
    void urlChanged ( const QUrl & );
    void reloadUrl ( const QUrl & );
    void sendMessage ( const QString & );

  public Q_SLOTS:
    Q_SCRIPTABLE void setUrl ( const QUrl & );

  public:
    UrlToolBar ( QWidget * parent = 0 );
    ~UrlToolBar();
};

#endif
