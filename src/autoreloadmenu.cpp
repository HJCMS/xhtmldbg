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

#include "autoreloadmenu.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QIcon>

AutoReloadMenu::AutoReloadMenu ( QMenu * parent )
    : QMenu ( parent )
{
  setObjectName ( QLatin1String ( "autoreloadmenu" ) );
  setTitle ( trUtf8 ( "Auto Refresh" ) );

  QIcon timeIcon ( QIcon::fromTheme ( QLatin1String ( "clock" ) ) );
  setIcon ( timeIcon );

  disableReload = addAction ( timeIcon, trUtf8 ( "Disabled" ) );
  connect ( disableReload, SIGNAL ( triggered() ), this, SLOT ( setTimerDisable() ) );

  ac15_seconds = addAction ( timeIcon, trUtf8 ( "15 Seconds" ) );
  connect ( ac15_seconds, SIGNAL ( triggered() ), this, SLOT ( set15_seconds() ) );

  ac30_seconds = addAction ( timeIcon, trUtf8 ( "30 Seconds" ) );
  connect ( ac30_seconds, SIGNAL ( triggered() ), this, SLOT ( set30_seconds() ) );

  ac45_seconds = addAction ( timeIcon, trUtf8 ( "45 Seconds" ) );
  connect ( ac45_seconds, SIGNAL ( triggered() ), this, SLOT ( set45_seconds() ) );

  ac60_seconds = addAction ( timeIcon, trUtf8 ( "1 Minute" ) );
  connect ( ac60_seconds, SIGNAL ( triggered() ), this, SLOT ( set60_seconds() ) );
}

void AutoReloadMenu::setTimerDisable()
{
  emit reloadInterval ( 0 );
}

void AutoReloadMenu::set15_seconds()
{
  emit reloadInterval ( 15 );
}

void AutoReloadMenu::set30_seconds()
{
  emit reloadInterval ( 30 );
}

void AutoReloadMenu::set45_seconds()
{
  emit reloadInterval ( 45 );
}

void AutoReloadMenu::set60_seconds()
{
  emit reloadInterval ( 60 );
}

AutoReloadMenu::~AutoReloadMenu()
{}
