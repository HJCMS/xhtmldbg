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

#include "xpasswordwidget.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextDocument>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

/* QtScript */
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

XPasswordWidget::XPasswordWidget ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "xPasswordWidget" ) );
  setMinimumWidth ( 250 );
  setMinimumHeight ( 200 );
  setContentsMargins ( 5, 5, 5, 5 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  m_groupBox = new QGroupBox ( i18n ( "Password Request" ), this );
  vLayout->addWidget ( m_groupBox );

  QGridLayout* layout = new QGridLayout ( m_groupBox );
  layout->setMargin ( 5 );
  m_groupBox->setLayout ( layout );

  QLabel* lb1 = new QLabel ( m_groupBox );
  lb1->setText ( i18n ( "User" ) );
  lb1->setAlignment ( Qt::AlignRight );
  lb1->setIndent ( 2 );
  layout->addWidget ( lb1, 0, 0, 1, 1 );

  m_onwerLineEdit = new QLineEdit ( m_groupBox );
  m_onwerLineEdit->setEchoMode ( QLineEdit::Normal );
  layout->addWidget ( m_onwerLineEdit, 0, 1, 1, 1 );

  QToolButton* btn1 = new QToolButton ( m_groupBox );
  btn1->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  btn1->setToolTip ( i18n ( "Clear" ) );
  layout->addWidget ( btn1, 0, 2, 1, 1 );

  QLabel* lb2 = new QLabel ( m_groupBox );
  lb2->setText ( i18n ( "Password" ) );
  lb2->setAlignment ( Qt::AlignRight );
  lb2->setIndent ( 2 );
  layout->addWidget ( lb2, 1, 0, 1, 1 );

  m_passLineEdit = new QLineEdit ( m_groupBox );
  m_passLineEdit->setEchoMode ( QLineEdit::PasswordEchoOnEdit );
  layout->addWidget ( m_passLineEdit, 1, 1, 1, 1 );

  QToolButton* btn2 = new QToolButton ( m_groupBox );
  btn2->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  btn2->setToolTip ( i18n ( "Clear" ) );
  layout->addWidget ( btn2, 1, 2, 1, 1 );

  QDialogButtonBox* btnBox = new QDialogButtonBox ( Qt::Horizontal, m_groupBox );
  QPushButton* m_resetbutton = btnBox->addButton ( QDialogButtonBox::Reset );
  m_resetbutton->setObjectName ( QLatin1String ( "resetbutton" ) );

  layout->addWidget ( btnBox, 2, 0, 1, 3, Qt::AlignRight );

  QSpacerItem* vsp = new QSpacerItem ( 20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding );
  vLayout->addItem ( vsp );

  setLayout ( vLayout );

  connect ( m_onwerLineEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( textModified ( const QString & ) ) );

  connect ( m_passLineEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( textModified ( const QString & ) ) );

  connect ( m_resetbutton, SIGNAL ( clicked() ), this, SLOT ( restore() ) );
  connect ( btn1, SIGNAL ( clicked() ), m_onwerLineEdit, SLOT ( clear() ) );
  connect ( btn2, SIGNAL ( clicked() ), m_passLineEdit, SLOT ( clear() ) );
}

bool XPasswordWidget::status()
{
  if ( m_onwerLineEdit->text().isEmpty() )
  {
    m_onwerLineEdit->setFocus();
    return false;
  }
  else if ( m_passLineEdit->text().isEmpty() )
  {
    m_passLineEdit->setFocus();
    return false;
  }
  return true;
}

void XPasswordWidget::textModified ( const QString &text )
{
  Q_UNUSED ( text )
}

void XPasswordWidget::restore()
{
  m_passLineEdit->clear();
  m_onwerLineEdit->clear();
  m_onwerLineEdit->setFocus();
}

void XPasswordWidget::setUser ( const QString &o )
{
  m_onwerLineEdit->setText ( o );
}

void XPasswordWidget::setTitle ( const QString &t )
{
  m_groupBox->setTitle ( t );
}

QString XPasswordWidget::title() const
{
  return m_groupBox->title();
}

QString XPasswordWidget::user() const
{
  return m_onwerLineEdit->text();
}

void XPasswordWidget::setPassword ( const QString &p )
{
  m_passLineEdit->setText ( p );
}

QString XPasswordWidget::password() const
{
  return m_passLineEdit->text();
}

XPasswordWidget::~XPasswordWidget()
{}
