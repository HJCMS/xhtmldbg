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

#include "configcookies.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QIcon>
#include <QtGui/QPushButton>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>

ConfigCookies::ConfigCookies ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Cookie Management" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_cookies" ) );
  setNotice ( false );
  setCheckable ( false );

  QSizePolicy sizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
  sizePolicy.setHorizontalStretch ( 0 );
  sizePolicy.setVerticalStretch ( 0 );

  Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QGridLayout* mainLayout = new QGridLayout ( centralWidget );
  mainLayout->setObjectName ( QLatin1String ( "config_cookies_main_layout" ) );
  mainLayout->setContentsMargins ( 0, 5, 0, 5 );

  // Erste Zeile Keks Tabelle
  cookiesTable = new EditCookiesTable ( centralWidget );
  cookiesTable->setMinimumHeight ( 200 );
  cookiesTable->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  mainLayout->addWidget ( cookiesTable, 0, 0, 1, 4 );

  // Zweite Zeile Tabellen Aktionen
  QSpacerItem* spacer1 = new QSpacerItem ( 445, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  mainLayout->addItem ( spacer1, 1, 0, 1, 2 );

  QPushButton* removeCookieItem = new QPushButton ( centralWidget );
  removeCookieItem->setObjectName ( QLatin1String ( "removeCookieItem" ) );
  removeCookieItem->setIcon ( QIcon::fromTheme ( QLatin1String ( "list-remove" ) ) );
  removeCookieItem->setText ( trUtf8 ( "Remove" ) );
  mainLayout->addWidget ( removeCookieItem, 1, 2, 1, 1 );

  QPushButton* removeAllCookies = new QPushButton ( centralWidget );
  removeAllCookies->setObjectName ( QLatin1String ( "removeAllCookies" ) );
  removeAllCookies->setIcon ( QIcon::fromTheme ( QLatin1String ( "archive-remove" ) ) );
  removeAllCookies->setText ( trUtf8 ( "Clear Table" ) );
  mainLayout->addWidget ( removeAllCookies, 1, 3, 1, 1 );

  // Dritte Zeile Keks einfügen
  QGroupBox* editGroup = new QGroupBox ( trUtf8 ( "Add Cookie" ), centralWidget );
  editGroup->setObjectName ( QLatin1String ( "config_cookies_add_group" ) );

  QGridLayout* editLayout = new QGridLayout ( editGroup );
  editLayout->setObjectName ( QLatin1String ( "config_cookies_edit_layout" ) );

  QLabel* label_edit = new QLabel ( editGroup );
  label_edit->setObjectName ( QLatin1String ( "config_cookies_edit_label" ) );
  label_edit->setAlignment ( labelAlign );
  label_edit->setIndent ( 3 );
  label_edit->setText ( trUtf8 ( "Hostname" ) );
  editLayout->addWidget ( label_edit, 0, 0, 1, 1 );

  cookieEdit = new QLineEdit ( editGroup );
  cookieEdit->setObjectName ( QLatin1String ( "config_cookies_lineedit_cookie" ) );
  cookieEdit->setSizePolicy ( sizePolicy );
  editLayout->addWidget ( cookieEdit, 0, 1, 1, 2 );

  // Vierte Zeile Keks einfügen
  selectedArrangementType = new QComboBox ( editGroup );
  selectedArrangementType->setObjectName ( QLatin1String ( "selectedArrangementType" ) );
  selectedArrangementType->insertItem ( 0, trUtf8 ( "Merely current Session" ) );
  selectedArrangementType->insertItem ( 1, trUtf8 ( "Always Blocked" ) );
  selectedArrangementType->insertItem ( 2, trUtf8 ( "Always Allowed" ) );
  editLayout->addWidget ( selectedArrangementType, 1, 0, 1, 1 );

  QSpacerItem* spacer2 = new QSpacerItem ( 445, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  editLayout->addItem ( spacer2, 1, 1, 1, 1 );

  QPushButton* addCookie = new QPushButton ( editGroup );
  addCookie->setObjectName ( QString::fromUtf8 ( "config_cookies_add_cookie_button" ) );
  addCookie->setText ( trUtf8 ( "Add" ) );
  addCookie->setIcon ( QIcon::fromTheme ( QLatin1String ( "list-add" ) ) );
  editLayout->addWidget ( addCookie, 1, 2, 1, 1 );

  editGroup->setLayout ( editLayout );

  mainLayout->addWidget ( editGroup, 2, 0, 1, 4 );

  centralWidget->setLayout ( mainLayout );

  // Signale
  connect ( addCookie, SIGNAL ( clicked() ), this, SLOT ( addCookieAccess() ) );
  connect ( removeCookieItem, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeItem() ) );
  connect ( removeAllCookies, SIGNAL ( clicked() ), cookiesTable, SLOT ( removeAll() ) );
  connect ( cookiesTable, SIGNAL ( modified() ), this, SLOT ( itemModified() ) );
}

/**
* Modifikationen Weiterleiten
*/
void ConfigCookies::itemModified ()
{
  emit modified ( true );
}

/**
* Eine Domäne in die Cookies Liste aufnehmen.
*/
void ConfigCookies::addCookieAccess()
{
  if ( cookieEdit->text().isEmpty() )
    return;

  QUrl::FormattingOptions stripOptions ( QUrl::RemovePath |
                                         QUrl::RemoveQuery |
                                         QUrl::RemoveFragment |
                                         QUrl::StripTrailingSlash
                                       );

  QUrl url;
  url.setScheme ( "http" );
  url.setHost ( cookieEdit->text().remove ( QRegExp ( "\\bwww\\." ) ) );

  QUrl domain ( url.toString ( stripOptions ), QUrl::StrictMode );
  if ( domain.isValid() )
  {
    if ( cookiesTable->addCookie ( selectedArrangementType->currentIndex(), domain.host() ) )
    {
      cookieEdit->clear();
      emit modified ( true );
    }
    else
      cookieEdit->setFocus();
  }
}

void ConfigCookies::load ( QSettings * cfg )
{
  cookiesTable->loadCookieArrangements ( cfg );
}

void ConfigCookies::save ( QSettings * cfg )
{
  cookiesTable->saveCookieArrangements ( cfg );
}

void ConfigCookies::defaults()
{}

ConfigCookies::~ConfigCookies()
{}
