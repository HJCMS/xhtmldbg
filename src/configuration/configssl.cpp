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

#include "configssl.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>

ConfigSSL::ConfigSSL ( QWidget * parent )
    : PageWidget ( trUtf8 ( "SSL Certification" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_ssl" ) );
  setNotice ( false );
  setCheckable ( false );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_ssl_layout" ) );
  verticalLayout->setContentsMargins ( 0, 0, 0, 0 );
  verticalLayout->setSpacing ( 5 );

  // Issuer Tabelle
  m_certIssuers = new CertIssuers ( centralWidget );
  verticalLayout->addWidget ( m_certIssuers );

  // Trusted Hosts Liste
  m_configTrustedHosts = new ConfigTrustedHosts ( centralWidget );
  verticalLayout->addWidget ( m_configTrustedHosts );

  // Authentifizierungs Kontrolle
  m_configAccessControl = new ConfigAccessControl ( centralWidget );
  verticalLayout->addWidget ( m_configAccessControl );

  centralWidget->setLayout ( verticalLayout );

  connect ( m_certIssuers, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );

  connect ( m_configTrustedHosts, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );

  connect ( m_configAccessControl, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );
}

void ConfigSSL::load ( QSettings * cfg )
{
  m_certIssuers->setCaCertDatabase ( cfg->value ( QLatin1String ( "sslCaCertsDatabase" ), QLatin1String ( "/etc/ssl/certs/ca-certificates.crt" ) ).toString() );

  // Trusted Hosts Liste
  QStringList list;
  int size = cfg->beginReadArray ( QLatin1String ( "TrustedCertsHosts" ) );

  if ( size < 0 )
    list << QLatin1String ( "localhost" );

  for ( int i = 0; i < size; ++i )
  {
    cfg->setArrayIndex ( i );
    list.append ( cfg->value ( "host" ).toString() );
  }
  cfg->endArray();
  m_configTrustedHosts->setTrustedList ( list );

  // Authentifizierungs Kontrolle
  m_configAccessControl->setPublicKeyPath ( cfg->value ( QLatin1String ( "sslPublicKey" ) ).toString() );
  m_configAccessControl->setPrivateKeyPath ( cfg->value ( QLatin1String ( "sslPrivateKey" ) ).toString() );
  m_configAccessControl->setPassPhrase ( cfg->value ( QLatin1String ( "sslPassPhrase" ) ).toByteArray() );
}

void ConfigSSL::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "sslCaCertsDatabase" ), m_certIssuers->getCaBundleFile() );

  // Trusted Hosts Liste
  cfg->remove ( QLatin1String ( "TrustedCertsHosts" ) );
  cfg->beginWriteArray ( QLatin1String ( "TrustedCertsHosts" ) );
  int index = 0;
  foreach ( QString host, m_configTrustedHosts->getTrustedList() )
  {
    cfg->setArrayIndex ( index++ );
    cfg->setValue ( QLatin1String ( "host" ) , host );
  }
  cfg->endArray();

  // Authentifizierungs Kontrolle
  cfg->setValue ( QLatin1String ( "sslPublicKey" ), m_configAccessControl->getPupKey() );
  cfg->setValue ( QLatin1String ( "sslPrivateKey" ), m_configAccessControl->getPrivKey() );
  cfg->setValue ( QLatin1String ( "sslPassPhrase" ), m_configAccessControl->getPassPhrase() );
}

void ConfigSSL::defaults()
{
  m_certIssuers->setCaCertDatabase ( QLatin1String ( "/etc/ssl/certs/ca-certificates.crt" ) );
}

ConfigSSL::~ConfigSSL()
{}
