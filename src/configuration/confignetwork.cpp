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

#include "confignetwork.h"

/* QtCore */
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>
#include <QtGui/QStandardItem>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

ConfigNetwork::ConfigNetwork ( QWidget * parent )
    : PageWidget ( i18n ( "Network Settings" ), parent )
{
  setObjectName ( QLatin1String ( "config_network_settings" ) );
  setNotice ( true );
  setCheckable ( false );

  QSizePolicy sizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
  sizePolicy.setHorizontalStretch ( 0 );
  sizePolicy.setVerticalStretch ( 0 );

  // Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );

  // Cache Einstellungen
  m_groupBox = new QGroupBox ( i18n ( "modify caching behavior" ), centralWidget );
  m_groupBox->setObjectName ( "Networking/ModifyCachingBehavior" );
  m_groupBox->setCheckable ( true );
  m_groupBox->setChecked ( false );

  QVBoxLayout* groupBoxLayout = new QVBoxLayout ( m_groupBox );

  QString warn = i18n ( "Warning: “xhtmldbg” always manage the best way for caching network connections with debugging.\nFor example the source viewer need cached data to set the real source code from requested sites.\nBy default “xhtmldbg” always remove cached data before a request operation has been send." );
  groupBoxLayout->addWidget ( new QLabel ( warn, m_groupBox ) );

  m_cb1 = new FeatureBox ( i18n ( "Controls if the data obtained should be saved to cache for future uses." ), m_groupBox );
  m_cb1->setObjectName ( QLatin1String ( "Networking/CacheSaveControlAttribute" ) );
  m_cb1->setToolTip ( i18n ( "If the box is not checked, the data obtained will not be automatically cached." ) );
  m_cb1->setChecked ( true );
  groupBoxLayout->addWidget ( m_cb1 );

  m_cb2 = new FeatureBox ( i18n ( "Indicates whether incoming data was obtained from cache or not." ), m_groupBox );
  m_cb2->setObjectName ( QLatin1String ( "Networking/SourceIsFromCacheAttribute" ) );
  m_cb2->setToolTip ( i18n ( "If the box is not checked, incoming data will not be automatically checked." ) );
  m_cb2->setChecked ( true );
  groupBoxLayout->addWidget ( m_cb2 );

  m_comboBox = new  QComboBox ( m_groupBox );
  // QNetworkRequest::AlwaysNetwork = 0
  m_comboBox->insertItem ( 0, i18n ( "Always Network (always load from network and do not check if the cache has a valid entry)" ), QNetworkRequest::AlwaysNetwork );
  // QNetworkRequest::PreferNetwork = 1 (Standard)
  m_comboBox->insertItem ( 1, i18n ( "Prefer Network (load from the network if the cached entry is older than the network entry)" ), QNetworkRequest::PreferNetwork );
  // QNetworkRequest::PreferCache
  m_comboBox->insertItem ( 2, i18n ( "Prefer Cache (load from cache if available, otherwise load from network)" ), QNetworkRequest::PreferCache );
  // QNetworkRequest::AlwaysCache
  m_comboBox->insertItem ( 3, i18n ( "Always Cache (only load from cache, indicating error if the item was not cached)" ), QNetworkRequest::AlwaysCache );
  m_comboBox->setCurrentIndex ( 1 );
  groupBoxLayout->addWidget ( m_comboBox );

  m_groupBox->setLayout ( groupBoxLayout );
  verticalLayout->addWidget ( m_groupBox );

  centralWidget->setLayout ( verticalLayout );

  connect ( m_groupBox, SIGNAL ( toggled ( bool ) ),
            this, SLOT ( cachingToggled ( bool ) ) );

  connect ( m_cb1, SIGNAL ( stateChanged ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );

  connect ( m_cb2, SIGNAL ( stateChanged ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );

  connect ( m_comboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );
}

void ConfigNetwork::cachingToggled ( bool )
{
  mod = true;
  emit modified ( true );
}

void ConfigNetwork::itemClicked ( int )
{
  mod = true;
  emit modified ( true );
}

void ConfigNetwork::load ( Settings * cfg )
{
  if ( cfg->value ( "Networking/ModifyCachingBehavior", false ).toBool() )
  {
    m_groupBox->setChecked ( true );
    m_cb1->setChecked ( cfg->value ( "Networking/CacheSaveControlAttribute", true ).toBool() );
    m_cb2->setChecked ( cfg->value ( "Networking/SourceIsFromCacheAttribute", true ).toBool() );
    m_comboBox->setCurrentIndex ( cfg->value ( "Networking/CacheLoadControlAttribute", 1 ).toUInt() );
  }
  else
  {
    m_groupBox->setChecked ( false );
    m_cb1->setChecked ( true );
    m_cb2->setChecked ( true );
    m_comboBox->setCurrentIndex ( 1 );
  }
  sighted = true;
}

void ConfigNetwork::save ( Settings * cfg )
{
  if ( m_groupBox->isChecked() )
  {
    cfg->setValue ( "Networking/ModifyCachingBehavior", true );
    cfg->setValue ( "Networking/CacheSaveControlAttribute", m_cb1->isChecked() );
    cfg->setValue ( "Networking/SourceIsFromCacheAttribute", m_cb2->isChecked() );
    cfg->setValue ( "Networking/CacheLoadControlAttribute", m_comboBox->currentIndex() );
  }
  else
  {
    cfg->setValue ( "Networking/ModifyCachingBehavior", false );
    cfg->remove ( "Networking/CacheSaveControlAttribute" );
    cfg->remove ( "Networking/SourceIsFromCacheAttribute" );
    cfg->remove ( "Networking/CacheLoadControlAttribute" );
  }
}

bool ConfigNetwork::isModified ()
{
  return mod;
}

bool ConfigNetwork::isSighted ()
{
  return sighted;
}

ConfigNetwork::~ConfigNetwork()
{}
