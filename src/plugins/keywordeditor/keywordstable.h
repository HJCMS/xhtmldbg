/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#ifndef KEYWORDSTABLE_H
#define KEYWORDSTABLE_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

class KeywordsDom;
class KeywordsTableModel;
class KeywordsTableItem;

class Q_DECL_EXPORT KeywordsTable : public QTableView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    KeywordsTableModel* m_model;
    QAction* m_insert;
    QAction* m_remove;

  private Q_SLOTS:
    void prepareModelIndex ( const QModelIndex &index );
    void actionInsertRow();
    void actionDeleteRow();

  Q_SIGNALS:
    void itemClicked ( const QModelIndex &index );

  public:
    KeywordsTable ( QWidget * parent = 0 );
    void setDomDocument ( const KeywordsDom &dom );
    const QList<KeywordsTableItem*> keywords();
    ~KeywordsTable();
};

#endif
