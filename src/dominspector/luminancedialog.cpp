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

#include "luminancedialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

/*
Die relative Helligkeit irgendeines Punktes in einem Farbraum,
normalisiert zu 0 für das dunkelste Schwarz und 1 für das hellste Weiß.
Anmerkung 1: Für den sRGB-Farbraum wird die relative Luminanz einer Farbe
definiert als L = 0.2126 * R + 0.7152 * G + 0.0722 * B, wobei R, G und B definiert sind als:

wenn RsRGB <= 0.03928 dann R = RsRGB/12.92 sonst R = ((RsRGB+0.055)/1.055) ^ 2.4
wenn GsRGB <= 0.03928 dann G = GsRGB/12.92 sonst G = ((GsRGB+0.055)/1.055) ^ 2.4
wenn BsRGB <= 0.03928 dann B = BsRGB/12.92 sonst B = ((BsRGB+0.055)/1.055) ^ 2.4
und RsRGB, GsRGB, und BsRGB definiert sind als:

RsRGB = R8bit/255
GsRGB = G8bit/255
BsRGB = B8bit/255
*/

LuminanceDialog::LuminanceDialog ( const QWebElement &element, QWidget * parent )
    : KDialog ( parent )
    , htmlElement ( element )
    , valid ( false )
{
  setObjectName ( QLatin1String ( "LuminanceDialog" ) );
  setPlainCaption ( i18n ( "WCAG Relative Luminance" ) );
  setMinimumWidth ( 400 );
  setButtons ( KDialog::Ok );
  setSizeGripEnabled ( true );

  QGridLayout* gridLayout = new QGridLayout ( mainWidget() );
  gridLayout->addWidget ( new QLabel ( i18n ( "Foreground Color" ) ), 0, 0, 1, 1 );
  gridLayout->addWidget ( new QLabel ( i18n ( "Background Color" ) ), 0, 1, 1, 1 );

  QLabel* labelForeground = new QLabel ( mainWidget() );
  gridLayout->addWidget ( labelForeground, 1, 0, 1, 1 );

  QLabel* labelBackground = new QLabel ( mainWidget() );
  gridLayout->addWidget ( labelBackground, 1, 1, 1, 1 );

  QLabel* labelForegroundName = new QLabel ( mainWidget() );
  gridLayout->addWidget ( labelForegroundName, 2, 0, 1, 1 );

  QLabel* labelBackgroundName = new QLabel ( mainWidget() );
  gridLayout->addWidget ( labelBackgroundName, 2, 1, 1, 1 );

  mainWidget()->setLayout ( gridLayout );

  QString front = findForegroundColor().name();
  QString back = findBackgroundColor().name();
  if ( front.isEmpty() || back.isEmpty() )
    return;

  labelForeground->setStyleSheet ( QString::fromUtf8 ( "*{background-color:%1;}" ).arg ( front ) );
  labelBackground->setStyleSheet ( QString::fromUtf8 ( "*{background-color:%1;}" ).arg ( back ) );

  labelForegroundName->setText ( front );
  labelBackgroundName->setText ( back );

  valid = true;
}

const QColor LuminanceDialog::toColor ( const QString &str ) const
{
  QString buf ( str );
  QColor col ( Qt::transparent );
  QStringList rgb = buf.remove ( QRegExp ( "^rgb\\([\\s]*|[\\s]*\\)$", Qt::CaseSensitive ) ).split ( "," );
  buf.clear();

  if ( rgb.size() == 3 )
  {
    bool ok;
    QString rs = rgb.at ( 0 ).trimmed();
    int r = rs.toInt ( &ok );
    if ( ! ok )
      return col;

    QString gs = rgb.at ( 1 ).trimmed();
    int g = gs.toInt ( &ok );
    if ( ! ok )
      return col;

    QString bs = rgb.at ( 2 ).trimmed();
    int b = bs.toInt ( &ok );
    if ( ! ok )
      return col;

    col.setRgb ( r, g, b );
  }
  return col;
}

const QColor LuminanceDialog::findForegroundColor() const
{
  QString rgb = htmlElement.styleProperty ( QLatin1String ( "color" ), QWebElement::ComputedStyle );
  QColor col = toColor ( rgb );
  if ( col.isValid() )
    return col;
  else
    return QColor ( Qt::white );
}

const QColor LuminanceDialog::findBackgroundColor() const
{
  QColor col;
  QString rgb;
  QString property ( "background-color" );
  QString transparent ( "transparent" );
  rgb = htmlElement.styleProperty ( property, QWebElement::ComputedStyle );
  if ( rgb == transparent )
  {
    QWebElement sibling = htmlElement.parent();
    do
    {
      rgb = sibling.styleProperty ( property, QWebElement::ComputedStyle );
      if ( rgb != transparent )
        break;

      sibling = sibling.parent();
    }
    while ( ! sibling.isNull() );

    col = toColor ( rgb );
  }
  else
    col = toColor ( rgb );

  if ( col.isValid() )
    return col;
  else
    return QColor ( Qt::black );
}

bool LuminanceDialog::isValid()
{
  return valid;
}

LuminanceDialog::~LuminanceDialog()
{}
