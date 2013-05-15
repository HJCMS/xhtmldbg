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

#include "formconstructor.h"

/* QtCore */
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

/* QtWebKit */
#include <QtWebKit/QWebElementCollection>

/**
* Diese Klasse liest ein Formular in ein neues QWebElement
* und entfernt dabei alle nicht benötigten Prädikate.
*/
FormConstructor::FormConstructor ( const QWebElement &form )
    : QWebElement ( form )
{
  if ( tagName().contains ( "FORM", Qt::CaseInsensitive ) )
    rebuild();
}

/**
* Überprüft ob das HTML Element mit in die erlaubte Liste aufgenommen werden kann.
*/
bool FormConstructor::isValidInputElement ( const QWebElement &e )
{
  if ( ! e.hasAttribute ( "type" ) )
    return false;

  QString t = e.attribute ( "type", QString::null );
  if ( t.isEmpty() )
    return false;

  if ( t.contains ( "TEXT", Qt::CaseInsensitive ) )
    return true;
  else if ( t.contains ( "PASSWORD", Qt::CaseInsensitive ) )
    return true;
  else if ( t.contains ( "CHECKBOX", Qt::CaseInsensitive ) )
    return true;
  else if ( t.contains ( "RADIO", Qt::CaseInsensitive ) )
    return true;
  else if ( t.contains ( "HIDDEN", Qt::CaseInsensitive ) )
    return true;
  else
    return false;
}

/**
* Ein neues \b nicht \e INPUT WebElement anhängen
* Es werden nur die Prädikate "type,name,id" übernommen!
*/
void FormConstructor::appendElement ( const QWebElement &ele )
{
  QWebElement element ( ele );
  QStringList restrict;
  restrict << "type" << "name" << "id";

  foreach ( QString name, ele.attributeNames() )
  {
    if ( restrict.contains ( name, Qt::CaseInsensitive ) )
      continue;
    else if ( element.hasAttribute ( name ) )
      element.removeAttribute ( name );
  }

  elements.append ( element );
}

/**
* Liest alle verwendbaren Attributes in ein Neues QWebElement
* und fügt dieses in die @ref items Liste ein.
* Es werden die Prädikate "type,name,id" übernommen, wenn "value"
* fehlt wird ein leeres Prädikat value eingefügt!
*/
void FormConstructor::appendInputElement ( const QWebElement &ele )
{
  QWebElement element ( ele );
  QStringList restrict;
  restrict << "type" << "name" << "id" << "value";

  foreach ( QString name, ele.attributeNames() )
  {
    if ( restrict.contains ( name, Qt::CaseInsensitive ) )
      continue;
    else if ( element.hasAttribute ( name ) )
      element.removeAttribute ( name );
  }

  if ( ! element.hasAttribute ( "value" ) )
    element.setAttribute ( "value", "" );

  elements.append ( element );
}

/**
* Sucht alle "INPUT" Elemente
*/
void FormConstructor::findInputs()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "INPUT" ) ) )
  {
    if ( isValidInputElement ( e ) )
      appendInputElement ( e );
  }
}

/**
* Sucht alle "TEXTAREA" Elemente
*/
void FormConstructor::findTextAreas()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "TEXTAREA" ) ) )
  {
    appendElement ( e );
  }
}

/**
* Sucht alle "SELECT" Elemente ohne die "OPTION" Element zu
* berücksichtigen!
*/
void FormConstructor::findSelections()
{
  foreach ( QWebElement e, findAll ( QLatin1String ( "SELECT" ) ) )
  {
    appendElement ( e );
  }
}

/** Sieht nach ob das Form Element ein id oder name Prädikat besitzt,
* \li Wenn kein Prädikat vorhanden wird eine hash erstellt.
*/
const QString FormConstructor::uniqueId() const
{
  // Erster Versucht
  QString id = attribute ( "id", QString::null );

  // zweiter Versuch
  if ( id.isEmpty() )
    id = attribute ( "name", QString::null );

  // hardcore :-/
  if ( id.isEmpty() )
  {
    QCryptographicHash hash ( QCryptographicHash::Md5 );
    hash.addData ( toPlainText().toUtf8() );
    id = "md5{";
    id.append ( QString ( hash.result().toHex() ) );
    id.append ( "}" );
  }
  return id;
}

/**
* Leert @ref elements und liest das übergebene Form neu ein.
*/
void FormConstructor::rebuild()
{
  elements.clear();
  findInputs();
  findTextAreas();
  findSelections();
}

FormConstructor::~FormConstructor()
{
  elements.clear();
}
