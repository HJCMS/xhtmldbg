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

#include "configuseragents.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>

ConfigUserAgents::ConfigUserAgents ( QWidget * parent )
    : PageWidget ( i18n ( "User-Agent" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_user_agents" ) );
  setNotice ( false );
  setCheckable ( false );

  QHBoxLayout* verticalLayout = new QHBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_user_agents_main_layout" ) );

  m_userAgentEditor = new UserAgentEditor ( centralWidget );
  verticalLayout->addWidget ( m_userAgentEditor );

  centralWidget->setLayout ( verticalLayout );

  polishVerticalSpacer ( QSizePolicy::Minimum );

  connect ( m_userAgentEditor, SIGNAL ( modified ( bool ) ),
            this, SLOT ( itemModified ( bool ) ) );
}

void ConfigUserAgents::itemModified ( bool b )
{
  mod = b;
  emit modified ( b );
}

void ConfigUserAgents::load ( Settings * cfg )
{
  m_userAgentEditor->loadUserAgents ( cfg );
  sighted = true;
}

void ConfigUserAgents::save ( Settings * cfg )
{
  m_userAgentEditor->saveUserAgents ( cfg );
}

bool ConfigUserAgents::isModified ()
{
  return mod;
}

bool ConfigUserAgents::isSighted ()
{
  return sighted;
}

ConfigUserAgents::~ConfigUserAgents()
{}
