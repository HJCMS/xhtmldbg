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

#include "authenticationdialog.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>

AuthenticationDialog::AuthenticationDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "authenticationdialog" ) );
  setWindowTitle ( trUtf8 ( "Authentication Request" ) );
  setMinimumWidth ( 350 );
  setSizeGripEnabled ( true );

  Qt::Alignment labelAligment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QGridLayout* gLayout = new QGridLayout ( this );

  // Realm
  txtRealm = new QLabel ( trUtf8 ( "Authentication" ), this );
  txtRealm->setIndent ( 2 );
  txtRealm->setWordWrap ( true );
  gLayout->addWidget ( txtRealm, 0, 0, 1, 0 );

  // Login Name
  QLabel* txtOwner = new QLabel ( trUtf8 ( "Login Username:" ), this );
  txtOwner->setAlignment ( labelAligment );
  txtOwner->setIndent ( 2 );
  txtOwner->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtOwner->setWordWrap ( false );
  gLayout->addWidget ( txtOwner, 1, 0 );

  username = new QLineEdit ( this );
  username->setObjectName ( QLatin1String ( "username" ) );
  gLayout->addWidget ( username, 1, 1 );

  // Login Password
  QLabel* txtPass = new QLabel ( trUtf8 ( "Login Password:" ), this );
  txtPass->setAlignment ( labelAligment );
  txtPass->setIndent ( 2 );
  txtPass->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtPass->setWordWrap ( false );
  gLayout->addWidget ( txtPass, 2, 0 );

  password = new QLineEdit ( this );
  password->setObjectName ( QLatin1String ( "password" ) );
  password->setEchoMode ( QLineEdit::Password );
  gLayout->addWidget ( password, 2, 1 );

  QDialogButtonBox::StandardButtons buttons = ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
  QDialogButtonBox* box = new QDialogButtonBox ( buttons, Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "buttonBox" ) );
  gLayout->addWidget ( box, 3, 0, 1, 0 );

  setLayout ( gLayout );

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

void AuthenticationDialog::setRealm ( const QString &realm, const QString &host )
{
  QString str;
  if ( host.isEmpty() )
    str = QString ( "%1" ).arg ( realm );
  else
    str = QString ( "%1 (%2)" ).arg ( realm, host );

  txtRealm->setText ( str );
}

void AuthenticationDialog::setLogin ( const QString &owner )
{
  if ( owner.isEmpty() )
    return;

  username->setText ( owner );
}

void AuthenticationDialog::setPass ( const QString &pass )
{
  if ( pass.isEmpty() )
    return;

  password->setText ( pass );
}

const QString AuthenticationDialog::login()
{
  return username->text();
}

const QString AuthenticationDialog::pass()
{
  return password->text();
}

AuthenticationDialog::~AuthenticationDialog()
{
}
