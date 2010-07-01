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

#include "configurationmenu.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QSizePolicy>

ConfigurationMenu::ConfigurationMenu ( QWidget * parent, QSettings * settings )
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

  insertItem ( 0, trUtf8 ( "Defaults" ), icon.fromTheme ( QLatin1String ( "preferences-system-windows" ) ) );
  insertItem ( 1, trUtf8 ( "Tidy" ), icon.fromTheme ( QLatin1String ( "applications-development-web" ) ) );
  insertItem ( 2, trUtf8 ( "Dom Inspector" ), icon.fromTheme ( QLatin1String ( "view-web-browser-dom-tree" ) ) );
  insertItem ( 3, trUtf8 ( "Browser" ), icon.fromTheme ( QLatin1String ( "internet-web-browser" ) ) );
  insertItem ( 4, trUtf8 ( "Cookies" ), icon.fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
  insertItem ( 5, trUtf8 ( "Proxy" ), icon.fromTheme ( QLatin1String ( "preferences-system-network-sharing" ) ) );
  insertItem ( 6, trUtf8 ( "Certification" ), icon.fromTheme ( QLatin1String ( "preferences-web-browser-identification" ) ) );
  insertItem ( 7, trUtf8 ( "User Agent" ), icon.fromTheme ( QLatin1String ( "preferences-desktop-user" ) ) );
  insertItem ( 8, trUtf8 ( "Geological IP" ), icon.fromTheme ( QLatin1String ( "applications-education-language" ) ) );

  connect ( this, SIGNAL ( itemClicked ( QListWidgetItem * ) ),
            this, SLOT ( findPage ( QListWidgetItem * ) ) );
}

void ConfigurationMenu::findPage ( QListWidgetItem * item )
{
  int index = item->data ( Qt::UserRole ).toUInt();
  emit itemClicked ( index );
}

void ConfigurationMenu::insertItem ( int index, const QString &title, const QIcon &icon )
{
  QString tip = trUtf8 ( "%1 Configuration options." ).arg ( title );
  QListWidgetItem* item = new QListWidgetItem ( icon, title, this );
  item->setData ( Qt::UserRole, index );
  item->setData ( Qt::ToolTipRole, tip );
  addItem ( item );
}

ConfigurationMenu::~ConfigurationMenu()
{}
