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

#include "colorpickerbutton.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ColorPickerButton::ColorPickerButton ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "colorpickerbutton" ) );
  setToolTip ( i18n ( "Color picker" ) );
  setStatusTip ( i18n ( "Color picker" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  QToolButton* button = new QToolButton ( this );
  button->setCheckable ( false );
  button->setAutoRaise ( true );
  button->setIcon ( KIcon ( "color-picker" ) );
  layout->addWidget ( button );

  setLayout ( layout );

  connect ( button, SIGNAL ( clicked() ), this, SLOT ( clicked() ) );
}

void ColorPickerButton::clicked()
{
  emit clicked ( true );
}

ColorPickerButton::~ColorPickerButton()
{}
