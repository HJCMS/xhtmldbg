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

#include "xdebugspinbox.h"

/* QtCore */
#include <QtCore/QDebug>

/**
* @class xdebugspinbox
*
* Abgeleitete Klasse wegen Qt4 fehler.
* BUG Es werden nur QString's nach einem setModelData angenommen!
* Deshalb an dieser Stelle das ganze auf QString abgewandelt!
*/
XDebugSpinBox::XDebugSpinBox ( QWidget * parent )
    : QSpinBox ( parent )
{
  setObjectName ( QLatin1String ( "qspinportbox" ) );
  setRange ( 1024, 65535 );
  interpretText();
}

void XDebugSpinBox::setText ( const QString &txt )
{
  bool b;
  uint p = txt.toUInt ( &b, 10 );
  if ( b )
    setValue ( p );
}

const QString XDebugSpinBox::text()
{
  return QString::number ( value() );
}
