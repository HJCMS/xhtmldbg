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

#include "errorsdialog.h"

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

ErrorsDialog::ErrorsDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "errorsdialog" ) );
  setWindowTitle ( trUtf8 ( "Error" ) );
  setMinimumWidth ( 350 );
  setMinimumHeight ( 100 );
  setSizeGripEnabled ( true );
  setContentsMargins ( 2, 2, 2, 9 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  message = new QLabel ( trUtf8 ( "Message" ), this );
  message->setIndent ( 2 );
  message->setWordWrap ( true );
  vLayout->addWidget ( message );

  QDialogButtonBox::StandardButtons buttons = ( QDialogButtonBox::Ok );
  QDialogButtonBox* box = new QDialogButtonBox ( buttons, Qt::Horizontal, this );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
}

void ErrorsDialog::setMessage ( const QString &m )
{
  message->setText ( m );
}

ErrorsDialog::~ErrorsDialog()
{}
