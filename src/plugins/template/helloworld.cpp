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

#include "helloworld.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

HelloWorld::HelloWorld ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "HelloWorldPlugin" ) );
  setWindowTitle ( QLatin1String ( "Template HelloWorld Plugin" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );

  m_labelUrl = new QLabel ( this );
  verticalLayout->addWidget ( m_labelUrl );

  m_Content = new QTextEdit ( this );
  verticalLayout->addWidget ( m_Content );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setStandardButtons ( QDialogButtonBox::Ok );
  box->setCenterButtons ( true );
  verticalLayout->addWidget ( box );

  setLayout ( verticalLayout );

  connect ( box, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
}

void HelloWorld::setLabelUrl ( const QString &text )
{
  m_labelUrl->setText ( text );
}

void HelloWorld::setLabelContent ( const QString &text )
{
  m_Content->setPlainText ( text );
}

HelloWorld::~HelloWorld()
{}
