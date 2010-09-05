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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "geolocation.h"
#include "hostinfodialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QHostAddress>

/* GeoIP */
#include <GeoIP.h>

GeoLocation::GeoLocation ( QWidget * parent, Settings * settings )
    : QWidget ( parent )
    , cfg ( settings )
    , defaultIcon ( QIcon ( QString::fromUtf8 ( ":/flags/icons/flags/europeanunion.png" ) ) )
    , hostName ( QLatin1String ( "localhost" ) )
{
  setObjectName ( QLatin1String ( "geolocation" ) );
  setToolTip ( trUtf8 ( "country code from host address." ) );
  setStatusTip ( trUtf8 ( "country code from host address." ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QString fallbackPath ( GEOIP_DATABASE_PATH );
  fallbackPath.append ( QDir::separator() );
  fallbackPath.append ( "GeoIP.dat" );

  databasePath = cfg->strValue ( QLatin1String ( "GeoIP_Database" ), fallbackPath );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "geolocationlayout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  // Button erstellen
  m_toolButton = new QToolButton ( this );
  m_toolButton->setObjectName ( QLatin1String ( "geolocationbutton" ) );
  m_toolButton->setIcon ( defaultIcon );
  m_toolButton->setAutoRaise ( true );
  layout->addWidget ( m_toolButton );

  setLayout ( layout );

  connect ( m_toolButton, SIGNAL ( clicked() ),
            this, SLOT ( showDetails() ) );
}

void GeoLocation::setFlag ( const QString &code )
{
  if ( code.isEmpty() )
  {
    m_toolButton->setIcon ( defaultIcon );
    return;
  }

  QIcon icon ( QString::fromUtf8 ( ":/flags/icons/flags/%1.png" ).arg ( code.toLower() ) );
  m_toolButton->setIcon ( icon );
}

/**
* In diser Methode wird mit GeoIP der Ländercode ermittelt und
* bei Erfolgreicher auflösung das Icon gesetzt!
*/
void GeoLocation::setGeoAddress ( const QString &address )
{
  const char* addr = address.toAscii().data();
  GeoIP* m_geoip;

  QByteArray path = databasePath.toAscii();

  /* Read from filesystem, check for updated file */
  m_geoip = GeoIP_open ( path.constData(), ( GEOIP_STANDARD | GEOIP_CHECK_CACHE ) );
  if ( m_geoip == NULL )
  {
    qWarning ( "(XHTMLDBG) Can not open GeoIP Database from %s", GEOIP_DATABASE_PATH );
    GeoIP_delete ( m_geoip );
    return;
  }

  const char* retval = GeoIP_country_code_by_addr ( m_geoip, addr );
  if ( retval == NULL )
  {
    GeoIP_delete ( m_geoip );
    return;
  }

  QString countryCode ( retval );
  setFlag ( countryCode );
  GeoIP_delete ( m_geoip );
}

/**
* Mit diesem Slot wird die IP Adresse ermitteln und an
* @ref setGeoAddress übergeben.
*/
void GeoLocation::fetchFromHost ( const QHostInfo &host )
{
  foreach ( QHostAddress address, host.addresses() )
  {
    // Kein (INADDR_ANY oder in6addr_any) oder (IPv6 Adresse) anzeigen.
    if ( address.isNull() || ! address.scopeId().isEmpty() )
      continue;

    if ( address.toIPv4Address() )
    {
      setGeoAddress ( address.toString() );
      break;
    }
  }
}

/**
* Bei einem anklicken des ToolButtons die kompletten
* Hostinformationen beziehen!
*/
void GeoLocation::showDetails()
{
  if ( hostName.isEmpty() )
    return;

  HostInfoDialog* dialog = new HostInfoDialog ( this );
  dialog->getHostInfo ( hostName );
}

/**
* Diese Methode wird von @ref Window::requestsFinished aufgerufen.
* @note Es wird dabei \b nicht überprüft ob es sich um eine URL mit
* dem Schema http* handelt. Die Prüfung auf fehlenden Hostnamen
* muss in dieser methode erfolgen.
*/
void GeoLocation::setHostName ( const QString &host )
{
  // Immer zuerst zurück setzen.
  setFlag ( QString() );

  // Wenn Leer aussteigen!
  if ( host.isEmpty() )
    return;

  hostName = host;
  QHostInfo::lookupHost ( host, this, SLOT ( fetchFromHost ( const QHostInfo & ) ) );
}

GeoLocation::~GeoLocation()
{}
