/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "lineeditor.h"

/* QtCore */
#include <QtCore/QRegExp>
#include <QtCore/QString>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

LineEditor::LineEditor ( QWidget * parent )
    : QLineEdit ( parent )
{
  setObjectName ( QLatin1String ( "LineEditor" ) );
}

void LineEditor::setData ( const QString &value )
{
  QStringList list;
  setText ( value );

  if ( text().contains ( QRegExp ( "^(on|off)$" ) ) )
    list << "on" << "off";
  else if ( text().contains ( QRegExp ( "^(0|1)$" ) ) )
  {
    for ( int i = 0; i < 30; i++ )
    {
      list << QString::number ( i );
    }
  }
  else
    list << text();

  m_completer = new QCompleter ( list, this );
  m_completer->setMaxVisibleItems ( 2 );
  m_completer->setCompletionMode ( QCompleter::UnfilteredPopupCompletion );
  setCompleter ( m_completer );
}

LineEditor::~LineEditor()
{}
