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

#include "configtidy.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QButtonGroup>

ConfigTidy::ConfigTidy ( QWidget * parent )
    : PageWidget ( trUtf8 ( "QTidy Source Parser" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_tidy" ) );
  setNotice ( false );
  setCheckable ( false );

  QVBoxLayout* vLayout = new QVBoxLayout ( centralWidget );

  m_radioCheck = new QRadioButton ( trUtf8 ( "Enable Auto Source Check" ), centralWidget );
  m_radioCheck->setObjectName ( QLatin1String ( "config_page_tidy_radio_check" ) );
  vLayout->addWidget ( m_radioCheck );

  m_radioFormat = new QRadioButton ( trUtf8 ( "Enable Autoformat and Check Source" ), centralWidget );
  m_radioFormat->setObjectName ( QLatin1String ( "config_page_tidy_radio_format" ) );
  vLayout->addWidget ( m_radioFormat );

  m_radioDisabled = new QRadioButton ( trUtf8 ( "Disable autoformat and autocheck" ), centralWidget );
  m_radioDisabled->setObjectName ( QLatin1String ( "config_page_tidy_radio_disabled" ) );
  m_radioDisabled->setChecked ( true );
  vLayout->addWidget ( m_radioDisabled );

  QButtonGroup* buttonGroup = new QButtonGroup ( centralWidget );
  buttonGroup->setObjectName ( QLatin1String ( "config_page_tidy_button_group" ) );
  buttonGroup->addButton ( m_radioDisabled, 0 );
  buttonGroup->addButton ( m_radioCheck, 1 );
  buttonGroup->addButton ( m_radioFormat, 2 );

  centralWidget->setLayout ( vLayout );

  connect ( buttonGroup, SIGNAL ( buttonClicked ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );
}

void ConfigTidy::itemClicked ( int )
{
  emit modified ( true );
}

void ConfigTidy::load ( Settings * cfg )
{
  if ( cfg->value ( QLatin1String ( "AutoCheck" ), false ).toBool() )
    m_radioCheck->setChecked ( true );
  else if ( cfg->value ( QLatin1String ( "AutoFormat" ), false ).toBool() )
    m_radioFormat->setChecked ( true );
  else
    m_radioDisabled->setChecked ( true );
}

void ConfigTidy::save ( Settings * cfg )
{
  cfg->setValue ( QLatin1String ( "AutoCheck" ), m_radioCheck->isChecked() );
  cfg->setValue ( QLatin1String ( "AutoFormat" ), m_radioFormat->isChecked() );
  cfg->setValue ( QLatin1String ( "AutoDisabled" ), m_radioDisabled->isChecked() );
}

ConfigTidy::~ConfigTidy()
{}
