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
*/

// XRegistrationFormular
#include "xregistrationformular.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QTextCodec>

/* qjson */
#include <qjson/qobjecthelper.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>

XRegistrationFormular::XRegistrationFormular ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "xRegistrationFormular" ) );
  setupUi ( this );
  setMinimumWidth ( 250 );
  setMinimumHeight ( 200 );
}

/**
* Standard Map Elemente
*/
inline const QVariantMap XRegistrationFormular::initMap() const
{
  QVariantMap m;
  m.insert ( "encoding", "UTF-8" );
  m.insert ( "use_space", false );
  return m;
}

/**
* Element Suchen und Wert zuweisen
*/
void XRegistrationFormular::setElementData ( const QString &name, const QVariant &value )
{
  if ( initMap().contains ( name ) )
    return;

  if ( findChild<QLineEdit*> ( name ) )
  {
    findChild<QLineEdit*> ( name )->setText ( value.toString() );
  }
  else if ( findChild<QSpinBox*> ( name ) )
  {
    findChild<QSpinBox*> ( name )->setValue ( value.toUInt() );
  }
  else if ( findChild<QRadioButton*> ( name ) )
  {
    findChild<QRadioButton*> ( name )->setChecked ( value.toBool() );
  }
  else if ( findChild<QGroupBox*> ( name ) )
  {
    if ( findChild<QGroupBox*> ( name )->isCheckable() )
      findChild<QGroupBox*> ( name )->setChecked ( value.toBool() );
  }
  else
    qWarning ( "(XHTMLDBG) Unknown JSon Object - %s", qPrintable ( name ) );
}

/**
* Alle Felder zurück setzen
*/
void XRegistrationFormular::restore()
{
  foreach ( QLineEdit* e, findChildren<QLineEdit*>() )
  {
    if ( e->objectName().contains ( "person" ) )
      e->clear();
  }
  if ( person_house->isCheckable() )
    person_house->setChecked ( false );
}

/**
* Den Titel der oberen Gruppen Box ändern!
*/
void XRegistrationFormular::setTitle ( const QString &t )
{
  person_box->setTitle ( t );
}

/**
* Den Titel der oberen Gruppen Box ausgeben
*/
QString XRegistrationFormular::title() const
{
  return person_box->title();
}

/**
* Nehme einen JSon String und lese diesen in eine QVariantMap.
* Danach übergebe jeden Eintrag an @ref setElementData
*/
void XRegistrationFormular::setData ( const QString &d )
{
  if ( d.isEmpty() )
    return;

  QJson::Parser parser;
  QVariantMap map = parser.parse ( d.toAscii() ).toMap();
  QMap<QString,QVariant>::const_iterator i = map.constBegin();
  while ( i != map.constEnd() )
  {
    if ( ! i.key().isEmpty() )
      setElementData ( i.key(), i.value() );

    ++i;
  }
}

/**
* Alle Inhalte der Objekte in einen JSon String schreiben!
*/
QString XRegistrationFormular::data() const
{
  QByteArray json;
  QVariantMap map = initMap();

  // Erst alle Eingabe zeilen setzen
  foreach ( QLineEdit* e, findChildren<QLineEdit*>() )
  {
    if ( e->objectName().contains ( "person" ) )
      map.insert ( e->objectName(), e->text() );
  }
  // GroupBox, RadioButtons & SpinBoxes
  if ( person_house->isChecked() )
  {
    map.insert ( "person_house", person_house->isChecked() );
    map.insert ( "person_house_female", person_house_female->isChecked() );
    map.insert ( "person_house_male", person_house_male->isChecked() );
    map.insert ( "person_age", person_age->value() );
  }
  // Zum abschluss die Mappengröße setzen
  map.insert ( "length", map.size() + 1 );

  QJson::Serializer serializer;
  json = serializer.serialize ( map );

  QTextCodec* codec = QTextCodec::codecForHtml ( json );
  return codec->toUnicode ( json );
}

XRegistrationFormular::~XRegistrationFormular()
{
  restore();
}
