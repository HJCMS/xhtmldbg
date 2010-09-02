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

#include "watcher.h"
#include "grabberwindow.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>

Watcher::Watcher ( QWidget * parent )
    : QWidget ( parent )
    , startRecording ( false )
{
  setObjectName ( QLatin1String ( "watcher" ) );

  QHBoxLayout* hLayout = new QHBoxLayout ( this );
  hLayout->setObjectName ( "watcher.hlayout" );

  m_recordButton = new QToolButton ( this );
  m_recordButton->setObjectName ( "watcher.hlayout.vlayout.recordbutton" );
  m_recordButton->setCheckable ( true );
  m_recordButton->setChecked ( false );
  m_recordButton->setIcon ( QIcon::fromTheme( "color-picker" ) );
  hLayout->addWidget ( m_recordButton, 0, Qt::AlignTop );

  m_grabberWindow = new GrabberWindow ( this );
  m_grabberWindow->setObjectName ( "watcher.hlayout.grabberwindow" );
  hLayout->addWidget ( m_grabberWindow, 0, ( Qt::AlignLeft | Qt::AlignTop ) );

  setLayout ( hLayout );

  connect ( m_recordButton, SIGNAL ( toggled ( bool ) ),
            m_grabberWindow, SLOT ( startRecording ( bool ) ) );

  connect ( m_grabberWindow, SIGNAL ( recording ( bool ) ),
            m_recordButton, SLOT ( setChecked ( bool ) ) );
}

Watcher::~Watcher()
{}
