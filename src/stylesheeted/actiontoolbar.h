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

#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KToolBar>

class ColorGroup : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QRadioButton* m_background;
    QRadioButton* m_foreground;

  public:
    explicit ColorGroup ( KToolBar * parent = 0 );
    bool isBackground();
};

class ActionToolBar : public KToolBar
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_ENUMS ( Type )

  private:
    ColorGroup* m_colorGroup;

  public:
    enum Type { BACKGROUND = 0, FOREGROUND = 1, BORDER = 2 };
    explicit ActionToolBar ( QWidget * parent = 0 );
    Type whichColor();
    virtual ~ActionToolBar();
};

#endif
