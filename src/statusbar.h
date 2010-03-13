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

#ifndef STATUSBAR_H
#define STATUSBAR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

class StatusBar : public QStatusBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QLabel* m_viewNetworkInfo;
    QLabel* m_viewPortInfo;

  public Q_SLOTS:
    void displayBrowserWidth ( const QSize & );

  public:
    StatusBar ( QStatusBar * parent = 0 );
    ~StatusBar();
};

#endif
