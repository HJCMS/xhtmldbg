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
**/

#include "borderstyle.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>

BorderStyle::BorderStyle ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "BorderStyle" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );

  // borderGroupBox {
  QGroupBox* borderGroupBox = new QGroupBox ( this );
  borderGroupBox->setCheckable ( true );
  borderGroupBox->setChecked ( false );

  QGridLayout* gridLayout = new QGridLayout ( borderGroupBox );

  m_borderWidth = new KIntNumInput ( borderGroupBox );
  m_borderWidth->setValue ( 0 );
  m_borderWidth->setRange ( 0, 100 );
  m_borderWidth->setSpecialValueText ( QLatin1String ( "border-width" ) );
  m_borderWidth->setSliderEnabled ( true );
  gridLayout->addWidget ( m_borderWidth, 0, 0, 1, 1 );

  m_borderSpacing = new KIntNumInput ( borderGroupBox );
  m_borderSpacing->setValue ( 0 );
  m_borderSpacing->setRange ( 0, 100 );
  m_borderSpacing->setSpecialValueText ( QLatin1String ( "border-spacing" ) );
  m_borderSpacing->setSliderEnabled ( true );
  gridLayout->addWidget ( m_borderSpacing, 0, 1, 1, 1 );

  m_borderRadius = new KIntNumInput ( borderGroupBox );
  m_borderRadius->setValue ( 0 );
  m_borderRadius->setRange ( 0, 500 );
  m_borderRadius->setSpecialValueText ( QLatin1String ( "border-radius" ) );
  m_borderRadius->setSliderEnabled ( true );
  gridLayout->addWidget ( m_borderRadius, 1, 0, 1, 1 );

  m_borderStyle = new KComboBox ( borderGroupBox );
  gridLayout->addWidget ( m_borderStyle, 1, 1, 1, 1 );

  borderGroupBox->setLayout ( gridLayout );
  verticalLayout->addWidget ( borderGroupBox );
  // } borderGroupBox

  setLayout ( verticalLayout );
}

BorderStyle::~BorderStyle()
{}
