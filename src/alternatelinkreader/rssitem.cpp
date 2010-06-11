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

#include "rssitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>

RSSItem::RSSItem ( const QString &text, const QUrl &url, const QString &title )
    : QStandardItem ( QIcon::fromTheme ( QLatin1String ( "application-rss+xml" ) ), text )
{
  setData ( QVariant ( url ), Qt::UserRole );
  setText ( text );
  setToolTip ( text );
  setStatusTip ( title );
  setEditable ( false );
  setEnabled ( url.isValid() );
  setFlags ( ( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
}
