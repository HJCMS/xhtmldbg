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

#include "addresstoolbar.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QCompleter>
#include <QtGui/QIcon>

AddressToolBar::AddressToolBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "addresstoolbar" ) );
  setWindowTitle ( trUtf8 ( "Address" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( Qt::TopToolBarArea | Qt::BottomToolBarArea );
  setMinimumWidth ( 350 );

  QIcon icon;

  QLabel *label = new QLabel ( trUtf8 ( "Address:" ), this );
  label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( label );

  m_lineEdit = new QLineEdit ( "http://", this );
  m_lineEdit->setMinimumWidth ( 400 );
  QStringList urls; // = m_settings->getHistoryList();
  if ( urls.size() >= 1 )
  {
    QCompleter *cpl = new QCompleter ( urls, this );
    m_lineEdit->setCompleter ( cpl );
  }
  addWidget ( m_lineEdit );

  QAction *cb = addAction ( trUtf8 ( "Clear" ) );
  cb->setIcon( icon.fromTheme ( QLatin1String( "edit-clear-locationbar-rtl" ) ) );

  connect ( m_lineEdit, SIGNAL ( returnPressed () ), this, SLOT ( checkInput () ) );
  connect ( cb, SIGNAL ( triggered() ), m_lineEdit, SLOT ( clear() ) );
}

void AddressToolBar::setUrl ( const QUrl &url )
{
  m_lineEdit->setText ( url.toString() );
}

void AddressToolBar::checkInput ()
{
  QUrl url ( m_lineEdit->text() );
  if ( !url.isValid() || url.isRelative() )
    return;

  if ( !url.scheme().contains ( "http" ) )
    return;

  emit urlChanged ( url );
}

AddressToolBar::~AddressToolBar()
{
}
