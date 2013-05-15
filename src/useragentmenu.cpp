/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "useragentmenu.h"

/* QtCore */
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>

UserAgentMenu::UserAgentMenu ( QMenu * parent, Settings * settings )
    : QMenu ( parent )
    , cfg ( settings )
    , agentIcon ( QIcon::fromTheme ( QLatin1String ( "preferences-desktop-user" ) ) )
{
  setObjectName ( QLatin1String ( "useragentmenu" ) );
  setTitle ( i18n ( "User Agent" ) );
  setIcon ( agentIcon );

  // Signal Mapper
  m_signalMapper = new QSignalMapper ( this );

  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SLOT ( clicked ( const QString & ) ) );

  if ( cfg )
    addAgentActions();
}

/**
* Lese aus der Konfiguration die Benutzer Kennungen aus und
* schreibe diese in das Menü.
*/
void UserAgentMenu::addAgentActions()
{
  int size = cfg->beginReadArray ( QLatin1String ( "UserAgents" ) );
  if ( size < 1 )
  {
    /* Wichtig - Immer schliessen sonst baut Settings ein Array
    * ohne size= Schlüssel und stürtzt bei nächsten leseversuch ab! */
    cfg->endArray();
    return;
  }

  for ( int i = 0; i < size; ++i )
  {
    cfg->setArrayIndex ( i );
    QString title = cfg->value ( "name" ).toString();
    QString tokens = cfg->value ( "agent" ).toString();
    tokens.append ( " " );
    tokens.append ( cfg->value ( "tokens" ).toString() );
    if ( title.isEmpty() )
      continue;

    QAction* ac = addAction ( agentIcon, title );
    ac->setToolTip ( tokens );
    connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
    m_signalMapper->setMapping ( ac, tokens );
  }
  cfg->endArray();
}

/**
* Wenn der Benutzer einen User-Agent ausgewählt hat.
* Diese Kennung in die Konfiguration schreiben.
* Die Methode @class NetworkSettings::userAgentString liest
* vor jeder Netzwerkanfrage den User-Agent aus und setzt
* bei @ref NetworkAccessManager::createRequest diese Option.
*/
void UserAgentMenu::clicked ( const QString &agent )
{
  if ( agent.isEmpty() )
    return;

  cfg->setValue ( QLatin1String ( "UserAgentString" ), agent );
}

UserAgentMenu::~UserAgentMenu()
{}
