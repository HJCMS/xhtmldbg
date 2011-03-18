/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "certissuers.h"
#include "certissuertable.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KLocalizedString>

CertIssuers::CertIssuers ( QWidget * parent )
    : QGroupBox ( i18n ( "Issuers" ), parent )
    , ssl ( QSslConfiguration::defaultConfiguration () )
{
  setObjectName ( QLatin1String ( "certissuers" ) );
  setFlat ( true );

  QIcon openFolderIcon = QIcon::fromTheme ( QLatin1String ( "document-open" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "certissuers_main_layout" ) );

  n_certIssuerTable = new CertIssuerTable ( this );
  verticalLayout->addWidget ( n_certIssuerTable );

  QHBoxLayout* horizontalLayout = new QHBoxLayout;
  horizontalLayout->setObjectName ( QLatin1String ( "certissuers_db_find_layout" ) );

  QLabel* label_1 = new QLabel ( this );
  label_1->setObjectName ( QLatin1String ( "label_1" ) );
  label_1->setText ( i18n ( "Database:" ) );
  label_1->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  horizontalLayout->addWidget ( label_1 );

  m_editCaDatabase = new QLineEdit ( this );
  m_editCaDatabase->setObjectName ( QLatin1String ( "certissuers_edit_ca_db" ) );
  m_editCaDatabase->setToolTip ( i18n ( "CA Bundle Database" ) );
  horizontalLayout->addWidget ( m_editCaDatabase );

  QToolButton* openCaButton = new QToolButton ( this );
  openCaButton->setObjectName ( QLatin1String ( "certissuers_open_cafile_button" ) );
  openCaButton->setIcon ( openFolderIcon );
  horizontalLayout->addWidget ( openCaButton );

  verticalLayout->addLayout ( horizontalLayout );

  setLayout ( verticalLayout );

  connect ( m_editCaDatabase, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( setCaCertDatabase ( const QString & ) ) );

  connect ( openCaButton, SIGNAL ( clicked() ),
            this, SLOT ( getCaCertDatabaseDialog() ) );
}

/**
* Lese die CA Bündel Datei aus und Schreibe alle
* Issuer in die Liste @ref caCerts danach wird die
* Methode @ref setCaCertIssuerTable neu aufgerufen.
*/
void CertIssuers::setCaCertDatabase ( const QString &p )
{
  if ( ! m_editCaDatabase->text().isEmpty() )
    emit modified ( true );

  QFileInfo db ( p );
  if ( db.exists() )
  {
    blockSignals ( true );
    m_editCaDatabase->setText ( db.absoluteFilePath() );
    QList<QSslCertificate> caCerts = ssl.caCertificates();
    caCerts << QSslCertificate::fromPath ( db.absoluteFilePath(), QSsl::Pem, QRegExp::FixedString );
    ssl.setCaCertificates ( caCerts );
    setCaCertIssuerTable();
    blockSignals ( false );
  }
}

/**
* Lese mit QSslCertificate::caCertificates() alle Zertifikate
* in ein QList und durchlaufe sie in einer foreach Schleife.
* Bevor die Issuer in die Tabelle eingtragen werden prüfe ob
* das Zertifikat nicht schon abgelaufen ist!
* Wenn ja setze einen gelben Hinweis Hintergrund!
*/
void CertIssuers::setCaCertIssuerTable()
{
  QList<QSslCertificate> certs = ssl.caCertificates();
  if ( certs.size() < 1 )
    return;

  QTableWidgetItem ref ( QTableWidgetItem::UserType );
  ref.setFlags ( ( Qt::ItemIsSelectable | Qt::ItemIsEnabled ) );

  n_certIssuerTable->clearContents();
  n_certIssuerTable->setRowCount ( certs.size() );
  int row = 0;
  foreach ( QSslCertificate cert, certs )
  {
    QTableWidgetItem* item0 = new QTableWidgetItem ( ref );
    item0->setText ( cert.issuerInfo ( QSslCertificate::Organization ) );
    n_certIssuerTable->setItem ( row, 0, item0 );

    QTableWidgetItem* item1 = new QTableWidgetItem ( ref );
    item1->setText ( cert.issuerInfo ( QSslCertificate::CommonName ) );
    n_certIssuerTable->setItem ( row, 1, item1 );

    QDateTime stamp = cert.expiryDate();
    QTableWidgetItem* item2 = new QTableWidgetItem ( ref );
    item2->setText ( stamp.toString ( Qt::DefaultLocaleLongDate ) );
    if ( stamp < QDateTime::currentDateTime () )
      item2->setBackground ( Qt::yellow );
    n_certIssuerTable->setItem ( row, 2, item2 );
    row++;
  }
  n_certIssuerTable->horizontalHeader()->setResizeMode ( QHeaderView::ResizeToContents );
}

/**
* Dialog zum finden der CA Bündel Datei.
*/
void CertIssuers::getCaCertDatabaseDialog()
{
  QString path ( m_editCaDatabase->text() );
  QStringList filt;
  filt << i18nc ( "CA Bundle %1", "*.crt ca-*" );
  filt << i18nc ( "Certificate %1", "*.pem *.der" );

  path = QFileDialog::getOpenFileName ( this, i18n ( "CA Bundle Database" ), path, filt.join ( ";;" ) );
  setCaCertDatabase ( path );
}

const QString CertIssuers::getCaBundleFile ()
{
  return m_editCaDatabase->text();
}

CertIssuers::~CertIssuers()
{}
