/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "configurationmenu.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QCursor>
#include <QtGui/QSizePolicy>

ConfigurationMenu::ConfigurationMenu ( QWidget * parent, Settings * settings )
    : QListWidget ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "configuration_menu" ) );
  setMinimumWidth ( 150 );
  setIconSize ( QSize ( 22, 22 ) );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setDragEnabled ( false );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );

  QIcon icon = QIcon::fromTheme ( "xhtmldbg" );

  int c = 0;
  insertItem ( c++, i18n ( "Defaults" ), icon.fromTheme ( QLatin1String ( "preferences-system-windows" ) ) );
  insertItem ( c++, i18n ( "Dom Inspector" ), icon.fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  insertItem ( c++, i18n ( "Browser" ), icon.fromTheme ( QLatin1String ( "internet-web-browser" ) ) );
  insertItem ( c++, i18n ( "Network" ), icon.fromTheme ( QLatin1String ( "preferences-system-network" ) ) );
  insertItem ( c++, i18n ( "Cookies" ), icon.fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
  insertItem ( c++, i18n ( "Proxy" ), icon.fromTheme ( QLatin1String ( "preferences-system-network-sharing" ) ) );
  insertItem ( c++, i18n ( "Certification" ), icon.fromTheme ( QLatin1String ( "preferences-web-browser-identification" ) ) );
  insertItem ( c++, i18n ( "User Agent" ), icon.fromTheme ( QLatin1String ( "preferences-desktop-user" ) ) );
  insertItem ( c++, i18n ( "HTML 5 Database" ), icon.fromTheme ( QLatin1String ( "applications-development-web" ) ) );
  insertItem ( c++, i18n ( "Extensions" ), icon.fromTheme ( QLatin1String ( "preferences-plugin" ) ) );

  connect ( this, SIGNAL ( itemClicked ( QListWidgetItem * ) ),
            this, SLOT ( findPage ( QListWidgetItem * ) ) );
}

void ConfigurationMenu::findPage ( QListWidgetItem * item )
{
  setCursor ( Qt::WaitCursor );
  int index = item->data ( Qt::UserRole ).toUInt();
  emit itemClicked ( index );
  unsetCursor ();
}

void ConfigurationMenu::insertItem ( int index, const QString &title, const QIcon &icon )
{
  QString tip = i18n ( "%1 Configuration options." ).arg ( title );
  QListWidgetItem* item = new QListWidgetItem ( icon, title, this );
  item->setData ( Qt::UserRole, index );
  item->setData ( Qt::ToolTipRole, tip );
  addItem ( item );
}

ConfigurationMenu::~ConfigurationMenu()
{}
