/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "useragenteditor.h"
#include "useragentstable.h"
#include "droptoclipboard.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

UserAgentEditor::UserAgentEditor ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "UserAgentEditor" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QHBoxLayout* hLayout = new QHBoxLayout ( this );
  hLayout->setContentsMargins ( 0, 0, 0, 0 );

  table = new UserAgentsTable ( this );
  hLayout->addWidget ( table );

  QVBoxLayout* vLayout = new QVBoxLayout;
  vLayout->setContentsMargins ( 0, 0, 2, 2 );
  vLayout->setSpacing ( 5 );
  vLayout->addStretch ( 1 );

  QToolButton* up = new QToolButton ( this );
  up->setObjectName ( QLatin1String ( "moveup" ) );
  up->setToolTip ( i18n ( "Move Selected row Up" ) );
  up->setWhatsThis ( i18n ( "Move Selected row Up" ) );
  up->setIcon ( QIcon::fromTheme ( QLatin1String ( "arrow-up" ) ) );
  vLayout->addWidget ( up );

  QToolButton* down = new QToolButton ( this );
  down->setObjectName ( QLatin1String ( "movedown" ) );
  down->setToolTip ( i18n ( "Move Selected row Down" ) );
  down->setWhatsThis ( i18n ( "Move Selected row Down" ) );
  down->setIcon ( QIcon::fromTheme ( QLatin1String ( "arrow-down" ) ) );
  vLayout->addWidget ( down );

  vLayout->addStretch ( 1 );
  vLayout->addWidget ( new DropToClipBoard ( this, down->sizeHint() ) );

  hLayout->addLayout ( vLayout );
  setLayout ( hLayout );

  connect ( table, SIGNAL ( agentChanged ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );

  connect ( up, SIGNAL ( clicked() ), this, SLOT ( moveUp() ) );
  connect ( down, SIGNAL ( clicked() ), this, SLOT ( moveDown() ) );
}

void UserAgentEditor::moveUp()
{
  table->moveRow ( -1 );
}

void UserAgentEditor::moveDown()
{
  table->moveRow ( 1 );
}

void UserAgentEditor::loadUserAgents ( Settings * cfg )
{
  table->loadUserAgents ( cfg );
}

void UserAgentEditor::saveUserAgents ( Settings * cfg )
{
  table->saveUserAgents ( cfg );
}

UserAgentEditor::~UserAgentEditor()
{}
