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

#include "configuration.h"
#include "configurationmenu.h"
#include "stackedwidget.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QScrollArea>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

Configuration::Configuration ( QWidget * parent, QSettings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "configurationdialog" ) );
  setWindowTitle ( trUtf8 ( "Configure xhtmldbg[*]" ) );
  setContentsMargins ( 0, 0, 0, 10 );
  setSizeGripEnabled ( true );
  setMinimumHeight ( 280 );
  setMinimumWidth ( 550 );

  // Vertikales Design
  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "config_vertical_layout" ) );
  verticalLayout->setContentsMargins ( 5, 5, 5, 5 );

  // Horizontales Design
  m_splitter = new QSplitter ( Qt::Horizontal, this );
  m_splitter->setObjectName ( QLatin1String ( "config_horizontal_splitter" ) );
  m_splitter->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  m_splitter->setChildrenCollapsible ( false );
  verticalLayout->addWidget ( m_splitter );

  // Scroolbereich festlegen
  QScrollArea* scrollArea = new QScrollArea ( m_splitter );
  scrollArea->setObjectName ( QLatin1String ( "config_scroll_area" ) );
  scrollArea->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  scrollArea->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
  scrollArea->setContentsMargins ( 2, 0, 2, 0 );
  scrollArea->setFrameStyle ( QFrame::NoFrame );
  scrollArea->setWidgetResizable ( true );
  m_splitter->insertWidget ( 0, scrollArea );

  // Menü Einträge
  menuWidget = new ConfigurationMenu ( this, cfg );
  m_splitter->insertWidget ( 1, menuWidget );

  // Konfigurations Seiten
  m_stackedWidget = new StackedWidget ( m_splitter, cfg );
  scrollArea->setWidget ( m_stackedWidget );

  // Dialog Knöpfe
  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  buttonBox->setObjectName ( QLatin1String ( "config_button_box" ) );
  buttonBox->setStandardButtons ( QDialogButtonBox::NoButton );
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );
  m_buttonReset = buttonBox->addButton ( QDialogButtonBox::Reset );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );
  verticalLayout->addWidget ( buttonBox );

  // Vertikales abschließen
  setLayout ( verticalLayout );

  // Signale
  connect ( menuWidget, SIGNAL ( itemClicked ( int ) ),
            m_stackedWidget, SLOT ( setCurrentIndex ( int ) ) );

  connect ( m_stackedWidget, SIGNAL ( settingsChanged ( bool ) ),
            this, SLOT ( setWindowModified ( bool ) ) );

  connect ( m_buttonSave, SIGNAL ( clicked() ),
            m_stackedWidget, SLOT ( saveSettings() ) );

  connect ( m_buttonReset, SIGNAL ( clicked() ),
            m_stackedWidget, SLOT ( loadSettings() ) );

  connect ( m_buttonRestore, SIGNAL ( clicked() ),
            m_stackedWidget, SLOT ( restoreSettings() ) );

  connect ( m_buttonCancel, SIGNAL ( clicked() ),
            this, SLOT ( reject() ) );

  connect ( m_buttonClose, SIGNAL ( clicked() ),
            this, SLOT ( quit() ) );

  m_splitter->restoreState ( cfg->value ( "ConfigDialog/SplitterState" ).toByteArray() );
  update();
}

/**
* Vor dem Beenden nach ungespeicherten Einträgen suchen
* und bei bedarf zuvor eine Warnmeldung ausgeben.
*/
void Configuration::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
    status = QMessageBox::question ( this, trUtf8 ( "Unsaved Changes" ),
                                     trUtf8 ( "Found unsaved Changes.\nDo you realy want to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  cfg->setValue ( "ConfigDialog/SplitterState", m_splitter->saveState() );
  if ( status == QMessageBox::Yes )
    accept();
}

Configuration::~Configuration()
{}
