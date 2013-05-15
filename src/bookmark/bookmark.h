/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2013
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

#ifndef BOOKMARK_H
#define BOOKMARK_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

class BookmarkReader;
class BookmarkEditor;

class Bookmark : public QMenu
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_PROPERTY ( int recent READ getRecent WRITE setRecent )

  private:
    const QIcon defaultIcon;
    int recent;
    // Bookmark Menues
    BookmarkReader* m_bookmarkReader;
    QMenu* mainMenu;
    QMenu* lastlyMenu;
    // Editor
    BookmarkEditor *m_bookmarkEditor;
    QAction* editorMenu;

  private Q_SLOTS:
    void loadBookmarkMenu();
    void getBookmark ( const QString & );

  Q_SIGNALS:
    void openBookmark ( const QUrl & );

  public Q_SLOTS:
    void openBookmarkEditor ();
    void addBookmark ( const QUrl &, const QString & );

  public:
    Bookmark ( QMenu * parent = 0 );
    int getRecent() const;
    void setRecent ( int );
    ~Bookmark();
};

#endif
