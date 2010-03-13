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

#include "cookieacceptdialog.h"
#include "editcookiestable.h"

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QVBoxLayout>

CookieAcceptDialog::CookieAcceptDialog ( const QUrl &url, QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "cookieacceptdialog" ) );
  setWindowTitle ( trUtf8 ( "URL Cookie Requirement" ) );
  setMinimumWidth ( 500 );
  setMinimumHeight ( 250 );
  setSizeGripEnabled ( true );

  QString host = url.host();
  host.remove ( QRegExp ( "^\\bwww\\." ) );

  // Settings
  m_settings = new QSettings ( QSettings::NativeFormat,
                               QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  QString info = trUtf8 ( "The Host have sent a Cookie request, add here the Arrangement." );
  layout->addWidget ( new QLabel ( info, this ) );

  m_editCookiesTable = new EditCookiesTable ( this );
  m_editCookiesTable->loadCookieArrangements ( m_settings );
  m_editCookiesTable->addCookie ( 1, host );

  QList<QTableWidgetItem *> list = m_editCookiesTable->findItems ( host, Qt::MatchExactly );
  if ( list.size() > 0 )
    m_editCookiesTable->setCurrentItem ( list.at ( 0 ) );

  layout->addWidget ( m_editCookiesTable );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "buttonBox" ) );
  QPushButton* save = box->addButton ( trUtf8 ( "Ready" ), QDialogButtonBox::ActionRole );
  layout->addWidget ( box );

  setLayout ( layout );

  connect ( save, SIGNAL ( clicked() ), this, SLOT ( saveAndExit() ) );
}

void CookieAcceptDialog::saveAndExit()
{
  m_editCookiesTable->saveCookieArrangements ( m_settings );

  accept();
}

CookieAcceptDialog::~CookieAcceptDialog()
{}
