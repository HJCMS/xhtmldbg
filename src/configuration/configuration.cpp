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
#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif

/* QtCore */
#ifdef XHTMLDBG_DEBUG_VERBOSE
# include <QtCore/QDebug>
#endif
#include <QtCore/QByteArray>
#include <QtCore/QRect>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/**
* Enthält eine Liste veralteter Konfigurationen
* die entfernt werden können!
*/
static inline const QStringList deprecatedList()
{
  QStringList list;
  list << "trustedEdit";
  list << "HeaderDefinitions/Accept-Language";
  list << "bootsplash";
  list << "addCookieDomain";
  list << "MainWindowState";
  list << "MainWindowGeometry";
  list << "iconthemepath";
  return list;
}

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

  /** Alte Konfigurationen entfernen */
  QString xhtmldbgVersion ( XHTMLDBG_VERSION_STRING );
  if ( cfg->value ( QLatin1String ( "Version" ) ).toString() !=  xhtmldbgVersion )
  {
    foreach ( QString param, deprecatedList() )
    {
      cfg->remove ( param );
    }
    cfg->setValue ( QLatin1String ( "Version" ), xhtmldbgVersion );
  }

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

  // Konfigurations Seiten
  m_stackedWidget = new StackedWidget ( m_splitter, cfg );
  m_splitter->insertWidget ( 0, m_stackedWidget );

  // Menü Einträge
  menuWidget = new ConfigurationMenu ( m_splitter, cfg );
  m_splitter->insertWidget ( 1, menuWidget );

  // Dialog Knöpfe
  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  buttonBox->setObjectName ( QLatin1String ( "config_button_box" ) );
  buttonBox->setStandardButtons ( QDialogButtonBox::NoButton );
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );
  m_buttonReset = buttonBox->addButton ( QDialogButtonBox::Reset );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );
  m_buttonRestore->setIcon ( QIcon::fromTheme ( QString::fromUtf8 ( "edit-clear-history" ) ) );
  verticalLayout->addWidget ( buttonBox );

  // Vertikales Design abschließen
  setLayout ( verticalLayout );

  // Widget Signale
  connect ( menuWidget, SIGNAL ( itemClicked ( int ) ),
            m_stackedWidget, SLOT ( setCurrentIndex ( int ) ) );

  connect ( m_stackedWidget, SIGNAL ( settingsChanged ( bool ) ),
            this, SLOT ( setWindowModified ( bool ) ) );

  // Knopf Signale
  connect ( m_buttonSave, SIGNAL ( clicked() ),
            m_stackedWidget, SLOT ( saveSettings() ) );

  connect ( m_buttonReset, SIGNAL ( clicked() ),
            m_stackedWidget, SLOT ( loadSettings() ) );

  connect ( m_buttonRestore, SIGNAL ( clicked() ),
            this, SLOT ( restoreSettings() ) );

  connect ( m_buttonCancel, SIGNAL ( clicked() ),
            this, SLOT ( reject() ) );

  connect ( m_buttonClose, SIGNAL ( clicked() ),
            this, SLOT ( quit() ) );

  m_splitter->restoreState ( cfg->value ( "ConfigDialog/SplitterState" ).toByteArray() );
  // update();
}

/**
* Alle Einstellungen zurück setzen.
* Zuerst an den Benutzer einen Hinweis ausgeben.
* Bei einer positiven Antwort alles entfernen.
*/
void Configuration::restoreSettings()
{
  QMessageBox::StandardButton status;
  status = QMessageBox::warning ( this, trUtf8 ( "Restore Settings" ),
                                  trUtf8 ( "All configuration options will be restored.\nAfter this operation, the configuration dialog will be closed automatically and you must restart the Application.\n\nDo you really want to do this?" ),
                                  ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  if ( status == QMessageBox::Yes )
  {
    cfg->clear ();
    reject();
  }
}

/**
* Vor dem Beenden nach ungespeicherten Einträgen suchen
* und bei bedarf zuvor eine Warnmeldung ausgeben.
*/
void Configuration::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
  {
    status = QMessageBox::question ( this, trUtf8 ( "Unsaved Changes" ),
                                     trUtf8 ( "Found unsaved Changes.\nDo you really want to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );
  }

  cfg->setValue ( "ConfigDialog/SplitterState", m_splitter->saveState() );
  if ( status == QMessageBox::Yes )
    accept();
}

Configuration::~Configuration()
{}
