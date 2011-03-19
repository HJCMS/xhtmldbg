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

#include "colorpicker.h"
#include "colors.h"
#include "colortable.h"
#include "grabberwindow.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ColorPicker::ColorPicker ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "colorpicker" ) );
  setWindowTitle ( i18n ( "Colors" ) );
  setMouseTracking ( false );

  QDBusConnection dbus = QDBusConnection::sessionBus();

  QWidget* layer = new QWidget ( this );
  layer->setObjectName ( QLatin1String ( "colorpicker.layer" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layer );
  verticalLayout->setObjectName ( QLatin1String ( "colorpicker.layer.vlayout" ) );

  // Farbpaletten Auswahl
  m_colorComboBox = new QComboBox ( layer );
  m_colorComboBox->setObjectName ( "colorpicker.layer.vlayout.combobox" );
  m_colorComboBox->insertItem ( 0, i18n ( "Web Colors" ) );
  m_colorComboBox->insertItem ( 1, i18n ( "Royal Colors" ) );
  m_colorComboBox->insertItem ( 2, i18n ( "Rainbow Colors" ) );
  m_colorComboBox->insertItem ( 3, i18n ( "Gray Colors" ) );
  m_colorComboBox->insertItem ( 4, i18n ( "Topographic Colors" ) );
  verticalLayout->addWidget ( m_colorComboBox, 0, Qt::AlignRight );

  // Farbpaletten Ausgeben
  m_colorTable = new ColorTable ( layer );
  m_colorTable->setMouseTracking ( true ); // wird für grabMouse benötigt
  m_colorTable->insertColors ( Colors::webColors() );
  verticalLayout->addWidget ( m_colorTable );

  // Vorschau Gruppe
  QGroupBox* groupBox1 = new QGroupBox ( i18n ( "Color preview" ), layer );
  verticalLayout->addWidget ( groupBox1 );

  QHBoxLayout* hLayout1 = new QHBoxLayout ( groupBox1 );
  groupBox1->setLayout ( hLayout1 );

  // Fraben Abgreifen
  m_grabberWindow = new GrabberWindow ( groupBox1 );
  hLayout1->addWidget ( m_grabberWindow );

  // Text Ausgabe
  QVBoxLayout* verticalBoxLayout = new QVBoxLayout;
  // Ausgabe Hex
  verticalBoxLayout->addWidget ( new QLabel ( i18n ( "hexcolor:" ), groupBox1 ) );
  m_hexEdit = new QLineEdit ( groupBox1 );
  verticalBoxLayout->addWidget ( m_hexEdit, 0, Qt::AlignLeft );

  // Ausgabe RGB
  verticalBoxLayout->addWidget ( new QLabel ( i18n ( "rgbcolor:" ), groupBox1 ) );
  m_rgbEdit = new QLineEdit ( groupBox1 );
  verticalBoxLayout->addWidget ( m_rgbEdit, 0, Qt::AlignLeft );
  hLayout1->addLayout ( verticalBoxLayout );

  // Farb-Vorschau
  m_preview = new QLabel ( groupBox1 );
  m_preview->setStyleSheet ( "*{border:1px ridge black;background-color:#FFFFFF;}" );
  m_preview->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
  hLayout1->addWidget ( m_preview );

  QLabel* hexinfo = new QLabel ( layer );
  hexinfo->setWordWrap ( true );
  hexinfo->setText ( i18n ( "There is a constraint on the color that it must have either 3 or 6 hex-digits (i.e., [0-9a-fA-F]) after the \"#\"; e.g., \"#000\" is OK, but \"#abcd\" is not." ) );
  verticalLayout->addWidget ( hexinfo );

  QSpacerItem* spacer = new QSpacerItem ( 2, 2, QSizePolicy::Preferred, QSizePolicy::Expanding );
  verticalLayout->addItem ( spacer );

  // Layout abschliessen
  layer->setLayout ( verticalLayout );
  setWidget ( layer );

  // D-Bus Registrierung
  dbus.registerObject ( "/Extension/ColorPicker", this, QDBusConnection::ExportScriptableContents );

  connect ( m_colorComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( colorMapChanged ( int ) ) );

  connect ( m_colorTable, SIGNAL ( colorChanged ( const QColor & ) ),
            this, SLOT ( colorChanged ( const QColor & ) ) );

  connect ( m_grabberWindow, SIGNAL ( recording ( bool ) ),
            this, SIGNAL ( recording ( bool ) ) );
}

void ColorPicker::findPixelColor ( const QPoint &p )
{
  QColor color ( Qt::transparent );
  QPixmap pixmap = QPixmap::grabWindow ( qApp->desktop()->winId(), p.x(), p.y(), 1, 1 );
  QImage img = pixmap.toImage();
  if ( img.isNull() )
  {
    m_hexEdit->clear();
    m_preview->setStyleSheet ( "*{border:1px ridge black;}" );
    m_rgbEdit->clear();
    return;
  }

  color.setRgba ( img.pixel ( 0, 0 ) );
  colorChanged ( color );
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

    case 3:
      m_colorTable->insertColors ( Colors::grayColors() );
      break;

    case 4:
      m_colorTable->insertColors ( Colors::topographicColors() );
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

/**
* Globale Maus Position
*/
void ColorPicker::pointerChanged ( const QPoint &p )
{
  findPixelColor ( p );
  m_grabberWindow->startRecording ( false );
  emit recording ( false );
}

/**
* Wenn Maustaste gedrückt wird und MouseTracking eingeschaltet ist!
* Ein Signal mit der aktuellen Position zurück geben!
* @see tapping
*/
void ColorPicker::mousePressEvent ( QMouseEvent * event )
{
  pointerChanged ( event->globalPos() );
  releaseMouse();
  setCursor ( Qt::ArrowCursor );
  QDockWidget::mousePressEvent ( event );
}

/**
* Aktiviere/Deaktiviere das abgreifen der Maus Position
* für die ermittlung der Farbe.
*/
void ColorPicker::tapping ( bool b )
{
  if ( b )
    grabMouse ( Qt::CrossCursor );

  m_grabberWindow->startRecording ( b );
}

ColorPicker::~ColorPicker()
{}
