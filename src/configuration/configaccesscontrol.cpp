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

#include "configaccesscontrol.h"
#include "configutils.h"

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

/* KDE */
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <KDE/KIcon>
#include <KDE/KUrl>
#include <KDE/KFileDialog>

ConfigAccessControl::ConfigAccessControl ( QWidget * parent )
    : QGroupBox ( i18n ( "Client Authentication and Access Control" ), parent )
{
  setObjectName ( QLatin1String ( "configaccesscontrol" ) );

  Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setObjectName ( QLatin1String ( "configaccesscontrol_main_layout" ) );

  QLabel* lr0 = new  QLabel ( this );
  lr0->setObjectName ( QLatin1String ( "configaccesscontrol_info_label" ) );
  lr0->setAlignment ( ( Qt::AlignJustify | Qt::AlignTop ) );
  lr0->setWordWrap ( true );
  lr0->setIndent ( 2 );
  lr0->setOpenExternalLinks ( true );
  lr0->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  // Certification Authorities Info
  lr0->setText ( i18n ( "This Certificate is used by the remote end to verify the local user's identity against its list of Certification Authorities. For more Information about Client Authentication and Access Control  with Certificates please refer the  Apache SSL FAQ. How can I authenticate clients based on certificates when <a href=\"%1\">I know all my clients</a>?" )
                 .arg ( "http://www.google.de/search?q=apache2+client+authentication+access+control%20site:httpd.apache.org" ) );
  gridLayout->addWidget ( lr0, 0, 0, 1, 3 );

  QLabel* lr1 = new QLabel ( this );
  lr1->setText ( i18n ( "Public Keyfile:" ) );
  lr1->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr1, 1, 0, 1, 1 );

  sslPublicKey = new QLineEdit ( this );
  sslPublicKey->setObjectName ( QLatin1String ( "sslPublicKey" ) );
  gridLayout->addWidget ( sslPublicKey, 1, 1, 1, 1 );

  QToolButton* btn1 = new QToolButton ( this );
  btn1->setObjectName ( QLatin1String ( "openPupKeyButton" ) );
  btn1->setIcon ( ConfigUtils::folderIcon() );
  gridLayout->addWidget ( btn1, 1, 2, 1, 1 );

  QLabel* lr2 = new QLabel ( this );
  lr2->setText ( i18n ( "Private Keyfile:" ) );
  lr2->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr2, 2, 0, 1, 1 );

  sslPrivateKey = new QLineEdit ( this );
  sslPrivateKey->setObjectName ( QLatin1String ( "sslPrivateKey" ) );
  gridLayout->addWidget ( sslPrivateKey, 2, 1, 1, 1 );

  QToolButton* btn2 = new QToolButton ( this );
  btn2->setObjectName ( QLatin1String ( "openPrivKeyButton" ) );
  btn2->setIcon ( ConfigUtils::folderIcon() );
  gridLayout->addWidget ( btn2, 2, 2, 1, 1 );

  QLabel* lr3 = new QLabel ( this );
  lr3->setText ( i18n ( "Private Key Password:" ) );
  lr3->setAlignment ( labelAlign );
  gridLayout->addWidget ( lr3, 3, 0, 1, 1 );

  sslPrivatePass = new QLineEdit ( this );
  sslPrivatePass->setObjectName ( QLatin1String ( "qt_ssl_pass_phrase" ) );
  sslPrivatePass->setEchoMode ( QLineEdit::PasswordEchoOnEdit );
  gridLayout->addWidget ( sslPrivatePass, 3, 1, 1, 1 );

  QToolButton* btn3 = new QToolButton ( this );
  btn3->setObjectName ( QLatin1String ( "clearPassButton" ) );
  btn3->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  gridLayout->addWidget ( btn3, 3, 2, 1, 1 );

  setLayout ( gridLayout );

  connect ( sslPublicKey, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( sslPrivateKey, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( sslPrivatePass, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( dataChanged ( const QString & ) ) );
  connect ( btn1, SIGNAL ( clicked() ), this, SLOT ( getPupKeyDialog() ) );
  connect ( btn2, SIGNAL ( clicked() ), this, SLOT ( getPrivKeyDialog() ) );
  connect ( btn3, SIGNAL ( clicked() ), sslPrivatePass, SLOT ( clear() ) );
}

/** Dialog für die Suche nach dem Privaten Schlüssel. */
void ConfigAccessControl::getPrivKeyDialog()
{
  QString path ( ( sslPrivateKey->text().isEmpty() ? "kfiledialog:///" : sslPrivateKey->text() ) );
  path = KFileDialog::getOpenFileName ( KUrl ( path ), QString ( "application/x-pkcs12" ),
                                        this, i18n ( "Open Private Certificate" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    sslPrivateKey->setText ( db.absoluteFilePath() );
}

/** Dialog für die Suche nach dem Öffentlichen Schlüssel */
void ConfigAccessControl::getPupKeyDialog()
{
  QString path ( ( sslPublicKey->text().isEmpty() ? "kfiledialog:///" : sslPublicKey->text() ) );
  path = KFileDialog::getOpenFileName ( KUrl ( path ), QString ( "application/x-x509-ca-cert" ),
                                        this, i18n ( "Open Public Certificate" ) );

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
