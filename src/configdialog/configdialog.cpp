/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "configdialog.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

/*
UserAgentLanguage
*/

ConfigDialog::ConfigDialog ( QWidget * parent, QSettings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "configdialog" ) );
  setWindowTitle ( trUtf8 ( "Configure xhtmldbg[*]" ) );
  setMinimumWidth ( 550 );
  setMinimumHeight ( 450 );
  setSizeGripEnabled ( true );

  setupUi ( this );

  // Modifications
  setCacheLoadControlComboBoxItems();


  // Load Saved Settings
  loadSettings();

  // Modify ButtonBox
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );
  m_buttonReset = buttonBox->addButton ( QDialogButtonBox::Reset );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );

  // connect ( , SIGNAL(), this, SLOT ( setModified() ) );
  // Check and Radio Boxes
  connect ( AutoCheck, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( AutoFormat, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( AutoDisabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( tabStartShow0, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( tabStartShow1, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( SourceIsFromCacheAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( DoNotBufferUploadDataAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( HttpPipeliningAllowedAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( HttpPipeliningWasUsedAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( bootsplash, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( DeveloperExtrasEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( AutoLoadImages, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( JavascriptEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( PluginsEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  // Group Boxes
  connect ( CacheSaveControlAttribute, SIGNAL ( clicked ( bool ) ), this, SLOT ( setModified() ) );
  // Spin Boxes
  connect ( DefaultFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( DefaultFixedFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( MaxHistoryItems, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  // Line Edits
  connect ( StartUpUrl, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );

  // Buttons
  connect ( m_buttonSave, SIGNAL ( clicked() ), this, SLOT ( saveSettings() ) );
  connect ( m_buttonReset, SIGNAL ( clicked() ), this, SLOT ( loadSettings() ) );
  connect ( m_buttonRestore, SIGNAL ( clicked() ), this, SLOT ( restoreSettings() ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
}

void ConfigDialog::setCacheLoadControlComboBoxItems()
{
  // Modify ComboBox CacheLoadControlAttribute
  CacheLoadControlAttribute->clear();
  CacheLoadControlAttribute->addItem ( trUtf8 ( "Always load from network." ) );
  CacheLoadControlAttribute->setItemData ( 0, QNetworkRequest::AlwaysNetwork, Qt::UserRole );

  CacheLoadControlAttribute->addItem ( trUtf8 ( "Load from the Network if cache is expired." ) );
  CacheLoadControlAttribute->setItemData ( 1, QNetworkRequest::PreferNetwork, Qt::UserRole );

  CacheLoadControlAttribute->addItem ( trUtf8 ( "Load from Cache if entry exists." ) );
  CacheLoadControlAttribute->setItemData ( 2, QNetworkRequest::PreferCache, Qt::UserRole );

  CacheLoadControlAttribute->addItem ( trUtf8 ( "Always load from Cache." ) );
  CacheLoadControlAttribute->setItemData ( 3, QNetworkRequest::AlwaysCache, Qt::UserRole );

  CacheLoadControlAttribute->setCurrentIndex ( 0 );
}

void ConfigDialog::loadHeaderDefinitions()
{
  cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
  QStringList keys = cfg->allKeys();
  if ( keys.size() > 1 )
  {
    headersTable->setRowCount( keys.size() );
    int r = 0;
    foreach ( QString key, keys )
    {
      QString val = cfg->value( key ).toString();
      if ( val.isEmpty() )
        continue;

      headersTable->setItem ( r, 0, new QTableWidgetItem ( key ) );
      headersTable->setItem ( r, 1, new QTableWidgetItem ( val ) );
      r++;
    }
  }
  cfg->endGroup();
}

void ConfigDialog::saveHeaderDefinitions()
{
  int rows = headersTable->rowCount();
  cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
  for ( int r = 0; r < rows; r++ )
  {
    QString key = headersTable->item ( r, 0 )->data ( Qt::EditRole ).toString();
    QString val = headersTable->item ( r, 1 )->data ( Qt::EditRole ).toString();
    cfg->setValue ( key, val );
  }
  cfg->endGroup();
}

void ConfigDialog::setModified()
{
  setWindowModified ( true );
}

void ConfigDialog::loadSettings()
{
  // Radio Buttons
  foreach ( QRadioButton* box, findChildren<QRadioButton *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    box->setChecked ( cfg->value ( box->objectName(), box->isChecked() ).toBool() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    box->setChecked ( cfg->value ( box->objectName(), box->isChecked() ).toBool() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() )
      continue;

    edit->setText ( cfg->value ( edit->objectName(), edit->text() ).toString() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    box->setValue ( cfg->value ( box->objectName(), box->minimum() ).toUInt() );
  }

  loadHeaderDefinitions();
  setWindowModified ( false );
}

void ConfigDialog::saveSettings()
{
  // Radio Buttons
  foreach ( QRadioButton* box, findChildren<QRadioButton *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->setValue ( box->objectName(), box->isChecked() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->setValue ( box->objectName(), box->isChecked() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() )
      continue;

    cfg->setValue ( edit->objectName(), edit->text() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->setValue ( box->objectName(), box->value() );
  }

  saveHeaderDefinitions();
  setWindowModified ( false );
}

void ConfigDialog::restoreSettings()
{
  // Radio Buttons
  foreach ( QRadioButton* box, findChildren<QRadioButton *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->remove ( box->objectName() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->remove ( box->objectName() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() )
      continue;

    cfg->remove ( edit->objectName() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() )
      continue;

    cfg->remove ( box->objectName() );
  }

  setWindowModified ( false );

  QMessageBox::information ( this, trUtf8 ( "Notice" ),
                             trUtf8 ( "Settings Restored.\nPlease restart the Configuration Dialog." ) );

}

void ConfigDialog::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
    status = QMessageBox::question ( this, trUtf8 ( "Unsaved Changes" ),
                                     trUtf8 ( "Found unsaved Changes.\nDo you realy wan to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  if ( status == QMessageBox::Yes )
    accept();
}

ConfigDialog::~ConfigDialog()
{}
