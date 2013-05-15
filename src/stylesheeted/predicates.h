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

#ifndef PREDICATES_H
#define PREDICATES_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QWidget>
#include <QtGui/QListWidget>

/* QtWebKit */
#include <QtWebKit/QWebElement>

/* KDE */
#include <KDE/KIcon>

class Predicates : public QListWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const KIcon cascadedStyle;
    const KIcon inlineStyle;
    const QListWidgetItem constructItem();

  Q_SIGNALS:
    void className ( const QString &cl );

  public Q_SLOTS:
    void predicateChanged ( const QString &pa, const QString &va );

  public:
    explicit Predicates ( QWidget * parent = 0 );
    void setElement ( const QWebElement & );
    virtual ~Predicates();
};

#endif
