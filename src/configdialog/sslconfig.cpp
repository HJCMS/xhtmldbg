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

#include "sslconfig.h"

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSizePolicy>
#include <QtGui/QStyle>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

SSLConfig::SSLConfig ( QWidget * parent )
    : QScrollArea ( parent )
    , ssl ( QSslConfiguration::defaultConfiguration () )
{
  setObjectName ( QLatin1String ( "sslconfig" ) );
  setWidgetResizable ( true );

  centeredWidget = new QWidget ( this );
  centeredWidget->setObjectName ( QLatin1String ( "layerwidget" ) );
  centeredWidget->setMinimumWidth ( 500 );

  // Label Alignments
  Qt::Alignment floatRight = ( Qt::AlignRight | Qt::AlignVCenter );
  Qt::Alignment justifyAlign = ( Qt::AlignJustify | Qt::AlignTop );

  QIcon openIcon = QIcon::fromTheme ( QLatin1String ( "document-open" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( centeredWidget );

  QGroupBox* m_groupBox1 = new QGroupBox ( centeredWidget );
  m_groupBox1->setTitle ( trUtf8 ( "Client Authentication and Access Control" ) );
  m_groupBox1->setCheckable ( false );
  m_groupBox1->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );

  QGridLayout* box1Layout = new QGridLayout ( m_groupBox1 );
  box1Layout->setVerticalSpacing ( 5 );

  // Info Certificates
  QLabel* infocert = new QLabel ( trUtf8 ( "This Certificate is used by the remote end to verify the local user's identity against its list of Certification Authorities. For more Information about Client Authentication and Access Control  with Certificates please refer the  Apache SSL FAQ. How can I authenticate clients based on certificates when <a href=\"http://www.google.de/search?q=apache2+client+authentication+access+control%20site:httpd.apache.org\">I know all my clients</a>?" ), m_groupBox1 );
  infocert->setAlignment ( justifyAlign );
  infocert->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  infocert->setWordWrap ( true );
  infocert->setOpenExternalLinks ( true );
  box1Layout->addWidget ( infocert, 0, 0, 1, 3 );

  // Public Certificates
  QLabel* lbpubkey = new QLabel ( trUtf8 ( "Public Keyfile:" ), m_groupBox1 );
  lbpubkey->setAlignment ( floatRight );
  lbpubkey->setTextInteractionFlags ( Qt::NoTextInteraction );
  box1Layout->addWidget ( lbpubkey, 1, 0, 1, 1 );

  sslPublicKey = new QLineEdit ( m_groupBox1 );
  sslPublicKey->setObjectName ( QLatin1String ( "sslPublicKey" ) );
  box1Layout->addWidget ( sslPublicKey, 1, 1, 1, 1 );

  QToolButton* openPupKeyButton = new QToolButton ( m_groupBox1 );
  openPupKeyButton->setObjectName ( QLatin1String ( "openpupkeybutton" ) );
  openPupKeyButton->setText ( QLatin1String ( "..." ) );
  openPupKeyButton->setIcon ( openIcon );
  box1Layout->addWidget ( openPupKeyButton, 1, 2, 1, 1 );

  // Private Certificates
  QLabel* lbprivkey = new QLabel ( trUtf8 ( "Private Keyfile:" ), m_groupBox1 );
  lbprivkey->setAlignment ( floatRight );
  lbprivkey->setTextInteractionFlags ( Qt::NoTextInteraction );
  box1Layout->addWidget ( lbprivkey, 2, 0, 1, 1 );

  sslPrivateKey = new QLineEdit ( m_groupBox1 );
  sslPrivateKey->setObjectName ( QLatin1String ( "sslPrivateKey" ) );
  box1Layout->addWidget ( sslPrivateKey, 2, 1, 1, 1 );

  QToolButton* openPrivKeyButton = new QToolButton ( m_groupBox1 );
  openPrivKeyButton->setObjectName ( QLatin1String ( "openprivkeybutton" ) );
  openPrivKeyButton->setText ( QLatin1String ( "..." ) );
  openPrivKeyButton->setIcon ( openIcon );
  box1Layout->addWidget ( openPrivKeyButton, 2, 2, 1, 1 );

  // sslPassPhrase
  QLabel* passphraseinfo = new QLabel ( trUtf8 ( "Private Key Password:" ), m_groupBox1 );
  passphraseinfo->setAlignment ( floatRight );
  passphraseinfo->setTextInteractionFlags ( Qt::NoTextInteraction );
  box1Layout->addWidget ( passphraseinfo, 3, 0, 1, 1 );

  sslPassPhrase = new QLineEdit ( m_groupBox1 );
  sslPassPhrase->setObjectName ( QLatin1String ( "qt_ssl_pass_phrase" ) );
  sslPassPhrase->setEchoMode ( QLineEdit::Password );
  box1Layout->addWidget ( sslPassPhrase, 3, 1, 1, 3 );

  // add Box 1 Layout
  m_groupBox1->setLayout ( box1Layout );
  vLayout->addWidget ( m_groupBox1 );

  // CA Certificate Database
  QGroupBox* m_groupBox2 = new QGroupBox ( centeredWidget );
  m_groupBox2->setTitle ( trUtf8 ( "CA Certificate Database" ) );
  m_groupBox2->setCheckable ( false );
  m_groupBox2->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );

  QGridLayout* box2Layout = new QGridLayout ( m_groupBox2 );

  // Ca Certificates Path
  QLabel* labeldbpath = new QLabel ( trUtf8 ( "Database:" ), m_groupBox2 );
  labeldbpath->setAlignment ( floatRight );
  labeldbpath->setTextInteractionFlags ( Qt::NoTextInteraction );
  box2Layout->addWidget ( labeldbpath, 0, 0, 1, 1 );

  sslCaCertsDatabase = new QLineEdit ( m_groupBox2 );
  sslCaCertsDatabase->setObjectName ( QLatin1String ( "sslCaCertsPath" ) );
#ifdef Q_WS_X11
  sslCaCertsDatabase->setToolTip ( trUtf8 ( "Default: /etc/ssl/certs/ca-certificates.crt" ) );
#else
  sslCaCertsDatabase->setToolTip ( trUtf8 ( "Default: C:/ssl/ca-certificates.crt" ) );
#endif
  box2Layout->addWidget ( sslCaCertsDatabase, 0, 1, 1, 1 );

  QToolButton* openCaCertButton = new QToolButton ( m_groupBox2 );
  openCaCertButton->setObjectName ( QLatin1String ( "opencacertbutton" ) );
  openCaCertButton->setText ( QLatin1String ( "..." ) );
  openCaCertButton->setIcon ( openIcon );
  box2Layout->addWidget ( openCaCertButton, 0, 2, 1, 1 );

  sslIssuers = new QTableWidget ( 0, 3, m_groupBox2 );
  sslIssuers->setObjectName ( QLatin1String ( "sslIssuers" ) );
  sslIssuers->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( trUtf8 ( "Organization (O)" ) ) );
  sslIssuers->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( trUtf8 ( "CommonName (CN)" ) ) );
  sslIssuers->setHorizontalHeaderItem ( 2, new QTableWidgetItem ( trUtf8 ( "Expires" ) ) );
  sslIssuers->horizontalHeader()->setResizeMode ( QHeaderView::Stretch );
  box2Layout->addWidget ( sslIssuers, 1, 0, 1, 3 );

  // add Box 2 Layout
  m_groupBox2->setLayout ( box2Layout );
  vLayout->addWidget ( m_groupBox2 );

  trustedHostsList = new QListWidget ( this );

  vLayout->addWidget ( trustedHostsList );

  // set main Layout
  centeredWidget->setLayout ( vLayout );
  setWidget ( centeredWidget );

  connect ( openPrivKeyButton, SIGNAL ( clicked() ), this, SLOT ( getPrivKeyDialog() ) );
  connect ( openPupKeyButton, SIGNAL ( clicked() ), this, SLOT ( getPupKeyDialog() ) );
  connect ( openCaCertButton, SIGNAL ( clicked() ), this, SLOT ( getCaCertDatabaseDialog() ) );

  connect ( sslCaCertsDatabase, SIGNAL ( editingFinished () ),
            this, SLOT ( setCaCertDatabase() ) );

  connect ( sslCaCertsDatabase, SIGNAL ( textEdited ( const QString & ) ),
            this, SLOT ( setCaCertDatabase ( const QString & ) ) );

  connect ( sslPublicKey, SIGNAL ( editingFinished () ), this, SIGNAL ( modified() ) );
  connect ( sslPrivateKey, SIGNAL ( editingFinished () ), this, SIGNAL ( modified() ) );
  connect ( sslPassPhrase, SIGNAL ( editingFinished () ), this, SIGNAL ( modified() ) );
}

