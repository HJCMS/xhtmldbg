/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "certdialog.h"
#include "networksettings.h"

/* QtCore */
#include <QtCore/QDateTime>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

CertDialog::CertDialog ( NetworkSettings * settings, QWidget * parent )
    : QDialog ( parent )
    , m_networkSettings ( settings )
    , certHost ( QLatin1String ( "https://localhost:443" ) )
{
  setObjectName ( QLatin1String ( "certdialog" ) );
  setWindowTitle ( i18n ( "Certification Notice" ) );
  setMinimumWidth ( 600 );
  setMinimumHeight ( 400 );
  setSizeGripEnabled ( true );
  setContentsMargins ( 2, 2, 2, 9 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  QTabWidget* tabWidget = new QTabWidget ( this );
  tabWidget->setObjectName ( QLatin1String ( "cert_tab_widget" ) );

  QWidget* infoWidget = new QWidget ( tabWidget );
  infoWidget->setObjectName ( QLatin1String ( "cert_info_widget" ) );
  tabWidget->addTab ( infoWidget, i18n ( "Notice" ) );

  QVBoxLayout* t1Layout = new QVBoxLayout ( infoWidget );
  notify = new QLabel ( i18n ( "Message" ), infoWidget );
  notify->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  notify->setIndent ( 2 );
  notify->setWordWrap ( true );
  t1Layout->addWidget ( notify );
  infoWidget->setLayout ( t1Layout );

  treeWidget = new QTreeWidget ( tabWidget );
  treeWidget->setObjectName ( QLatin1String ( "cert_tree_widget" ) );
  treeWidget->setWordWrap ( false );
  treeWidget->setColumnCount ( 2 );
  QStringList treeHeaders;
  treeHeaders << i18n( "Level" ) << i18n( "Values" );
  treeWidget->setHeaderLabels ( treeHeaders );

  subjectInfo = new QTreeWidgetItem ( treeWidget->invisibleRootItem() );
  subjectInfo->setText ( 0, i18n ( "Subject" ) );
  subjectInfo->setExpanded ( true );
  treeWidget->addTopLevelItem ( subjectInfo );

  issuerInfo = new QTreeWidgetItem ( treeWidget->invisibleRootItem() );
  issuerInfo->setText ( 0, i18n ( "Issuer" ) );
  issuerInfo->setExpanded ( true );
  treeWidget->addTopLevelItem ( issuerInfo );

  /**
  * Um das ganze etwas Übersichtlicher zu halten. Verwende ich hier
  * einen Vector weil beim befüllen der zweiten Spalte eine Referenz
  * auf das Datenfeld der 1. Spalte erfolgen muss! Erscheint es mir
  * von der Performance aus gesehen hier als sinnvoll.
  * Und weil die Einträge so mehrfach eingelesen werden können.
  **/
  RowItem item1;
  item1.text = i18n ( "Organization (O)" );
  item1.tip = i18n ( "The name of the organization. (O)" );
  item1.info = QSslCertificate::Organization;
  RowItems.append ( item1 );

  RowItem item2;
  item2.text = i18n ( "Common Name (CN)" );
  item2.tip = i18n ( "The common name; most often this is used to store the host name." );
  item2.info = QSslCertificate::CommonName;
  RowItems.append ( item2 );

  RowItem item3;
  item3.text = i18n ( "Locality (L)" );
  item3.tip = i18n ( "The locality. (L)" );
  item3.info = QSslCertificate::LocalityName;
  RowItems.append ( item3 );

  RowItem item4;
  item4.text = i18n ( "Organizational UnitName (OU)" );
  item4.tip = i18n ( "The organizational unit name. (OU)" );
  item4.info = QSslCertificate::OrganizationalUnitName;
  RowItems.append ( item4 );

  RowItem item5;
  item5.text = i18n ( "CountryName (C)" );
  item5.tip = i18n ( "The country. (C)" );
  item5.info = QSslCertificate::CountryName;
  RowItems.append ( item5 );

  RowItem item6;
  item6.text = i18n ( "State or Province (ST)" );
  item6.tip = i18n ( "The state or province name. (ST)" );
  item6.info = QSslCertificate::StateOrProvinceName;
  RowItems.append ( item6 );

  foreach ( const RowItem &r, RowItems )
  {
    QTreeWidgetItem* sub = new QTreeWidgetItem ( subjectInfo );
    sub->setForeground ( 0, Qt::blue );
    sub->setText ( 0, r.text );
    sub->setToolTip ( 0, r.tip );
    subjectInfo->addChild ( sub );

    QTreeWidgetItem* iss = new QTreeWidgetItem ( issuerInfo );
    iss->setForeground ( 0, Qt::blue );
    iss->setText ( 0, r.text );
    iss->setToolTip ( 0, r.tip );
    issuerInfo->addChild ( iss );
  }

  tabWidget->addTab ( treeWidget, i18n ( "Certificate" ) );

  tabWidget->setCurrentIndex ( 0 );
  vLayout->addWidget ( tabWidget );

  QDialogButtonBox::StandardButtons buttons = ( QDialogButtonBox::Ignore | QDialogButtonBox::Abort );
  QDialogButtonBox* box = new QDialogButtonBox ( buttons, Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "buttonBox" ) );
  QPushButton* btnsave = box->addButton ( i18n ( "Import" ), QDialogButtonBox::ActionRole );

  vLayout->addWidget ( box );

  setLayout ( vLayout );

  connect ( btnsave, SIGNAL ( clicked () ), this, SLOT ( import() ) );
  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

/**
* Lese/Schreibe aus der Konfiguration die Weiße liste für voraussichtlich
* nicht vertrauenswürdige Zertifikate aus.
* Danach wird der Dialog beendet!
*/
void CertDialog::import()
{
  if ( certHost.isEmpty() )
    return;

  int size = m_networkSettings->beginReadArray ( QLatin1String ( "TrustedCertsHosts" ) );
  m_networkSettings->endArray();

  int index = ( size < 0 ) ? 0 : size;
  m_networkSettings->beginWriteArray ( QLatin1String ( "TrustedCertsHosts" ) );
  m_networkSettings->setArrayIndex ( index );
  m_networkSettings->setValue ( QLatin1String ( "host" ) , certHost );
  m_networkSettings->endArray();
  accept();
}

/**
* Füge die Zertifikate in die liste ein!
* Nehme hierfür die Schlüssel aus dem Vector.
*/
void CertDialog::setCertificate ( const QSslCertificate &pem, const QString &host )
{
  certHost = host;

  int index = 0;
  foreach ( const RowItem &r, RowItems )
  {
    subjectInfo->child ( index )->setText ( 1, pem.subjectInfo ( r.info ) );
    issuerInfo->child ( index )->setText ( 1, pem.issuerInfo ( r.info ) );
    index++;
  }

  if ( ! pem.serialNumber().isNull() )
  {
    QTreeWidgetItem* itemSerialNumber = new QTreeWidgetItem ( subjectInfo );
    itemSerialNumber->setForeground ( 0, Qt::blue );
    itemSerialNumber->setText ( 0, i18n ( "Serial Nr." ) );
    itemSerialNumber->setText ( 1, pem.serialNumber() );
    subjectInfo->addChild ( itemSerialNumber );
  }

  QTreeWidgetItem* itemExpiryDate = new QTreeWidgetItem ( subjectInfo );
  itemExpiryDate->setForeground ( 0, Qt::blue );
  itemExpiryDate->setText ( 0, i18n ( "Expires" ) );
  itemExpiryDate->setText ( 1, pem.expiryDate().toString ( Qt::DefaultLocaleLongDate ) );
  subjectInfo->addChild ( itemExpiryDate );

  treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
}

/**
* Hier werden die vom Netzwerk Manager generierten Nachrichten eingefügt.
*/
void CertDialog::setMessages ( const QStringList &mess )
{
  notify->setText ( mess.join ( "\n" ) );
}

CertDialog::~CertDialog()
{}
