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

// XRegistrationFormular
#include "xregistrationformular.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QTextCodec>
#include <QtCore/QVariant>

/* qjson */
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

void XRegistrationFormular::setElementData ( const QString &name, const QVariant &value )
{
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
  else
    qWarning ( "Unknown Object", qPrintable ( name ) );
}

/**
* Set Meta-Object Property Title for GroupBox
**/
void XRegistrationFormular::setTitle ( const QString &t )
{
  person_box->setTitle ( t );
}

/**
* Current GroupBox Title
**/
QString XRegistrationFormular::title() const
{
  return person_box->title();
}

/**
* Set Meta-Object Property Title for GroupBox
**/
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
* Current GroupBox Title
**/
QString XRegistrationFormular::data() const
{
  QVariantMap map;
  QByteArray json;

  map.insert ( "encoding", "UTF-8" );
  foreach ( QLineEdit* e, findChildren<QLineEdit*>() )
  {
    if ( e->objectName().contains ( "person" ) )
      map.insert ( e->objectName(), e->text() );
  }
  // Radio & SpinBox
  map.insert ( "person_house_female", person_house_female->isChecked() );
  map.insert ( "person_house_male", person_house_male->isChecked() );
  map.insert ( "person_age", person_age->value() );
  map.insert ( "use_space", false );
  map.insert ( "length", map.size() + 1 );

  QJson::Serializer serializer;
  json = serializer.serialize ( map );

  QTextCodec* codec = QTextCodec::codecForHtml ( json );
  return codec->toUnicode ( json );
}

XRegistrationFormular::~XRegistrationFormular()
{}
