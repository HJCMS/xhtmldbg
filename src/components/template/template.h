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

#ifndef @TEMPLATE@_H
#define @TEMPLATE@_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptable>

class Q_DECL_EXPORT @TEMPLATE@
      : public QWidget
      , protected QScriptable
{
    Q_OBJECT
    // Q_PROPERTY ( QString ?? READ ?? WRITE ?? )

  public:
    explicit @TEMPLATE@ ( QWidget * parent = 0 );
    virtual ~@TEMPLATE@();
};

#endif
