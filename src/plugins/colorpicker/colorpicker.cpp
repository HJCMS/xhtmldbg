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

#include "colorpicker.h"
#include "colors.h"
#include "colortable.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>

ColorPicker::ColorPicker ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "colorpicker" ) );
  setWindowTitle ( trUtf8 ( "Colors" ) );

  QWidget* layer = new QWidget ( this );
  layer->setObjectName ( QLatin1String ( "colorpicker.layer" ) );

  QGridLayout* layout = new QGridLayout ( layer );
  layout->setObjectName ( QLatin1String ( "colorpicker.layer.layout" ) );

  // Farbpaletten Auswahl
  m_colorComboBox = new QComboBox ( layer );
  m_colorComboBox->setObjectName ( "colorpicker.layer.layout.combobox" );
  m_colorComboBox->insertItem ( 0, trUtf8 ( "Web Colors" ) );
  m_colorComboBox->insertItem ( 1, trUtf8 ( "Royal Colors" ) );
  m_colorComboBox->insertItem ( 2, trUtf8 ( "Rainbow Colors" ) );
  layout->addWidget ( m_colorComboBox, 0, 0, 1, 2, Qt::AlignRight );

  // Farbpaletten Ausgeben
  m_colorTable = new ColorTable ( layer );
  m_colorTable->setObjectName ( "colorpicker.layer.layout.colortable" );
  m_colorTable->insertColors ( Colors::webColors() );
  layout->addWidget ( m_colorTable, 1, 0, 1, 2 );

  // Ausgabe Hex
  m_hexEdit = new QLineEdit ( layer );
  m_hexEdit->setObjectName ( "colorpicker.layer.layout.hexedit" );
  layout->addWidget ( m_hexEdit, 2, 0, 1, 1, Qt::AlignLeft );

  // Ausgabe RGB
  m_rgbEdit = new QLineEdit ( layer );
  m_rgbEdit->setObjectName ( "colorpicker.layer.rgbedit" );
  layout->addWidget ( m_rgbEdit, 2, 1, 1, 1, Qt::AlignRight );

  QSpacerItem* spacer = new QSpacerItem ( 2, 2, QSizePolicy::Preferred, QSizePolicy::Preferred );
  layout->addItem ( spacer, 3, 0, 1, 2 );

  // Layout abschliessen
  layer->setLayout ( layout );
  setWidget ( layer );

  connect ( m_colorComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( colorMapChanged ( int ) ) );
}

void ColorPicker::colorMapChanged ( int index )
{
  switch ( index )
  {
    case 0:
      m_colorTable->insertColors ( Colors::webColors() );
      break;

    case 1:
      m_colorTable->insertColors ( Colors::royalColors() );
      break;

    case 2:
      m_colorTable->insertColors ( Colors::rainbowColors() );
      break;

    default:
      m_colorTable->insertColors ( Colors::webColors() );
      break;
  }
}

ColorPicker::~ColorPicker()
{}
