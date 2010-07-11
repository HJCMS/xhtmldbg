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

#include "iconthemeslist.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QSizePolicy>

IconThemesList::IconThemesList ( QWidget * parent )
    : QListWidget ( parent )
{
  setObjectName ( QLatin1String ( "iconthemeslist" ) );
  setToolTip ( trUtf8 ( "descending order for icon theme search paths" ) );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
}

/**
* Suche pfad in der Liste
*/
bool IconThemesList::itemExists ( const QString &p )
{
  for ( int i = 0 ; i < count(); i++ )
  {
    if ( item ( i )->text().contains ( p ) )
      return true;
  }
  return false;
}

/**
* Ausgewählte Einträge aus der Liste entfernen.
*/
void IconThemesList::removeSelectedPath()
{
  if ( count() <= 1 )
    return; // Keine Leere List erlauben!

  foreach ( QListWidgetItem* item, selectedItems() )
  {
    delete item;
  }
  update();
  emit modified ( true );
}

/**
* Kontext Menü aktionen für das Editieren
*/
void IconThemesList::contextMenuEvent ( QContextMenuEvent * ev )
{
  QMenu* m_menu = new QMenu ( "Actions", this );
  QAction* add = m_menu->addAction ( QIcon::fromTheme ( "list-add" ), trUtf8 ( "Adding" ) );
  connect ( add, SIGNAL ( triggered() ), this, SIGNAL ( setPathClicked() ) );

  QAction* del = m_menu->addAction ( QIcon::fromTheme ( "list-remove" ), trUtf8 ( "Remove" ) );
  connect ( del, SIGNAL ( triggered() ), this, SLOT ( removeSelectedPath() ) );
  m_menu->exec ( ev->globalPos() );
  delete m_menu;
}

/**
* Füge einen Suchpfad in die Liste ein.
*/
void IconThemesList::addPath ( const QString &p )
{
  if ( ! itemExists ( p ) )
  {
    addItem ( new QListWidgetItem ( p ) );
    emit modified ( true );
  }
}

/**
* Alle Suchpfade zurück geben.
*/
const QStringList IconThemesList::iconPaths()
{
  QStringList list;
  for ( int i = 0 ; i < count(); i++ )
  {
    if ( item ( i )->text().isEmpty() )
      continue;

    list.append ( item ( i )->text() );
  }
  return list;
}

/**
* Komplette Liste \b NEU einfügen!
*/
void IconThemesList::insertPaths ( const QStringList &list )
{
  clear();
  insertItems ( 0, list );
}

IconThemesList::~IconThemesList()
{}
