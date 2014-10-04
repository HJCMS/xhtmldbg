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
  insertWidget ( ITEM_CONFIG_IDE, m_configIDE );

  m_configDomTree = new ConfigDomTree ( this );
  insertWidget ( ITEM_CONFIG_DOMTREE, m_configDomTree );

  m_configBrowser = new ConfigBrowser ( this );
  insertWidget ( ITEM_CONFIG_BROWSER, m_configBrowser );

  m_configNetwork = new ConfigNetwork ( this );
  insertWidget ( ITEM_CONFIG_NETWORK, m_configNetwork );

  m_configCookies = new ConfigCookies ( this );
  insertWidget ( ITEM_CONFIG_COOKIES, m_configCookies );

  m_configProxy = new ConfigProxy ( this );
  insertWidget ( ITEM_CONFIG_PROXY, m_configProxy );

  m_configSSL = new ConfigSSL ( this );
  insertWidget ( ITEM_CONFIG_SSL, m_configSSL );

  m_configUserAgents = new ConfigUserAgents ( this );
  insertWidget ( ITEM_CONFIG_USERAGENTS, m_configUserAgents );

  m_configWebSecurity = new ConfigWebSecurity ( this );
  insertWidget ( ITEM_CONFIG_WEBSECURE, m_configWebSecurity );

  m_configExtras = new ConfigExtras ( this );
  insertWidget ( ITEM_CONFIG_EXTRAS, m_configExtras );

  setCurrentIndex ( ITEM_CONFIG_IDE );
  loadWidgetSettings ( ITEM_CONFIG_IDE );

  connect ( m_configIDE, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configDomTree, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configBrowser, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configNetwork, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configCookies, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configProxy, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configSSL, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configUserAgents, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configWebSecurity, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( m_configExtras, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( settingsChanged ( bool ) ) );

  connect ( this, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( loadWidgetSettings ( int ) ) );

  // First start
  emit settingsChanged ( false );
}

/** Damit der Start schneller von statten geht
* erst beim öffnen eines Stacks die Konfiguration laden! */
void StackedWidget::loadWidgetSettings ( int index )
{
  blockSignals ( true );
  switch ( index )
  {
    case ITEM_CONFIG_IDE:
    {
      if ( ! m_configIDE->isSighted () )
        m_configIDE->load ( cfg );
    }
    break;

    case ITEM_CONFIG_DOMTREE:
    {
      if ( ! m_configDomTree->isSighted () )
        m_configDomTree->load ( cfg );
    }
    break;

    case ITEM_CONFIG_BROWSER:
    {
      if ( ! m_configBrowser->isSighted () )
        m_configBrowser->load ( cfg );
    }
    break;

    case ITEM_CONFIG_NETWORK:
    {
      if ( ! m_configNetwork->isSighted () )
        m_configNetwork->load ( cfg );
    }
    break;

    case ITEM_CONFIG_COOKIES:
    {
      if ( ! m_configCookies->isSighted () )
        m_configCookies->load ( cfg );
    }
    break;

    case ITEM_CONFIG_PROXY:
    {
      if ( ! m_configProxy->isSighted () )
        m_configProxy->load ( cfg );
    }
    break;

    case ITEM_CONFIG_SSL:
    {
      if ( ! m_configSSL->isSighted () )
        m_configSSL->load ( cfg );
    }
    break;

    case ITEM_CONFIG_USERAGENTS:
    {
      if ( ! m_configUserAgents->isSighted () )
        m_configUserAgents->load ( cfg );
    }
    break;

    case ITEM_CONFIG_WEBSECURE:
    {
      if ( ! m_configWebSecurity->isSighted () )
        m_configWebSecurity->load ( cfg );
    }
    break;

    case ITEM_CONFIG_EXTRAS:
    {
      if ( ! m_configExtras->isSighted () )
        m_configExtras->load ( cfg );
    }
    break;

    default:
    {
      if ( ! m_configIDE->isSighted () )
        m_configIDE->load ( cfg );
    }
    break;
  }
  blockSignals ( false );
}

/** Alle Einstellungen laden */
void StackedWidget::loadSettings()
{
  blockSignals ( true );
  m_configIDE->load ( cfg );
  m_configDomTree->load ( cfg );
  m_configBrowser->load ( cfg );
  m_configNetwork->load ( cfg );
  m_configCookies->load ( cfg );
  m_configProxy->load ( cfg );
  m_configSSL->load ( cfg );
  m_configUserAgents->load ( cfg );
  m_configWebSecurity->load ( cfg );
  m_configExtras->load ( cfg );
  blockSignals ( false );
  // zurück setzen
  emit settingsChanged ( false );
}

/** Alle Einstellungen Speichern */
void StackedWidget::saveSettings()
{
  blockSignals ( true );

  if ( m_configIDE->isModified () )
    m_configIDE->save ( cfg );

  if ( m_configDomTree->isModified () )
    m_configDomTree->save ( cfg );

  if ( m_configBrowser->isModified () )
    m_configBrowser->save ( cfg );

  if ( m_configNetwork->isModified () )
    m_configNetwork->save ( cfg );

  if ( m_configCookies->isModified () )
    m_configCookies->save ( cfg );

  if ( m_configProxy->isModified () )
    m_configProxy->save ( cfg );

  if ( m_configSSL->isModified () )
    m_configSSL->save ( cfg );

  if ( m_configUserAgents->isModified () )
    m_configUserAgents->save ( cfg );

  if ( m_configWebSecurity->isModified () )
    m_configWebSecurity->save ( cfg );

  if ( m_configExtras->isModified () )
    m_configExtras->save ( cfg );

  blockSignals ( false );

  /* Qt >= 4.6.* hat einen bug beim Speichern und
  * wieder herstellen der Fenster Dimensionen.
  * Deshalb werden hier die Variable gelöscht
  * damit sie bei @ref Window::closeEvent neu
  * geschrieben werden. */
  cfg->remove ( "Window/MainWindowGeometry" );

  // zurück setzen
  emit settingsChanged ( false );
}

StackedWidget::~StackedWidget()
{}
