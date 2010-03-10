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
#include "editcookiestable.h"
#include "proxysettings.h"
#include "sslconfig.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>

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

  QIcon icon;
  // Set Tab Icons
  tabWidget->setTabIcon ( 0, icon.fromTheme ( QLatin1String ( "preferences-system-windows-actions" ) ) );
  tabWidget->setTabIcon ( 1, icon.fromTheme ( QLatin1String ( "preferences-system-windows" ) ) );
  tabWidget->setTabIcon ( 2, icon.fromTheme ( QLatin1String ( "preferences-system-network-sharing" ) ) );
  tabWidget->setTabIcon ( 3, icon.fromTheme ( QLatin1String ( "preferences-system-performance" ) ) );
  tabWidget->setTabIcon ( 4, icon.fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
  tabWidget->setTabIcon ( 5, icon.fromTheme ( QLatin1String ( "preferences-system-network" ) ) );
  tabWidget->setTabIcon ( 6, icon.fromTheme ( QLatin1String ( "preferences-web-browser-identification" ) ) );
  // Set PushButton Icons
  removeCookieItem->setIcon ( icon.fromTheme ( QLatin1String ( "list-remove" ) ) );
  removeAllCookies->setIcon ( icon.fromTheme ( QLatin1String ( "archive-remove" ) ) );
  addCookieArrangement->setIcon ( icon.fromTheme ( QLatin1String ( "list-add" ) ) );

  // Modifications
  setCacheLoadControlComboBoxItems();

  // Modify ButtonBox
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );
  m_buttonReset = buttonBox->addButton ( QDialogButtonBox::Reset );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );

  // Load Saved Settings
  loadSettings();

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

  // Combo Boxes
  connect ( CacheLoadControlAttribute, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( setModified() ) );

  // Spin Boxes
  connect ( DefaultFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( DefaultFixedFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( MaxHistoryItems, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );

  // Line Edits
  connect ( StartUpUrl, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );

  // Tables
  connect ( headersTable, SIGNAL ( itemSelectionChanged() ), this, SLOT ( setModified() ) );

  // Sub Widget's
  connect ( cookiesTable, SIGNAL ( modified() ), this, SLOT ( setModified() ) );
  connect ( proxySettings, SIGNAL ( modified () ), this, SLOT ( setModified() ) );
  connect ( sslConfigWidget, SIGNAL ( modified () ), this, SLOT ( setModified() ) );

  // Buttons
  connect ( addCookieArrangement, SIGNAL ( clicked() ), this, SLOT ( addCookieAccess() ) );
  connect ( removeCookieItem, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeItem() ) );
  connect ( removeAllCookies, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeAll() ) );
  // Dialog Buttons
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
  CacheLoadControlAttribute->insertItem ( 0, trUtf8 ( "Always load from network." ), QNetworkRequest::AlwaysNetwork );
  CacheLoadControlAttribute->setItemData ( 0, QNetworkRequest::AlwaysNetwork, Qt::UserRole );

  CacheLoadControlAttribute->insertItem ( 1, trUtf8 ( "Load from the Network if cache is expired." ), QNetworkRequest::PreferNetwork );
  CacheLoadControlAttribute->setItemData ( 1, QNetworkRequest::PreferNetwork, Qt::UserRole );

  CacheLoadControlAttribute->insertItem ( 2, trUtf8 ( "Load from Cache if entry exists." ), QNetworkRequest::PreferCache );
  CacheLoadControlAttribute->setItemData ( 2, QNetworkRequest::PreferCache, Qt::UserRole );

  CacheLoadControlAttribute->insertItem ( 3, trUtf8 ( "Always load from Cache." ), QNetworkRequest::AlwaysCache );
  CacheLoadControlAttribute->setItemData ( 3, QNetworkRequest::AlwaysCache, Qt::UserRole );
}

