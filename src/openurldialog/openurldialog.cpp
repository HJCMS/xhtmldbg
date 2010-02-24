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

#include "openurldialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLayout>

OpenUrlDialog::OpenUrlDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "openurldialog" ) );
  setWindowTitle ( trUtf8 ( "Open Url" ) );
  setMinimumWidth ( 450 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  m_lineEdit = new QLineEdit ( QLatin1String ( "http://" ), this );
  vLayout->addWidget ( m_lineEdit );

  QDialogButtonBox::StandardButtons buttons = ( QDialogButtonBox::Open | QDialogButtonBox::Cancel );
  QDialogButtonBox* box = new QDialogButtonBox ( buttons, Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "buttonBox" ) );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( checkInput() ) );
  connect ( box, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

void OpenUrlDialog::checkInput()
{
  QUrl url;
  url.setUrl ( m_lineEdit->text(), QUrl::StrictMode );
  if ( url.isValid() && url.scheme() == "http" && !url.encodedHost().isEmpty() )
  {
    emit openUrl ( url );
    accept();
  }
}

OpenUrlDialog::~OpenUrlDialog()
{}
