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

#ifndef COOKIESEDITORTABLE_H
#define COOKIESEDITORTABLE_H

/* QtCore */
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

/* QtSql */
#include <QtSql/QSqlDatabase>

class CookiesEditorTable : public QTableWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QSqlDatabase sql;
    bool initialDatabase ();

  private Q_SLOTS:
    void cellChanged ( const QModelIndex & );

  Q_SIGNALS:
    void modified();

  public Q_SLOTS:
    void markCookie ( const QString & );
    void loadCookieAccess ();
    void saveCookieAccess ();
    void removeItem();
    void removeAll();

  public:
    CookiesEditorTable ( QWidget * parent = 0 );
    bool addCookie ( int, const QString &, int rfc = 1 );
    virtual ~CookiesEditorTable();
};

#endif
