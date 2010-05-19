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

#include "zoombar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QLayout>

ZoomBar::ZoomBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "zoommenu" ) );
  setWindowTitle ( trUtf8 ( "Zoom" ) );
  setStatusTip ( trUtf8 ( "Zoom Webbrowser Content" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );
  layout()->setSpacing ( 5 );

  QIcon icon;

  ac_in = addAction ( trUtf8 ( "ZoomIn" ) );
  ac_in->setObjectName ( QLatin1String ( "action_zoom_in" ) );
  ac_in->setIcon ( icon.fromTheme ( QLatin1String ( "zoom-in" ) ) );

  ac_out = addAction ( trUtf8 ( "ZoomOut" ) );
  ac_out->setObjectName ( QLatin1String ( "action_zoom_out" ) );
  ac_out->setIcon ( icon.fromTheme ( QLatin1String ( "zoom-out" ) ) );

  ac_original = addAction ( trUtf8 ( "Original" ) );
  ac_original->setObjectName ( QLatin1String ( "action_zoom_original" ) );
  ac_original->setIcon ( icon.fromTheme ( QLatin1String ( "zoom-original" ) ) );

  connect ( ac_in, SIGNAL ( triggered () ),
            this , SLOT ( zoomInClicked () ) );

  connect ( ac_out, SIGNAL ( triggered () ),
            this , SLOT ( zoomOutClicked () ) );

  connect ( ac_original, SIGNAL ( triggered () ),
            this , SLOT ( zoomOriginalClicked () ) );

}

/**
* Setze Zoom Faktor Plus=1
*/
void ZoomBar::zoomInClicked()
{
  emit zoomFactor ( 1 );
}

/**
* Setze Zoom Faktor Minus=2
*/
void ZoomBar::zoomOutClicked()
{
  emit zoomFactor ( 2 );
}

/**
* Setze Zoom Faktor Original=0
*/
void ZoomBar::zoomOriginalClicked()
{
  emit zoomFactor ( 0 );
}

ZoomBar::~ZoomBar()
{}
