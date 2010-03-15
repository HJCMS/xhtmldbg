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

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QComboBox>
#include <QtGui/QFileDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QStyle>
#include <QtGui/QToolButton>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>

ConfigDialog::ConfigDialog ( QWidget * parent, QSettings * settings )
    : QDialog ( parent )
    , cfg ( settings )
    , ssl ( QSslConfiguration::defaultConfiguration () )
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
  tabWidget->setTabIcon ( 2, icon.fromTheme ( QLatin1String ( "preferences-system-performance" ) ) );
  tabWidget->setTabIcon ( 3, icon.fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
  tabWidget->setTabIcon ( 4, icon.fromTheme ( QLatin1String ( "preferences-system-network-sharing" ) ) );
  tabWidget->setTabIcon ( 5, icon.fromTheme ( QLatin1String ( "preferences-web-browser-identification" ) ) );
  // Set PushButton Icons
  removeCookieItem->setIcon ( icon.fromTheme ( QLatin1String ( "list-remove" ) ) );
  removeAllCookies->setIcon ( icon.fromTheme ( QLatin1String ( "archive-remove" ) ) );
  addCookieArrangement->setIcon ( icon.fromTheme ( QLatin1String ( "list-add" ) ) );
  // Set ToolButton Icons
  clearStartupUrlBtn->setIcon ( icon.fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  QIcon openFolderIcon = icon.fromTheme ( QLatin1String ( "document-open" ) );
  openCaCertButton->setIcon ( openFolderIcon );
  openPupKeyButton->setIcon ( openFolderIcon );
  openPrivKeyButton->setIcon ( openFolderIcon );
  clearPassButton->setIcon ( icon.fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  removeFromWhiteListBtn->setIcon ( icon.fromTheme ( QLatin1String ( "list-remove" ) ) );
  addToWhiteListBtn->setIcon ( icon.fromTheme ( QLatin1String ( "list-add" ) ) );
  clearWhiteListBtn->setIcon ( icon.fromTheme ( QLatin1String ( "archive-remove" ) ) );

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
  connect ( DoNotBufferUploadDataAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( HttpPipeliningAllowedAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( HttpPipeliningWasUsedAttribute, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( bootsplash, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( DeveloperExtrasEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( AutoLoadImages, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( JavascriptEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( PluginsEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );
  connect ( JavaEnabled, SIGNAL ( released() ), this, SLOT ( setModified() ) );

  // Spin Boxes
  connect ( DefaultFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( DefaultFixedFontSize, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( MaxHistoryItems, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );

  // Line Edits
  connect ( StartUpUrl, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( sslPublicKey, SIGNAL ( editingFinished () ), this, SLOT ( setModified() ) );
  connect ( sslPrivateKey, SIGNAL ( editingFinished () ), this, SLOT ( setModified() ) );
  connect ( qt_ssl_pass_phrase, SIGNAL ( editingFinished () ), this, SLOT ( setModified() ) );
  connect ( sslCaCertsDatabase, SIGNAL ( editingFinished () ),
            this, SLOT ( setCaCertDatabase() ) );
  connect ( sslCaCertsDatabase, SIGNAL ( textEdited ( const QString & ) ),
            this, SLOT ( setCaCertDatabase ( const QString & ) ) );

  // Tables
  connect ( headersTable, SIGNAL ( itemSelectionChanged() ), this, SLOT ( setModified() ) );

  // Sub Widget's
  connect ( cookiesTable, SIGNAL ( modified() ), this, SLOT ( setModified() ) );
  connect ( proxySettings, SIGNAL ( modified () ), this, SLOT ( setModified() ) );

  // Buttons
  connect ( addCookieArrangement, SIGNAL ( clicked() ), this, SLOT ( addCookieAccess() ) );
  connect ( removeCookieItem, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeItem() ) );
  connect ( removeAllCookies, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeAll() ) );
  connect ( openPrivKeyButton, SIGNAL ( clicked() ), this, SLOT ( getPrivKeyDialog() ) );
  connect ( openPupKeyButton, SIGNAL ( clicked() ), this, SLOT ( getPupKeyDialog() ) );
  connect ( openCaCertButton, SIGNAL ( clicked() ), this, SLOT ( getCaCertDatabaseDialog() ) );
  connect ( removeFromWhiteListBtn, SIGNAL ( clicked() ), this, SLOT ( delTrustedHost() ) );
  connect ( addToWhiteListBtn, SIGNAL ( clicked() ), this, SLOT ( addTrustedHost() ) );
  connect ( clearWhiteListBtn, SIGNAL ( clicked() ), trustedHostsList, SLOT ( clear() ) );
  connect ( clearWhiteListBtn, SIGNAL ( clicked() ), this, SLOT ( setModified() ) );
  connect ( openHighlightSettings, SIGNAL ( clicked() ),
            this, SLOT ( setDomTreeHighlightColor() ) );

  // Dialog Buttons
  connect ( m_buttonSave, SIGNAL ( clicked() ), this, SLOT ( saveSettings() ) );
  connect ( m_buttonReset, SIGNAL ( clicked() ), this, SLOT ( loadSettings() ) );
  connect ( m_buttonRestore, SIGNAL ( clicked() ), this, SLOT ( restoreSettings() ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
}

void ConfigDialog::setCaCertIssuerTable()
{
  QList<QSslCertificate> certs = ssl.caCertificates();
  if ( certs.size() < 1 )
    return;

  QTableWidgetItem ref ( QTableWidgetItem::UserType );
  ref.setFlags ( ( Qt::ItemIsSelectable | Qt::ItemIsEnabled ) );

  sslIssuers->clearContents();
  sslIssuers->setRowCount ( certs.size() );
  int row = 0;
  foreach ( QSslCertificate cert, certs )
  {
    QTableWidgetItem* item0 = new QTableWidgetItem ( ref );
    item0->setText ( cert.issuerInfo ( QSslCertificate::Organization ) );
    sslIssuers->setItem ( row, 0, item0 );

    QTableWidgetItem* item1 = new QTableWidgetItem ( ref );
    item1->setText ( cert.issuerInfo ( QSslCertificate::CommonName ) );
    sslIssuers->setItem ( row, 1, item1 );

    QDateTime stamp = cert.expiryDate();
    QTableWidgetItem* item2 = new QTableWidgetItem ( ref );
    item2->setText ( stamp.toString ( Qt::DefaultLocaleLongDate ) );
    if ( stamp < QDateTime::currentDateTime () )
      item2->setBackground ( Qt::yellow );
    sslIssuers->setItem ( row, 2, item2 );
    row++;
  }
  sslIssuers->horizontalHeader()->setResizeMode ( QHeaderView::ResizeToContents );
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

void ConfigDialog::loadUntrustedHostsWhiteList()
{
  QStringList list;
  int size = cfg->beginReadArray ( QLatin1String ( "TrustedCertsHosts" ) );

  if ( size < 0 )
    list << QLatin1String ( "localhost" );

  trustedHostsList->clear();
  for ( int i = 0; i < size; ++i )
  {
    cfg->setArrayIndex ( i );
    list.append ( cfg->value ( "host" ).toString() );
  }
  cfg->endArray();
  trustedHostsList->addItems ( list );
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

void ConfigDialog::saveUntrustedHostsWhiteList()
{
  cfg->remove ( QLatin1String ( "TrustedCertsHosts" ) );
  cfg->beginWriteArray ( QLatin1String ( "TrustedCertsHosts" ) );
  for ( int i = 0; i < trustedHostsList->count(); ++i )
  {
    cfg->setArrayIndex ( i );
    cfg->setValue ( QLatin1String ( "host" ) , trustedHostsList->item ( i )->text() );
  }
  cfg->endArray();
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

void ConfigDialog::addTrustedHost()
{
  if ( trustedEdit->text().isEmpty() )
    return;

  QUrl url;
  url.setScheme ( QLatin1String ( "https" ) );
  url.setHost ( trustedEdit->text() );
  if ( ! url.isValid() )
    return;

  trustedHostsList->addItem ( url.host() );
  trustedEdit->clear();
  trustedEdit->setFocus();
  setModified();
}

void ConfigDialog::delTrustedHost()
{
  foreach ( QListWidgetItem* item, trustedHostsList->selectedItems() )
  {
    if ( item->isSelected() )
    {
      trustedHostsList->removeItemWidget ( item );
      delete item;
      break;
    }
  }
  setModified();
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

  highlightColor = cfg->value ( QLatin1String ( "highlightColor" ), QLatin1String ( "yellow" ) ).toString();
  defaultHighlightStyleSheet = QString ( "background-color: %1;" ).arg ( highlightColor );
  labelHighlightPreview->setStyleSheet ( defaultHighlightStyleSheet );

  loadHeaderDefinitions();
  cookiesTable->loadCookieArrangements ( cfg );
  proxySettings->load ( cfg );

  if ( ! sslCaCertsDatabase->text().isEmpty() )
    setCaCertDatabase ();

  loadUntrustedHostsWhiteList();

  QString p ( QByteArray::fromBase64 ( cfg->value ( QLatin1String ( "sslPassPhrase" ) ).toByteArray() ) );
  qt_ssl_pass_phrase->setText ( p );

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

  cfg->setValue ( QLatin1String ( "highlightColor" ), highlightColor );

  saveHeaderDefinitions();
  saveUntrustedHostsWhiteList();

  QByteArray p = qt_ssl_pass_phrase->text().toAscii();
  cfg->setValue ( QLatin1String ( "sslPassPhrase" ), p.toBase64() );

  cookiesTable->saveCookieArrangements ( cfg );
  proxySettings->save ( cfg );
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

  cfg->remove ( QLatin1String ( "highlightColor" ) );
  cfg->remove ( QLatin1String ( "proxyType" ) );
  proxySettings->setType ( QNetworkProxy::NoProxy );
  setWindowModified ( false );

  QMessageBox::information ( this, trUtf8 ( "Notice" ),
                             trUtf8 ( "Settings Restored.\nPlease restart the Configuration Dialog." ) );

}

void ConfigDialog::setCaCertDatabase ( const QString &p )
{
  QString path = ( p.isEmpty() ) ? sslCaCertsDatabase->text() : p;
  QFileInfo db ( path );
  if ( db.exists() )
  {
    QList<QSslCertificate> caCerts = ssl.caCertificates();
    caCerts << QSslCertificate::fromPath ( db.absoluteFilePath(), QSsl::Pem, QRegExp::FixedString );
    ssl.setCaCertificates ( caCerts );
    setCaCertIssuerTable();
  }
}

void ConfigDialog::setDomTreeHighlightColor()
{
  highlightColor = cfg->value ( QLatin1String ( "highlightColor" ), QLatin1String ( "yellow" ) ).toString();
  QColorDialog* dialog = new QColorDialog ( this );
  dialog->setCurrentColor ( QColor ( highlightColor ) );
  if ( dialog->exec() )
  {
    highlightColor = dialog->selectedColor().name();
    defaultHighlightStyleSheet = QString ( "background-color: %1;" ).arg ( highlightColor );
    labelHighlightPreview->setStyleSheet ( defaultHighlightStyleSheet );
    setModified();
  }
}

void ConfigDialog::getPrivKeyDialog()
{
  QString path ( sslPrivateKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  sslPrivateKey->clear();

  QFileInfo db ( path );
  if ( db.exists() )
    sslPrivateKey->setText ( db.absoluteFilePath() );

  setModified();
}

void ConfigDialog::getPupKeyDialog()
{
  QString path ( sslPublicKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  sslPublicKey->clear();

  QFileInfo db ( path );
  if ( db.exists() )
    sslPublicKey->setText ( db.absoluteFilePath() );

  setModified();
}

void ConfigDialog::getCaCertDatabaseDialog()
{
  QString path ( sslCaCertsDatabase->text() );
  QStringList filt;
  filt << trUtf8 ( "CA Bundle %1" ).arg ( "*.crt ca-*" );
  filt << trUtf8 ( "Certificate %1" ).arg ( "*.pem *.der" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open CA Database" ), path, filt.join ( ";;" ) );

  sslCaCertsDatabase->clear ();

  QFileInfo db ( path );
  if ( db.exists() )
    sslCaCertsDatabase->setText ( db.absoluteFilePath() );

  setModified();
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
