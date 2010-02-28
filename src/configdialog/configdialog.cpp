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

#include "configdialog.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

/*
UserAgentLanguage
*/

ConfigDialog::ConfigDialog ( QWidget * parent, QSettings * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "configdialog" ) );
  setWindowTitle ( trUtf8 ( "Configure xhtmldbg[*]" ) );
  setMinimumWidth ( 550 );
  setMinimumHeight ( 450 );
  setSizeGripEnabled ( true );

  setupUi ( this );

  // Modify ButtonBox
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );

  // connect ( , SIGNAL(), this, SLOT ( setModified() ) );
  connect ( StartUpUrl, SIGNAL ( editingFinished() ), this, SLOT ( setModified() ) );
  connect ( m_buttonSave, SIGNAL ( clicked() ), this, SLOT ( save() ) );
  // connect ( m_buttonRestore, SIGNAL ( clicked() ), this, SLOT ( () ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
}

void ConfigDialog::setModified()
{
  setWindowModified ( true );
}

void ConfigDialog::save()
{
  qDebug() << Q_FUNC_INFO;
  setWindowModified ( false );
}

void ConfigDialog::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
    status = QMessageBox::question ( this, trUtf8 ( "Unsaved Changes" ),
                                     trUtf8 ( "Found unsaved Changes.\nDo you realy wan to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  if ( status == QMessageBox::Yes )
    accept();
}

ConfigDialog::~ConfigDialog()
{}
