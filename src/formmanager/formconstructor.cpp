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

#include "formconstructor.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

FormConstructor::FormConstructor ( const QWebElement &form )
    : QWebElement ( form )
{
  if ( tagName().contains ( "FORM", Qt::CaseInsensitive ) )
    rebuild();
}

FormConstructor::InputType FormConstructor::isInputType ( const QWebElement &e )
{
  if ( e.hasAttribute ( "type" ) )
  {
    QString t = e.attribute ( "type", QString::null );
    if ( t.isEmpty() )
      return NONE;

    if ( t.contains ( "TEXT", Qt::CaseInsensitive ) )
      return TEXT;
    else if ( t.contains ( "PASSWORD", Qt::CaseInsensitive ) )
      return PASSWORD;
    else if ( t.contains ( "CHECKBOX", Qt::CaseInsensitive ) )
      return CHECKBOX;
    else if ( t.contains ( "RADIO", Qt::CaseInsensitive ) )
      return RADIO;
    else if ( t.contains ( "HIDDEN", Qt::CaseInsensitive ) )
      return HIDDEN;
    else
      return NONE;
  }
  return NONE;
}

void FormConstructor::appendInput ( const QWebElement &ele, InputType type )
{
  QString xml ( "<input type=\"" );

  switch ( type )
  {
    case TEXT:
      xml.append ( ele.attribute ( "type", QLatin1String ( "text" ) ) );
      break;

    case PASSWORD:
      xml.append ( ele.attribute ( "type", QLatin1String ( "password" ) ) );
      break;

    case CHECKBOX:
      xml.append ( ele.attribute ( "type", QLatin1String ( "checkbox" ) ) );
      break;

    case RADIO:
      xml.append ( ele.attribute ( "type", QLatin1String ( "radio" ) ) );
      break;

    case HIDDEN:
      xml.append ( ele.attribute ( "type", QLatin1String ( "hidden" ) ) );
      break;

    default:
      return;
  }

  if ( ele.hasAttribute ( "name" ) )
  {
    xml.append ( "\" name=\"" );
    xml.append ( ele.attribute ( "name", QLatin1String ( "" ) ) );
  }

  if ( ele.hasAttribute ( "value" ) )
  {
    xml.append ( "\" value=\"" );
    xml.append ( ele.attribute ( "value", QLatin1String ( "" ) ) );
  }

  xml.append ( "\" />" );

  QWebElement element;
  element.setInnerXml ( xml );
  items.append ( element );
}

void FormConstructor::findInputs()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "INPUT" ) ) )
  {
    InputType t = isInputType ( e );
    if ( t != NONE )
      appendInput ( e, t );
  }
}

void FormConstructor::appendTextArea ( const QWebElement & )
{}

void FormConstructor::findTextAreas()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "TEXTAREA" ) ) )
  {
    appendTextArea ( e );
  }
}

void FormConstructor::appendSelection ( const QWebElement & )
{}

void FormConstructor::findSelections()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "SELECT" ) ) )
  {
    appendSelection ( e );
  }
}

void FormConstructor::rebuild()
{
  items.clear();
  findInputs();
  findTextAreas();
  findSelections();
}

const QList<QWebElement> FormConstructor::content()
{
  return items;
}

FormConstructor::~FormConstructor()
{
  items.clear();
}
