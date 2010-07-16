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

#include "configextras.h"
#include "configutils.h"
#ifndef GEOIP_DATABASE_PATH
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

ConfigExtras::ConfigExtras ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Extensions" ), parent )
    , databasePath ( QString ( GEOIP_DATABASE_PATH ) )
{
  setObjectName ( QLatin1String ( "config_page_geo" ) );
  setNotice ( true );
  setCheckable ( false );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_extensions_main_layout" ) );
  centralWidget->setLayout ( verticalLayout );

  QGroupBox* geoipGroup = new QGroupBox ( trUtf8 ( "Geological IP Resolver" ), centralWidget );
  geoipGroup->setObjectName ( QLatin1String ( "config_page_extensions_group_geoip" ) );

  QHBoxLayout* geoipLayout = new QHBoxLayout ( geoipGroup );
  geoipLayout->setObjectName ( QLatin1String ( "config_page_extensions_group_geoip_layout" ) );

  QLabel* infoGeoIP = new QLabel ( geoipGroup );
  infoGeoIP->setObjectName ( QLatin1String ( "config_page_extensions_label_geoip_info" ) );
  infoGeoIP->setText ( trUtf8 ( "Database" ) );
  geoipLayout->addWidget ( infoGeoIP );

  m_lineEditGeoIP = new QLineEdit ( geoipGroup );
  m_lineEditGeoIP->setObjectName ( QLatin1String ( "config_page_extensions_edit_geoip" ) );
  geoipLayout->addWidget ( m_lineEditGeoIP );

  QToolButton* btn = new QToolButton ( geoipGroup );
  btn->setObjectName ( QLatin1String ( "config_page_extensions_button_geoip_dialog" ) );
  btn->setIcon ( ConfigUtils::folderIcon() );
  geoipLayout->addWidget ( btn );

  verticalLayout->addWidget ( geoipGroup );

  connect ( btn, SIGNAL ( clicked() ),
            this, SLOT ( openDatabaseDialog() ) );

  connect ( m_lineEditGeoIP, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
}

/**
* Sucht nach dem Pfad zur GeoIP.dat Datenbank
*/
const QString ConfigExtras::absoluteDatabasePath()
{
  QDir d ( databasePath );
  QFileInfo info;
  info.setFile ( d, "GeoIP.dat" );
  if ( info.exists() )
    return info.absoluteFilePath();
  else
    return d.absolutePath();
}

/**
* Öffnet den Dialog zum setzen der Datenbank Datei
*/
void ConfigExtras::openDatabaseDialog()
{
  QStringList dat;
  dat << trUtf8 ( "GeoIP Database %1" ).arg ( "*.dat *.DAT" );
  QString p = ConfigUtils::findFileDialog ( m_lineEditGeoIP->text(), dat, this );
  if ( p.isEmpty() )
    return;

  m_lineEditGeoIP->setText ( p );
}

/**
* Signal weiterleitung für @ref modified
*/
void ConfigExtras::dataChanged ( const QString & )
{
  emit modified ( true );
}

/**
* Externer Aufruf für @ref QLineEdit::setText
*/
void ConfigExtras::setDatabasePath ( const QString &p )
{
  m_lineEditGeoIP->setText ( p );
}

void ConfigExtras::load ( QSettings * cfg )
{
  m_lineEditGeoIP->setText ( cfg->value ( QLatin1String ( "GeoIP_Database" ), absoluteDatabasePath() ).toString() );
}

void ConfigExtras::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "GeoIP_Database" ), m_lineEditGeoIP->text() );
}

ConfigExtras::~ConfigExtras()
{}
