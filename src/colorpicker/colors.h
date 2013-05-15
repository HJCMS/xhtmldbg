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

#ifndef COLORS_H
#define COLORS_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QColor>

class Colors : public QObject
{
  public:
    static const QVector<QColor> webColors();
    static const QVector<QColor> royalColors();
    static const QVector<QColor> rainbowColors();
    static const QVector<QColor> grayColors();
    static const QVector<QColor> topographicColors();
    static const QStringList namedSVGColors();
};

#endif
