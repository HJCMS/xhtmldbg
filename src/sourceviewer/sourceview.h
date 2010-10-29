/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef SOURCEVIEW_H
#define SOURCEVIEW_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QFont>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>
#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QWidget>

class Highlighter;
class ContextMenu;

class SourceView : public QTextEdit
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    Highlighter* m_highlighter;
    QListWidget* m_listWidget;
    QMenu* editMenu;
    ContextMenu* m_contextMenu;
    bool setBlockWithNumber ( int row, int column = 0 );

  private Q_SLOTS:
    void createListWidgetItems();
    void cursorPosChanged();
    void swapWordWrap();
    virtual void Clipboard();

  protected:
    virtual void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void textChanged ( const QList<QListWidgetItem*> & );
    void lineChanged ( int );
    void check();
    void format();

  public Q_SLOTS:
    void fetchRow ( QListWidgetItem * );
    void setSource ( const QString & );
    void setCursorToRow ( int row, int column = 0 );
    void saveSource();
    void printSource();

  public:
    SourceView ( const QFont &font, QWidget * parent = 0 );
    const QString source();
    ~SourceView();

};

#endif
