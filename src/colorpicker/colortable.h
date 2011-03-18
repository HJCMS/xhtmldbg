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

#ifndef ColorTable_H
#define ColorTable_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

class ColorTable : public QTableWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    int fixedColumnCount;
    int fixedSizeFactor;
    void insertColorCell ( int row, int column, const QColor &color );

  private Q_SLOTS:
    void selectedItem ( QTableWidgetItem * );

  Q_SIGNALS:
    void colorChanged ( const QColor & );

  public:
    ColorTable ( QWidget * parent = 0 );
    void insertColors ( const QVector<QColor> &vector );
    ~ColorTable();
};

#endif
