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

#include "geolocation.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* QtNetwork */
// #include <QtNetwork>

GeoLocation::GeoLocation ( QWidget * parent )
    : QWidget ( parent )
    , defaultIcon ( QIcon ( QString::fromUtf8 ( ":/flags/icons/flags/europeanunion.png" ) ) )
{
  setObjectName ( QLatin1String ( "geolocation" ) );
  setToolTip ( trUtf8 ( "find country for current host address." ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "geolocationlayout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  // Button erstellen
  m_toolButton = new QToolButton ( this );
  m_toolButton->setObjectName ( QLatin1String ( "geolocationbutton" ) );
  m_toolButton->setIcon ( defaultIcon );
  m_toolButton->setAutoRaise ( true );
  // m_toolButton->setPopupMode ( QToolButton::InstantPopup );
  m_toolButton->setEnabled ( false );
  layout->addWidget ( m_toolButton );

  setLayout ( layout );
}

/**
* Diese Methode wird von @ref Window::requestsFinished aufgerufen.
* @note Es wird dabei \b nicht überprüft ob es sich um eine URL mit
* dem Schema http* handelt. Die Prüfung auf fehlenden Hostnamen
* muss in dieser Klasse erfolgen.
*/
void GeoLocation::setHostName ( const QString &host )
{
  if ( host.isEmpty() )
  {
    // Wenn Leer dann alles zurück setzen!
    m_toolButton->setIcon ( defaultIcon );
    m_toolButton->setEnabled ( false );
    return;
  }

  qDebug() << Q_FUNC_INFO << host;
}

GeoLocation::~GeoLocation()
{}
