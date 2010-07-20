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

#include "cookieaccesscombobox.h"

/* QtCore */
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QStandardItemModel>
#include <QtGui/QIcon>
#include <QtGui/QStandardItem>

CookieAccessComboBox::CookieAccessComboBox ( QWidget * parent, int type )
    : QComboBox ( parent )
{
  QIcon icon;
  QStandardItemModel* model = new QStandardItemModel ( this );
  model->setItem ( 0, new QStandardItem ( icon.fromTheme ( QString::fromUtf8 ( "view-history" ) ), trUtf8 ( "Session" ) ) );
  model->setItem ( 1, new QStandardItem ( icon.fromTheme ( QString::fromUtf8 ( "list-remove" ) ), trUtf8 ( "Blocked" ) ) );
  model->setItem ( 2, new QStandardItem ( icon.fromTheme ( QString::fromUtf8 ( "list-add" ) ), trUtf8 ( "Allowed" ) ) );
  setModel ( model );

  setCurrentIndex ( type );
  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SIGNAL ( itemChanged() ) );
}

void CookieAccessComboBox::setValue ( int index )
{
  setCurrentIndex ( index );
}

int CookieAccessComboBox::value()
{
  return currentIndex();
}

const QString CookieAccessComboBox::text()
{
  return currentText();
}
