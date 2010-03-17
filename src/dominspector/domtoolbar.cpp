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

#include "domtoolbar.h"

/* QtCore */
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>

DomToolBar::DomToolBar ( QWidget * parent )
    : QToolBar ( trUtf8 ( "Actions" ), parent )
{
  setObjectName ( QLatin1String ( "domtoolbar" ) );
  setContentsMargins ( 10, 1, 10, 1 );
  setMovable ( false );
  setFloatable ( false );
  setOrientation ( Qt::Horizontal );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Fixed );

  QIcon icon;
  QAction* acPrune = addAction ( trUtf8 ( "Collapse" ) );
  acPrune->setIcon ( icon.fromTheme ( QLatin1String ( "view-list-tree" ) ) );
  acPrune->setToolTip ( trUtf8( "Collapse Tree" ) );

  QAction* acExpand = addAction ( trUtf8 ( "Expand" ) );
  acExpand->setIcon (icon.fromTheme ( QLatin1String ( "view-process-all-tree" ) ) );
  acExpand->setToolTip ( trUtf8( "Expanding Tree" ) );

  QAction* acDeselect = addAction ( trUtf8 ( "Deselect" ) );
  acDeselect->setIcon (icon.fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  acDeselect->setToolTip ( trUtf8( "Deselect Highlights" ) );

  connect ( acPrune, SIGNAL ( triggered() ), this, SIGNAL ( prune() ) );
  connect ( acExpand, SIGNAL ( triggered() ), this, SIGNAL ( expand() ) );
  connect ( acDeselect, SIGNAL ( triggered() ), this, SIGNAL ( unselect() ) );
}

DomToolBar::~DomToolBar()
{}
