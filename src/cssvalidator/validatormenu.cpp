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

#include "validatormenu.h"

/* QtCore */
#include <QtCore/QString>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ValidatorMenu::ValidatorMenu ( QWidget * parent )
    : QMenu ( parent )
{
  setObjectName ( QLatin1String ( "validatormenu" ) );

  // Aktuelle Url Überprüfen
  aCheck = addAction ( KIcon ( QLatin1String ( "system-run" ) ), i18n ( "Check" ) );
  aCheck->setToolTip ( i18n ( "Checking last changed url" ) );
  aCheck->setEnabled ( false );
  connect ( aCheck, SIGNAL ( triggered() ), this, SIGNAL ( check() ) );

  // Sofort Abbrechen (killen)
  aDropout = addAction ( KIcon ( QLatin1String ( "run-build-prune" ) ), i18n ( "Dropout" ) );
  aDropout->setToolTip ( i18n ( "Dropout current request" ) );
  aDropout->setEnabled ( false );
  connect ( aDropout, SIGNAL ( triggered() ), this, SIGNAL ( dropout() ) );

  // Aufsteigend sortieren
  aAscending = addAction ( KIcon ( QLatin1String ( "view-sort-ascending" ) ), i18n ( "Ascending" ) );
  connect ( aAscending, SIGNAL ( triggered() ), this, SIGNAL ( ascending() ) );

  // Absteigend sortieren
  aDecending = addAction ( KIcon ( QLatin1String ( "view-sort-descending" ) ), i18n ( "Descending" ) );
  connect ( aDecending, SIGNAL ( triggered() ), this, SIGNAL ( descending() ) );

  // Konfiguration öffnen
  aConfig = addAction ( KIcon ( QLatin1String ( "configure" ) ), i18n ( "Configure" ) );
  connect ( aConfig, SIGNAL ( triggered() ), this, SIGNAL ( configure() ) );

  // Liste leeren
  aClearItem = addAction ( KIcon ( QLatin1String ( "edit-clear" ) ), i18n ( "Clear" ) );
  connect ( aClearItem, SIGNAL ( triggered() ), this, SIGNAL ( clearlist() ) );
}

/**
* Aktiviere das Menü nur wenn die URL Valide ist.
*/
void ValidatorMenu::enableCheckUrlAction ( bool b )
{
  aCheck->setEnabled ( b );
}

/**
* Aktiviere das Menü nur wenn ein Prozess am laufen ist.
*/
void ValidatorMenu::enableKillProcessAction ( bool b )
{
  aDropout->setEnabled ( b );
}

ValidatorMenu::~ValidatorMenu()
{}
