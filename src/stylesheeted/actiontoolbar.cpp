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

#include "actiontoolbar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QHBoxLayout>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

ColorGroup::ColorGroup ( KToolBar * parent )
    : QGroupBox ( i18n ( "Color" ), parent )
{
  setObjectName ( QLatin1String ( "ColorGroup" ) );
  QHBoxLayout* layout = new QHBoxLayout ( this );

  m_background = new QRadioButton ( i18n ( "Background" ), this );
  m_background->setChecked ( true );
  layout->addWidget ( m_background );

  m_foreground = new QRadioButton ( i18n ( "Foreground" ), this );
  layout->addWidget ( m_foreground );

  layout->addStretch ( 1 );
  setLayout ( layout );
}

bool ColorGroup::isBackground()
{
  return m_background->isChecked();
}

ActionToolBar::ActionToolBar ( QWidget * parent )
    : KToolBar ( parent )
{
  setObjectName ( QLatin1String ( "ActionToolBar" ) );

  m_colorGroup = new  ColorGroup ( this );
  addWidget ( m_colorGroup );
}

ActionToolBar::Type ActionToolBar::whichColor()
{
  if ( m_colorGroup->isBackground() )
    return BACKGROUND;
  else
    return FOREGROUND;
}

ActionToolBar::~ActionToolBar()
{}
