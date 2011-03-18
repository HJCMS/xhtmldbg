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

#include "bookmark.h"
#include "bookmarkitem.h"
#include "bookmarkreader.h"
#include "bookmarkeditor.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QDialog>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

Bookmark::Bookmark ( QMenu * parent )
    : QMenu ( parent )
    , defaultIcon ( KIcon ( QLatin1String ( "bookmarks" ) ) )
    , recent ( 5 )
{
  Q_INIT_RESOURCE ( bookmark );
  setObjectName ( QLatin1String ( "bookmarkmenu" ) );
  setTitle ( i18n ( "Bookmarks" ) );
  setIcon ( defaultIcon );

  m_bookmarkReader = new BookmarkReader ( this );
  loadBookmarkMenu();

  connect ( m_bookmarkReader, SIGNAL ( openBookmark ( const QString & ) ),
            this, SLOT ( getBookmark ( const QString & ) ) );
}

void Bookmark::loadBookmarkMenu()
{
  QString bfile = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  bfile.append ( QDir::separator() );
  bfile.append ( QLatin1String ( "bookmarks.xbel" ) );
  QFileInfo info ( bfile );
  if ( ! info.exists() )
    bfile = QString::fromUtf8 ( ":/default.xml" );

  QFile fp ( bfile );
  if ( !fp.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  if ( m_bookmarkReader->read ( &fp ) )
    fp.close();
}

void Bookmark::openBookmarkEditor()
{
  BookmarkEditor editor ( this );
  if ( editor.exec() )
    loadBookmarkMenu();
}

void Bookmark::getBookmark ( const QString &str )
{
  QUrl url ( str );
  if ( url.isValid() )
    emit openBookmark ( url );
}

void Bookmark::addBookmark ( const QUrl &url, const QString &title )
{
  BookmarkEditor editor ( this );
  editor.addBookmark ( url, title );
  if ( editor.exec() )
    loadBookmarkMenu();
}

int Bookmark::getRecent() const
{
  return recent;
}

void Bookmark::setRecent ( int i )
{
  recent = ( i >= 3 && i <= 30 ) ? i : 5;
}

Bookmark::~Bookmark()
{}
