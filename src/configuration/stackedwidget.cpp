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

#include "stackedwidget.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QSizePolicy>

StackedWidget::StackedWidget ( QWidget * parent, Settings * settings )
    : QStackedWidget ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "configure_stacked_widget" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  m_configIDE = new  ConfigIDE ( this );
  insertWidget ( 0, m_configIDE );

  m_configTidy = new ConfigTidy ( this );
  insertWidget ( 1, m_configTidy );

  m_configDomTree = new ConfigDomTree ( this );
  insertWidget ( 2, m_configDomTree );

  m_configBrowser = new ConfigBrowser ( this );
  insertWidget ( 3, m_configBrowser );

  m_configCookies = new ConfigCookies ( this );
  insertWidget ( 4, m_configCookies );

  m_configProxy = new ConfigProxy ( this );
  insertWidget ( 5, m_configProxy );

  m_configSSL = new ConfigSSL ( this );
  insertWidget ( 6, m_configSSL );

  m_configUserAgents = new ConfigUserAgents ( this );
  insertWidget ( 7, m_configUserAgents );

  m_configExtras = new ConfigExtras ( this );
  insertWidget ( 8, m_configExtras );

  setCurrentIndex ( 0 );
  update();

  loadSettings();

  connect ( m_configIDE, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configTidy, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configDomTree, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configBrowser, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configCookies, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configProxy, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configSSL, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configUserAgents, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configExtras, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );
}

/**
* Alle Einstellungen laden
**/
void StackedWidget::loadSettings()
{
  blockSignals ( true );
  m_configIDE->load ( cfg );
  m_configTidy->load ( cfg );
  m_configDomTree->load ( cfg );
  m_configBrowser->load ( cfg );
  m_configCookies->load ( cfg );
  m_configProxy->load ( cfg );
  m_configSSL->load ( cfg );
  m_configUserAgents->load ( cfg );
  m_configExtras->load ( cfg );
  blockSignals ( false );
  emit settingsChanged ( false );
}

/**
* Alle Einstellungen Speichern
**/
void StackedWidget::saveSettings()
{
  blockSignals ( true );
  m_configIDE->save ( cfg );
  m_configTidy->save ( cfg );
  m_configDomTree->save ( cfg );
  m_configBrowser->save ( cfg );
  m_configCookies->save ( cfg );
  m_configProxy->save ( cfg );
  m_configSSL->save ( cfg );
  m_configUserAgents->save ( cfg );
  m_configExtras->save ( cfg );
  blockSignals ( false );

  /**
  * Qt >= 4.6.* hat einen bug beim Speichern und
  * wieder herstellen der Fenster Dimensionen.
  * Deshalb werden hier die Variable gelöscht
  * damit sie bei @ref Window::closeEvent neu
  * geschrieben werden.
  */
  cfg->remove ( "Window/MainWindowGeometry" );

  emit settingsChanged ( false );
}

StackedWidget::~StackedWidget()
{}
