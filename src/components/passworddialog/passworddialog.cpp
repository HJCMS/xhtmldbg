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

#include "passworddialog.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

/* QtScript */
#include <QtScript/QScriptEngine>

PasswordDialog::PasswordDialog ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "xPasswordDialog" ) );
  setMinimumWidth ( 250 );
  setMinimumHeight ( 200 );
  setContentsMargins ( 5, 5, 5, 5 );

  QScriptEngine p_scriptEngine ( this );
  p_scriptEngine.setObjectName ( QLatin1String ( "PasswordDialogScriptEngine" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  QGroupBox* box = new QGroupBox ( trUtf8 ( "Password Request" ), this );
  vLayout->addWidget ( box );

  QGridLayout* layout = new QGridLayout ( box );
  layout->setMargin ( 5 );
  box->setLayout ( layout );

  QLabel* lb1 = new QLabel ( box );
  lb1->setText ( trUtf8 ( "User" ) );
  lb1->setAlignment ( Qt::AlignRight );
  lb1->setIndent ( 2 );
  layout->addWidget ( lb1, 0, 0, 1, 1 );

  m_onwerLineEdit = new QLineEdit ( box );
  m_onwerLineEdit->setEchoMode ( QLineEdit::Normal );
  layout->addWidget ( m_onwerLineEdit, 0, 1, 1, 1 );

  scOwnerField = p_scriptEngine.newQObject ( m_onwerLineEdit );
  p_scriptEngine.globalObject().setProperty ( "userfield", scOwnerField );

  QToolButton* btn1 = new QToolButton ( box );
  btn1->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  btn1->setToolTip ( trUtf8 ( "Clear" ) );
  layout->addWidget ( btn1, 0, 2, 1, 1 );

  QLabel* lb2 = new QLabel ( box );
  lb2->setText ( trUtf8 ( "Password" ) );
  lb2->setAlignment ( Qt::AlignRight );
  lb2->setIndent ( 2 );
  layout->addWidget ( lb2, 1, 0, 1, 1 );

  m_passLineEdit = new QLineEdit ( box );
  m_passLineEdit->setEchoMode ( QLineEdit::PasswordEchoOnEdit );
  layout->addWidget ( m_passLineEdit, 1, 1, 1, 1 );

  scPasswordField = p_scriptEngine.newQObject ( m_passLineEdit );
  p_scriptEngine.globalObject().setProperty ( "passwordfield", scPasswordField );

  QToolButton* btn2 = new QToolButton ( box );
  btn2->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  btn2->setToolTip ( trUtf8 ( "Clear" ) );
  layout->addWidget ( btn2, 1, 2, 1, 1 );

  QDialogButtonBox* btnBox = new QDialogButtonBox ( Qt::Horizontal, box );
  QPushButton* submit = btnBox->addButton ( QDialogButtonBox::Apply );
  QPushButton* reset = btnBox->addButton ( QDialogButtonBox::Reset );
  layout->addWidget ( btnBox, 2, 0, 1, 3, Qt::AlignRight );

  QSpacerItem* vsp = new QSpacerItem ( 20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding );
  vLayout->addItem ( vsp );

  setLayout ( vLayout );

  connect ( submit, SIGNAL ( clicked() ), this, SLOT ( submit() ) );
  connect ( reset, SIGNAL ( clicked() ), this, SLOT ( restore() ) );
  connect ( btn1, SIGNAL ( clicked() ), m_onwerLineEdit, SLOT ( clear() ) );
  connect ( btn2, SIGNAL ( clicked() ), m_passLineEdit, SLOT ( clear() ) );

  setTabOrder ( m_onwerLineEdit, btn1 );
  setTabOrder ( btn1, m_passLineEdit );
  setTabOrder ( m_passLineEdit, btn2 );
  setTabOrder ( btn2, submit );
  setTabOrder ( submit, reset );
}

void PasswordDialog::submit()
{
  if ( m_onwerLineEdit->text().isEmpty() )
  {
    m_onwerLineEdit->setFocus();
    return;
  }
  else if ( m_passLineEdit->text().isEmpty() )
  {
    m_passLineEdit->setFocus();
    return;
  }
  emit accept ( m_onwerLineEdit->text(), m_passLineEdit->text() );
}

void PasswordDialog::restore()
{
  m_passLineEdit->clear();
  m_onwerLineEdit->clear();
  m_onwerLineEdit->setFocus();
}

void PasswordDialog::setUser ( const QString &o )
{
  m_onwerLineEdit->setText( o );
}

QString PasswordDialog::user() const
{
  return m_onwerLineEdit->text();
}

void PasswordDialog::setPassword ( const QString &p )
{
  m_passLineEdit->setText( p );
}

QString PasswordDialog::password() const
{
  return m_passLineEdit->text();
}

PasswordDialog::~PasswordDialog()
{}
