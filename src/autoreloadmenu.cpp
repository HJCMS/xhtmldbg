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
  disableReload->setCheckable ( true );
  disableReload->setChecked ( true );
  connect ( disableReload, SIGNAL ( triggered() ), this, SLOT ( setTimerDisable() ) );

  ac10_seconds = addAction ( timeIcon, trUtf8 ( "10 Seconds" ) );
  ac10_seconds->setCheckable ( true );
  connect ( ac10_seconds, SIGNAL ( triggered() ), this, SLOT ( set10_seconds() ) );

  ac20_seconds = addAction ( timeIcon, trUtf8 ( "20 Seconds" ) );
  ac20_seconds->setCheckable ( true );
  connect ( ac20_seconds, SIGNAL ( triggered() ), this, SLOT ( set20_seconds() ) );

  ac30_seconds = addAction ( timeIcon, trUtf8 ( "30 Seconds" ) );
  ac30_seconds->setCheckable ( true );
  connect ( ac30_seconds, SIGNAL ( triggered() ), this, SLOT ( set30_seconds() ) );

  ac40_seconds = addAction ( timeIcon, trUtf8 ( "40 Seconds" ) );
  ac40_seconds->setCheckable ( true );
  connect ( ac40_seconds, SIGNAL ( triggered() ), this, SLOT ( set40_seconds() ) );

  ac50_seconds = addAction ( timeIcon, trUtf8 ( "50 Seconds" ) );
  ac50_seconds->setCheckable ( true );
  connect ( ac50_seconds, SIGNAL ( triggered() ), this, SLOT ( set50_seconds() ) );

  ac60_seconds = addAction ( timeIcon, trUtf8 ( "1 Minute" ) );
  ac60_seconds->setCheckable ( true );
  connect ( ac60_seconds, SIGNAL ( triggered() ), this, SLOT ( set60_seconds() ) );

  setActiveAction ( disableReload );
}

void AutoReloadMenu::swapChecked ( QAction *action )
{
  foreach ( QAction* item, findChildren<QAction*>() )
  {
    if ( item != action )
      item->setChecked ( false );
  }
}

void AutoReloadMenu::setTimerDisable()
{
  swapChecked ( disableReload );
  emit reloadInterval ( 0 );
}

void AutoReloadMenu::set10_seconds()
{
  swapChecked ( ac10_seconds );
  emit reloadInterval ( 10 );
}

void AutoReloadMenu::set20_seconds()
{
  swapChecked ( ac20_seconds );
  emit reloadInterval ( 20 );
}

void AutoReloadMenu::set30_seconds()
{
  swapChecked ( ac30_seconds );
  emit reloadInterval ( 30 );
}

void AutoReloadMenu::set40_seconds()
{
  swapChecked ( ac40_seconds );
  emit reloadInterval ( 40 );
}

void AutoReloadMenu::set50_seconds()
{
  swapChecked ( ac50_seconds );
  emit reloadInterval ( 50 );
}

void AutoReloadMenu::set60_seconds()
{
  swapChecked ( ac60_seconds );
  emit reloadInterval ( 60 );
}

AutoReloadMenu::~AutoReloadMenu()
{}