void SSLConfig::setCaCertDatabase ( const QString &p )
{
  QString path = ( p.isEmpty() ) ? sslCaCertsDatabase->text() : p;
  QFileInfo db ( path );
  if ( db.exists() )
  {
    QList<QSslCertificate> caCerts = ssl.caCertificates();
    caCerts << QSslCertificate::fromPath ( db.absoluteFilePath(), QSsl::Pem, QRegExp::FixedString );
    ssl.setCaCertificates ( caCerts );
    fillCaCertIssuerTable();
  }
}

void SSLConfig::fillCaCertIssuerTable()
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

    QTableWidgetItem* item2 = new QTableWidgetItem ( ref );
    item2->setText ( cert.expiryDate().toString ( Qt::DefaultLocaleLongDate ) );
    sslIssuers->setItem ( row, 2, item2 );
    row++;
  }
  sslIssuers->horizontalHeader()->setResizeMode ( QHeaderView::ResizeToContents );
}

void SSLConfig::getPrivKeyDialog()
{
  QString path ( sslPrivateKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    sslPrivateKey->setText ( db.absoluteFilePath() );
  else
    sslPrivateKey->clear();

  emit modified ();
}

void SSLConfig::getPupKeyDialog()
{
  QString path ( sslPublicKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    sslPublicKey->setText ( db.absoluteFilePath() );
  else
    sslPublicKey->clear();

  emit modified ();
}

void SSLConfig::getCaCertDatabaseDialog()
{
  QString path ( sslCaCertsDatabase->text() );
  QStringList filt;
  filt << trUtf8 ( "CA Bundle %1" ).arg ( "*.crt ca-*" );
  filt << trUtf8 ( "Certificate %1" ).arg ( "*.pem *.der" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open CA Database" ), path, filt.join ( ";;" ) );
  sslCaCertsDatabase->setText ( path );
  emit modified ();
}

void SSLConfig::load ( QSettings * cfg )
{
  if ( ! cfg )
    return;

  QString p ( QByteArray::fromBase64 ( cfg->value ( QLatin1String ( "sslPassPhrase" ) ).toByteArray() ) );
  sslPassPhrase->setText ( p );

  if ( ! sslCaCertsDatabase->text().isEmpty() )
    setCaCertDatabase ();

  QStringList list;
  int size = cfg->beginReadArray ( QLatin1String ( "TrustedCertsHosts" ) );
  for ( int i = 0; i < size; ++i )
  {
    cfg->setArrayIndex ( i );
    list.append ( cfg->value ( "host" ).toString() );
  }
  cfg->endArray();
  trustedHostsList->addItems ( list );
}

void SSLConfig::save ( QSettings * cfg )
{
  if ( ! cfg )
    return;

  QByteArray p = sslPassPhrase->text().toAscii();
  cfg->setValue ( QLatin1String ( "sslPassPhrase" ), p.toBase64() );

  cfg->beginWriteArray ( QLatin1String ( "TrustedCertsHosts" ) );
  for ( int i = 0; i < trustedHostsList->count(); ++i )
  {
    cfg->setArrayIndex ( i );
    cfg->setValue ( QLatin1String ( "host" ) , trustedHostsList->item ( i )->text() );
  }
  cfg->endArray();
}

SSLConfig::~SSLConfig()
{}
