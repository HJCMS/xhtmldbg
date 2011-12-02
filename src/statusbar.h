/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2012
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

#ifndef STATUSBAR_H
#define STATUSBAR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KStatusBar>

class StatusBar : public KStatusBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QLabel* m_refreshLabel;
    QLabel* m_noticeLabel;
    QLabel* m_viewPageSize;
    QLabel* m_viewPortInfo;

  public Q_SLOTS:
    Q_SCRIPTABLE void timerStatus ( int, int );
    Q_SCRIPTABLE void notice ( bool notice = false );
    Q_SCRIPTABLE void displayBrowserWidth ( const QSize & );
    Q_SCRIPTABLE void setLoadedPageSize ( qint64 );

  public:
    StatusBar ( KStatusBar * parent = 0 );
    ~StatusBar();
};

#endif
