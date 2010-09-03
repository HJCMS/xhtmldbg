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

#ifndef LISTSTYLESHEET_H
#define LISTSTYLESHEET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>

/* QtWebKit */
#include <QtWebKit/QWebElement>

/* xhtmldbg */
#include "settings.h"

class ListStyleSheet : public QListWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    Settings* cfg;
    const QIcon cascadedStyle;
    const QIcon inlineStyle;
    QString excludeRgb;
    QVector<QString> cssAttributes;
    bool exclude ( const QString &, const QString & );
    void addStyleItem ( const QString &, const QString &, bool cascaded = true );

  public Q_SLOTS:
    void setStyleSheetList ( const QWebElement & );

  public:
    ListStyleSheet ( QWidget * parent = 0, Settings * settings = 0 );
    ~ListStyleSheet();
};

#endif