void ConfigDialog::loadHeaderDefinitions()
{
  cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
  QStringList keys = cfg->allKeys();
  if ( keys.size() >= 1 )
  {
    headersTable->setRowCount ( keys.size() );
    int r = 0;
    foreach ( QString key, keys )
    {
      QString val = cfg->value ( key ).toString();
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
  cfg->remove ( QLatin1String ( "HeaderDefinitions" ) );
  if ( rows >= 1 )
  {
    cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
    for ( int r = 0; r < rows; r++ )
    {
      QString key = headersTable->item ( r, 0 )->data ( Qt::EditRole ).toString();
      QString val = headersTable->item ( r, 1 )->data ( Qt::EditRole ).toString();
      cfg->setValue ( key, val );
    }
    cfg->endGroup();
  }
}

void ConfigDialog::addCookieAccess()
{
  if ( addCookieDomain->text().isEmpty() )
    return;

  QUrl url ( addCookieDomain->text() );
  if ( url.isValid() && url.scheme().contains ( "http" ) )
  {
    QString domain = url.host().remove ( QRegExp ( "\\bwww\\." ) );
    if ( cookiesTable->addCookie ( selectedArrangementType->currentIndex(), domain ) )
      addCookieDomain->setText ( QLatin1String ( "http://" ) );
    else
      addCookieDomain->setFocus();
  }
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
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    box->setChecked ( cfg->value ( box->objectName(), box->isChecked() ).toBool() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    box->setChecked ( cfg->value ( box->objectName(), box->isChecked() ).toBool() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() || edit->objectName().contains ( "qt_" ) )
      continue;

    edit->setText ( cfg->value ( edit->objectName(), edit->text() ).toString() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    box->setValue ( cfg->value ( box->objectName(), box->minimum() ).toUInt() );
  }

  loadHeaderDefinitions();
  cookiesTable->loadCookieArrangements ( cfg );
  proxySettings->load ( cfg );
  sslConfigWidget->load ( cfg );

  // Cache Control
  QNetworkRequest::CacheLoadControl controlCache = ( QNetworkRequest::CacheLoadControl )
          cfg->value ( QLatin1String ( "CacheLoadControlAttribute" ), QNetworkRequest::AlwaysNetwork ).toUInt();

  CacheLoadControlAttribute->setCurrentIndex ( CacheLoadControlAttribute->findData ( controlCache, Qt::UserRole ) );

  setWindowModified ( false );
}

void ConfigDialog::saveSettings()
{
  // Radio Buttons
  foreach ( QRadioButton* box, findChildren<QRadioButton *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->setValue ( box->objectName(), box->isChecked() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->setValue ( box->objectName(), box->isChecked() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() || edit->objectName().contains ( "qt_" ) )
      continue;

    cfg->setValue ( edit->objectName(), edit->text() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->setValue ( box->objectName(), box->value() );
  }

  saveHeaderDefinitions();

  int cIndex = CacheLoadControlAttribute->itemData ( CacheLoadControlAttribute->currentIndex(), Qt::UserRole ).toUInt();
  cfg->setValue ( QLatin1String ( "CacheLoadControlAttribute" ), cIndex );

  cookiesTable->saveCookieArrangements ( cfg );
  proxySettings->save ( cfg );
  sslConfigWidget->save ( cfg );
  setWindowModified ( false );
}

void ConfigDialog::restoreSettings()
{
  // Radio Buttons
  foreach ( QRadioButton* box, findChildren<QRadioButton *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->remove ( box->objectName() );
  }

  // Check Boxes
  foreach ( QCheckBox* box, findChildren<QCheckBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->remove ( box->objectName() );
  }

  // Line Edits
  foreach ( QLineEdit* edit, findChildren<QLineEdit *>() )
  {
    if ( edit->objectName().isEmpty() || edit->objectName().contains ( "qt_" ) )
      continue;

    cfg->remove ( edit->objectName() );
  }

  // Spin Boxes
  foreach ( QSpinBox* box, findChildren<QSpinBox *>() )
  {
    if ( box->objectName().isEmpty() || box->objectName().contains ( "qt_" ) )
      continue;

    cfg->remove ( box->objectName() );
  }

  cfg->remove ( QLatin1String ( "CacheLoadControlAttribute" ) );
  cfg->remove ( QLatin1String ( "proxyType" ) );
  proxySettings->setType ( QNetworkProxy::NoProxy );
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
