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

#include "domtoolbar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QtGui/QLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

DomToolBar::DomToolBar ( QWidget * parent )
    : QToolBar ( i18n ( "Actions" ), parent )
{
  setObjectName ( QLatin1String ( "domtoolbar" ) );
  setContentsMargins ( 10, 0, 10, 0 );
  setMovable ( false );
  setFloatable ( false );
  setOrientation ( Qt::Horizontal );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Fixed );

  if ( layout() )
    layout()->setSpacing ( 5 );

  QAction* acPrune = addAction ( i18n ( "Collapse" ) );
  acPrune->setIcon ( KIcon ( "view-list-tree" ) );
  acPrune->setToolTip ( i18n ( "Collapse Tree" ) );

  QAction* acExpand = addAction ( i18n ( "Expand" ) );
  acExpand->setIcon ( KIcon ( "view-process-all-tree" ) );
  acExpand->setToolTip ( i18n ( "Expanding Tree" ) );

  QAction* acDeselect = addAction ( i18n ( "Deselect" ) );
  acDeselect->setIcon ( KIcon ( "view-web-browser-dom-tree" ) );
  acDeselect->setToolTip ( i18n ( "Deselect Highlights" ) );

  acHighlight = addAction ( i18n ( "Highlight" ) );
  acHighlight->setIcon ( KIcon ( "view-statistics" ) );
  acHighlight->setToolTip ( i18n ( "onHover Highlight" ) );
  acHighlight->setCheckable ( true );

  connect ( acPrune, SIGNAL ( triggered() ), this, SIGNAL ( prune() ) );
  connect ( acExpand, SIGNAL ( triggered() ), this, SIGNAL ( expand() ) );
  connect ( acDeselect, SIGNAL ( triggered() ), this, SIGNAL ( unselect() ) );
}

bool DomToolBar::onHoverEnabled()
{
  return acHighlight->isChecked();
}

DomToolBar::~DomToolBar()
{}
