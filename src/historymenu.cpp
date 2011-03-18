/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "historymenu.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>

/* KDE */
#include <KDE/KLocale>

HistoryMenu::HistoryMenu ( QMenu * parent )
    : QMenu ( parent )
    , defaultIcon ( QIcon::fromTheme ( QLatin1String ( "bookmarks" ) ) )
{
  setObjectName ( QLatin1String ( "historymenu" ) );
  setTitle ( i18n ( "History" ) );
  setIcon ( defaultIcon );
  m_mapper = new QSignalMapper();

  connect ( m_mapper, SIGNAL ( mapped ( const QString & ) ),
            this, SLOT ( clicked ( const QString & ) ) );

}

void HistoryMenu::clicked ( const QString &path )
{
  QUrl url ( path );
  if ( url.isValid() )
    emit openBookmark ( url );
}

void HistoryMenu::updateHistoryItems ( const QList<HistoryItem> &items )
{
  if ( items.count() >= 1 )
  {
    clear();
    foreach ( HistoryItem item, items )
    {
      QString url = item.url;
      QString title = ( item.title.isEmpty() ) ? url.left ( 25 ) : item.title;
      QAction* ac = addAction ( defaultIcon, title );
      ac->setObjectName ( url );
      ac->setStatusTip ( url );
      connect ( ac, SIGNAL ( triggered() ), m_mapper, SLOT ( map() ) );
      m_mapper->setMapping ( ac, url );
    }
  }
}

HistoryMenu::~HistoryMenu()
{
  clear();
}

