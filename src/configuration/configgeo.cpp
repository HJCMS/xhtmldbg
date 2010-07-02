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

#include "configgeo.h"
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
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

ConfigGeo::ConfigGeo ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Geological IP Resolver" ), parent )
    , databasePath ( QString ( GEOIP_DATABASE_PATH ) )
{
  setObjectName ( QLatin1String ( "config_page_geo" ) );
  setNotice ( true );
  setCheckable ( false );

  QGridLayout* gridLayout = new QGridLayout ( centralWidget );
  gridLayout->setObjectName ( QLatin1String ( "config_page_geo_main_layout" ) );
  gridLayout->setContentsMargins ( 0, 5, 0, 5 );

  QLabel* infoEdit = new QLabel ( centralWidget );
  infoEdit->setText ( trUtf8 ( "Database" ) );
  gridLayout->addWidget ( infoEdit, 0, 0, 1, 1 );

  dbEdit = new QLineEdit ( centralWidget );
  gridLayout->addWidget ( dbEdit, 0, 1, 1, 1 );

  QToolButton* btn = new QToolButton ( centralWidget );
  btn->setIcon ( ConfigUtils::folderIcon() );
  gridLayout->addWidget ( btn, 0, 2, 1, 1 );

  centralWidget->setLayout ( gridLayout );

  connect ( btn, SIGNAL ( clicked() ),
            this, SLOT ( openDatabaseDialog() ) );

  connect ( dbEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
}

const QString ConfigGeo::absoluteDatabasePath()
{
  QDir d ( databasePath );
  QFileInfo info;
  info.setFile ( d, "GeoIP.dat" );
  if ( info.exists() )
    return info.absoluteFilePath();
  else
    return d.absolutePath();
}

void ConfigGeo::openDatabaseDialog()
{
  QStringList dat;
  dat << trUtf8 ( "GeoIP Database %1" ).arg ( "*.dat *.DAT" );
  QString p = ConfigUtils::findFileDialog ( dbEdit->text(), dat, this );
  if ( p.isEmpty() )
    return;

  dbEdit->setText ( p );
}

void ConfigGeo::dataChanged ( const QString & )
{
  emit modified ( true );
}

void ConfigGeo::setDatabasePath ( const QString &p )
{
  dbEdit->setText ( p );
}

void ConfigGeo::load ( QSettings * cfg )
{
  dbEdit->setText ( cfg->value ( QLatin1String ( "GeoIP_Database" ), absoluteDatabasePath() ).toString() );
}

void ConfigGeo::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "GeoIP_Database" ), dbEdit->text() );
}

void ConfigGeo::defaults()
{
  dbEdit->setText ( absoluteDatabasePath() );
}

ConfigGeo::~ConfigGeo()
{}
