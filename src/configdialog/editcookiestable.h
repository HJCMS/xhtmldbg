/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#ifndef EDITCOOKIESTABLE_H
#define EDITCOOKIESTABLE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QModelIndex>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

class ArrangementItem : public QComboBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  Q_SIGNALS:
    void itemChanged();

  public:
    ArrangementItem ( const QString &name, int type, QWidget * parent = 0 );
};

class EditCookiesTable : public QTableWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private Q_SLOTS:
    void cellChanged ( const QModelIndex & );

  Q_SIGNALS:
    void modified();

  public Q_SLOTS:
    void loadCookieArrangements ( QSettings * cfg );
    void saveCookieArrangements ( QSettings * cfg );
    void removeItem();
    void removeAll();

  public:
    EditCookiesTable ( QWidget * parent = 0 );
    bool addCookie ( int, const QString & );
    ~EditCookiesTable();
};

#endif
