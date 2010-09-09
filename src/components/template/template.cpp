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

#include "@TEMPLATE@.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* QtScript */
#include <QtScript/QScriptEngine>

@TEMPLATE@::@TEMPLATE@ ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "@TEMPLATE@" ) );
  setMinimumWidth ( 250 );
  setMinimumHeight ( 200 );
  setContentsMargins ( 5, 5, 5, 5 );

  QScriptEngine p_scriptEngine ( this );
  p_scriptEngine.setObjectName ( QLatin1String ( "@TEMPLATE@ScriptEngine" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  setLayout ( vLayout );
}

@TEMPLATE@::~@TEMPLATE@()
{}
