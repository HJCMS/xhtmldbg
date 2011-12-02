/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "iconthemeitem.h"

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLocale>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QIcon>

IconThemeItem::IconThemeItem ( const QSettings &cfg )
    : QStandardItem ()
{
  QFileInfo info ( cfg.fileName() );
  if ( ! info.exists() )
    return;

  QString lng = QLocale::system ().name().remove ( QRegExp ( "_.+$" ) );
  QString name = cfg.value ( QString::fromUtf8 ( "Icon Theme/Name[%1]" ).arg ( lng ) ).toString();
  if ( name.isEmpty() )
    name =  cfg.value ( QString::fromUtf8 ( "Icon Theme/Name" ) ).toString();

  QString comment = cfg.value ( QString::fromUtf8 ( "Icon Theme/Comment[%1]" ).arg ( lng ) ).toString();
  if ( comment.isEmpty() )
    comment =  cfg.value ( QString::fromUtf8 ( "Icon Theme/Comment" ) ).toString();

  setData ( QIcon::fromTheme ( "preferences-desktop-theme" ), Qt::DecorationRole );
  setData ( info.dir().dirName(), Qt::UserRole );
  setData ( name, Qt::DisplayRole );
  setData ( comment, Qt::ToolTipRole );
  setData ( comment, Qt::StatusTipRole );
  setData ( comment, Qt::WhatsThisRole );
}
