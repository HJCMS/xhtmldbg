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

#include "configaccesscontrol.h"

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

ConfigAccessControl::ConfigAccessControl ( QWidget * parent )
    : QGroupBox ( trUtf8 ( "Client Authentication and Access Control" ), parent )
{
  setObjectName ( QLatin1String ( "configaccesscontrol" ) );
  setFlat ( true );
  setContentsMargins ( 0, 5, 0, 5 );
  setMinimumHeight ( 200 );

  QSizePolicy sizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  sizePolicy.setHorizontalStretch ( 0 );
  sizePolicy.setVerticalStretch ( 0 );
  setSizePolicy ( sizePolicy );

  QIcon openFolderIcon = QIcon::fromTheme ( QLatin1String ( "document-open" ) );
  Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setObjectName ( QLatin1String ( "configaccesscontrol_main_layout" ) );
  gridLayout->setContentsMargins ( 0, 5, 0, 5 );
  gridLayout->setSpacing ( 5 );

  QLabel* lr0 = new  QLabel ( this );
  lr0->setObjectName ( QLatin1String ( "configaccesscontrol_info_label" ) );
  lr0->setAlignment ( ( Qt::AlignJustify | Qt::AlignTop ) );
  lr0->setWordWrap ( true );
  lr0->setIndent ( 2 );
  lr0->setOpenExternalLinks ( true );
  lr0->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  lr0->setText ( trUtf8 ( "This Certificate is used by the remote end to verify the local user's identity against its list of Certification Authorities. For more Information about Client Authentication and Access Control  with Certificates please refer the  Apache SSL FAQ. How can I authenticate clients based on certificates when <a href=\"%1\">I know all my clients</a>?" )
                 .arg ( "http://www.google.de/search?q=apache2+client+authentication+access+control%20site:httpd.apache.org" ) );
  gridLayout->addWidget ( lr0, 0, 0, 1, 3 );

  QLabel* lr1 = new QLabel ( this );
  lr1->setText ( trUtf8 ( "Public Keyfile:" ) );
  lr1->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr1, 1, 0, 1, 1 );

  sslPublicKey = new QLineEdit ( this );
  sslPublicKey->setObjectName ( QLatin1String ( "sslPublicKey" ) );
  gridLayout->addWidget ( sslPublicKey, 1, 1, 1, 1 );

  QToolButton* btn1 = new QToolButton ( this );
  btn1->setObjectName ( QLatin1String ( "openPupKeyButton" ) );
  btn1->setIcon ( openFolderIcon );
  gridLayout->addWidget ( btn1, 1, 2, 1, 1 );

  QLabel* lr2 = new QLabel ( this );
  lr2->setText ( trUtf8 ( "Private Keyfile:" ) );
  lr2->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr2, 2, 0, 1, 1 );

  sslPrivateKey = new QLineEdit ( this );
  sslPrivateKey->setObjectName ( QLatin1String ( "sslPrivateKey" ) );
  gridLayout->addWidget ( sslPrivateKey, 2, 1, 1, 1 );

  QToolButton* btn2 = new QToolButton ( this );
  btn2->setObjectName ( QLatin1String ( "openPrivKeyButton" ) );
  btn2->setIcon ( openFolderIcon );
  gridLayout->addWidget ( btn2, 2, 2, 1, 1 );

  QLabel* lr3 = new QLabel ( this );
  lr3->setText ( trUtf8 ( "Private Key Password:" ) );
  lr3->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr3, 3, 0, 1, 1 );

  sslPrivatePass = new QLineEdit ( this );
  sslPrivatePass->setObjectName ( QLatin1String ( "qt_ssl_pass_phrase" ) );
  sslPrivatePass->setEchoMode ( QLineEdit::PasswordEchoOnEdit );
  gridLayout->addWidget ( sslPrivatePass, 3, 1, 1, 1 );

  QToolButton* btn3 = new QToolButton ( this );
  btn3->setObjectName ( QLatin1String ( "clearPassButton" ) );
  btn3->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  gridLayout->addWidget ( btn3, 3, 2, 1, 1 );

  setLayout ( gridLayout );

  connect ( sslPublicKey, SIGNAL ( textChanged ( const QString &) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( sslPrivateKey, SIGNAL ( textChanged ( const QString &) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( sslPrivatePass, SIGNAL ( textChanged ( const QString &) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( btn1, SIGNAL ( clicked() ), this, SLOT ( getPupKeyDialog() ) );
  connect ( btn2, SIGNAL ( clicked() ), this, SLOT ( getPrivKeyDialog() ) );
  connect ( btn3, SIGNAL ( clicked() ), sslPrivatePass, SLOT ( clear() ) );
}

/**
* Dialog für die Suche nach dem Privaten Schlüssel.
*/
void ConfigAccessControl::getPrivKeyDialog()
{
  QString path ( sslPrivateKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    sslPrivateKey->setText ( db.absoluteFilePath() );
}

/**
* Dialog für die Suche nach dem Öffentlichen Schlüssel
*/
void ConfigAccessControl::getPupKeyDialog()
{
  QString path ( sslPublicKey->text() );
  QStringList filt;
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open Certificate" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    sslPublicKey->setText ( db.absoluteFilePath() );
}

void ConfigAccessControl::dataChanged ( const QString & )
{
  emit modified ( true );
}

void ConfigAccessControl::setPublicKeyPath ( const QString &txt )
{
  if ( ! txt.isEmpty() )
    sslPublicKey->setText ( txt );
}

void ConfigAccessControl::setPrivateKeyPath ( const QString &txt )
{
  if ( ! txt.isEmpty() )
    sslPrivateKey->setText ( txt );
}

void ConfigAccessControl::setPassPhrase ( const QByteArray &txt )
{
  QString p ( QByteArray::fromBase64 ( txt ) );
  if ( ! p.isEmpty() )
    sslPrivatePass->setText ( p );
}

const QString ConfigAccessControl::getPupKey()
{
  return sslPublicKey->text();
}

const QString ConfigAccessControl::getPrivKey()
{
  return sslPrivateKey->text();
}

const QByteArray ConfigAccessControl::getPassPhrase()
{
  QByteArray p = sslPrivatePass->text().toAscii();
  return p.toBase64();
}

ConfigAccessControl::~ConfigAccessControl()
{}
