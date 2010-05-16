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

#include "ledlabel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QIcon>

LedLabel::LedLabel ( QWidget * parent )
    : QLabel ( parent )
{
  Q_INIT_RESOURCE ( ledlabel );
  setObjectName ( QLatin1String ( "ledlabel" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setScaledContents ( false );
  setAlignment ( ( Qt::AlignHCenter | Qt::AlignVCenter ) );

  QSize baseSize ( 50, 25 );

  QIcon onIcon ( QString::fromUtf8 ( ":/on.png" ) );
  on = onIcon.pixmap ( baseSize, QIcon::Normal, QIcon::On );

  QIcon offIcon ( QString::fromUtf8 ( ":/off.png" ) );
  off = offIcon.pixmap ( baseSize, QIcon::Normal, QIcon::On );

  setOn ( false );
}

void LedLabel::setOn ( bool b )
{
  if ( b )
  {
    setPixmap ( on );
    setToolTip ( trUtf8 ( "listening" ) );
  }
  else
  {
    setPixmap ( off );
    setToolTip ( trUtf8 ( "not listen" ) );
  }
  repaint();
}

LedLabel::~LedLabel()
{}
