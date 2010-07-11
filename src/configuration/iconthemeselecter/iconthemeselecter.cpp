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

#include "iconthemeselecter.h"
#include "iconthememodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

IconThemeSelecter::IconThemeSelecter ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "iconthemeselecter" ) );
  m_model = new IconThemeModel ( this );
  setModel ( m_model );

  connect ( this, SIGNAL ( currentIndexChanged ( const QString & ) ),
            this, SLOT ( indexChanged ( const QString & ) ) );
}

/**
* Wenn eine Auswahl getroffen wurde das Signal
* @ref modified abstoßen.
*/
void IconThemeSelecter::indexChanged ( const QString &txt )
{
  Q_UNUSED ( txt )
  emit modified ( true );
}

/**
* Alle Inhalte aus dem Model entfernen!
*/
void IconThemeSelecter::clearContent ()
{
  m_model->clear();
}

/**
* Suche in @ref QIcon::themeSearchPaths nach den Themes
*/
void IconThemeSelecter::findThemeIndexes ( const QStringList &list )
{
  QDir dir;
  QStringList filter ( "*" );
  QStringList themeDirs = list;
  themeDirs.removeDuplicates();

  // erst Bereinigen
  clearContent();

  foreach ( QString p, themeDirs )
  {
    dir.setPath ( p );
    if ( dir.dirName().contains ( QRegExp ( "\\bdefault\\b", Qt::CaseInsensitive ) ) )
      continue;

    if ( dir.exists() )
    {
      foreach ( QString f, dir.entryList ( filter, ( QDir::Dirs | QDir::NoSymLinks ), QDir::Name ) )
      {
        QFileInfo info ( dir, QString ( "%1/index.theme" ).arg ( f ) );
        if ( info.exists() )
          m_model->insert ( info.absoluteFilePath() );
      }
    }
  }
}

/**
* Gibt den Verzeichniss namen des ausfewählten Themas zurück!
*/
const QString IconThemeSelecter::selectedTheme()
{
  int index = currentIndex();
  return itemData ( index, Qt::UserRole ).toString();
}

/**
* Setz den Verzeichniss Namen für das aktuelle Icon Thema
*/
void IconThemeSelecter::setTheme ( const QString &folderName )
{
  int index = findData ( folderName, Qt::UserRole );
  if ( index > 0 )
    setCurrentIndex ( index );
  else
    setCurrentIndex ( 0 );
}

IconThemeSelecter::~IconThemeSelecter()
{}
