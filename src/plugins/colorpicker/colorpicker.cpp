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
#include "watcher.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

ColorPicker::ColorPicker ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "colorpicker" ) );
  setWindowTitle ( trUtf8 ( "Colors" ) );

  QWidget* layer = new QWidget ( this );
  layer->setObjectName ( QLatin1String ( "colorpicker.layer" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layer );
  verticalLayout->setObjectName ( QLatin1String ( "colorpicker.layer.vlayout" ) );

  // Farbpaletten Auswahl
  m_colorComboBox = new QComboBox ( layer );
  m_colorComboBox->setObjectName ( "colorpicker.layer.vlayout.combobox" );
  m_colorComboBox->insertItem ( 0, trUtf8 ( "Web Colors" ) );
  m_colorComboBox->insertItem ( 1, trUtf8 ( "Royal Colors" ) );
  m_colorComboBox->insertItem ( 2, trUtf8 ( "Rainbow Colors" ) );
  verticalLayout->addWidget ( m_colorComboBox, 0, Qt::AlignRight );

  // Farbpaletten Ausgeben
  m_colorTable = new ColorTable ( layer );
  m_colorTable->setObjectName ( "colorpicker.layer.vlayout.colortable" );
  m_colorTable->insertColors ( Colors::webColors() );
  verticalLayout->addWidget ( m_colorTable );

  // Vorschau Gruppe
  QGroupBox* groupBox = new QGroupBox ( trUtf8( "Color preview" ), layer );
  groupBox->setObjectName ( "colorpicker.layer.vlayout.groupbox" );
  verticalLayout->addWidget ( groupBox );

  QHBoxLayout* horizontalLayout = new QHBoxLayout ( groupBox );
  horizontalLayout->setObjectName ( "colorpicker.layer.vlayout.groupbox.hlayout" );
  groupBox->setLayout ( horizontalLayout );

  // Ausgabe Hex
  m_hexEdit = new QLineEdit ( groupBox );
  m_hexEdit->setObjectName ( "colorpicker.layer.vlayout.groupbox.hlayout.hexedit" );
  horizontalLayout->addWidget ( m_hexEdit, 0, Qt::AlignLeft );

  // Farb-Vorschau
  m_preview = new QLabel ( groupBox );
  m_preview->setObjectName ( "colorpicker.layer.vlayout.groupbox.hlayout.preview" );
  m_preview->setStyleSheet ( "*{border:1px ridge black;}" );
  horizontalLayout->addWidget ( m_preview );

  // Ausgabe RGB
  m_rgbEdit = new QLineEdit ( groupBox );
  m_rgbEdit->setObjectName ( "colorpicker.layer.vlayout.groupbox.hlayout.rgbedit" );
  horizontalLayout->addWidget ( m_rgbEdit, 0, Qt::AlignRight );

  // Abgreifen von Farben aus dem QWebView Fenster
  m_watcher = new Watcher ( layer );
  m_watcher->setObjectName ( "colorpicker.layer.vlayout.watcher" );
  verticalLayout->addWidget ( m_watcher );

  QSpacerItem* spacer = new QSpacerItem ( 2, 2, QSizePolicy::Preferred, QSizePolicy::Expanding );
  verticalLayout->addItem ( spacer );

  // Layout abschliessen
  layer->setLayout ( verticalLayout );
  setWidget ( layer );

  connect ( m_colorComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( colorMapChanged ( int ) ) );

  connect ( m_colorTable, SIGNAL ( colorChanged ( const QColor & ) ),
            this, SLOT ( colorChanged ( const QColor & ) ) );
}

/**
* Farb-Palette aufrufen
*/
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

/**
* gewählte Farbe ausgeben
*/
void ColorPicker::colorChanged ( const QColor &c )
{
  // Hex Text einfügen
  m_hexEdit->setText ( c.name() );

  // RGB Text einfügen
  QString rgb = QString ( "rgb(%1,%2,%3)" ).arg (
                    QString::number ( c.red() ),
                    QString::number ( c.green() ),
                    QString::number ( c.blue() )
                );
  m_rgbEdit->setText ( rgb );

  // Vorschau erzeugen
  m_preview->setStyleSheet ( QString ( "*{background:%1; border:1px ridge black;}" ).arg ( c.name() ) );
}

ColorPicker::~ColorPicker()
{}
