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

#include "fontstyle.h"
#include "fontsizeadjust.h"
#include "fontweight.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QFontMetrics>
#include <QtGui/QGroupBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KDialog>
#include <KDE/KLocale>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

FontStyle::FontStyle ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "FontStyle" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  m_fontWidget = new KFontDialog ( this );
  m_fontWidget->setWindowFlags ( Qt::Widget );
  m_fontWidget->setButtons ( KDialog::None );
  vLayout->addWidget ( m_fontWidget );

  QGroupBox* groupBox = new QGroupBox ( i18n ( "CSS-Extras" ), this );
  QGridLayout* gridLayout = new QGridLayout ( groupBox );
  gridLayout->addWidget ( new QLabel ( i18n ( "font-size-adjust" ), groupBox ), 0, 0, 1, 1 );
  m_fontSizeAdjust = new FontSizeAdjust ( groupBox );
  gridLayout->addWidget ( m_fontSizeAdjust, 0, 1, 1, 1 );
  gridLayout->addWidget ( new QLabel ( i18n ( "CSS3" ), groupBox ), 0, 2, 1, 1 );

  gridLayout->addWidget ( new QLabel ( i18n ( "font-weigth" ), groupBox ), 1, 0, 1, 1 );
  m_fontWeight = new  FontWeight ( groupBox );
  gridLayout->addWidget ( m_fontWeight, 1, 1, 1, 1 );
  gridLayout->addWidget ( new QLabel ( i18n ( "CSS3" ), groupBox ), 1, 2, 1, 1 );
  groupBox->setLayout ( gridLayout );

  vLayout->addWidget ( groupBox );

  setLayout ( vLayout );

  connect ( m_fontWidget, SIGNAL ( fontSelected ( const QFont & ) ),
            this, SIGNAL ( fontChanged ( const QFont & ) ) );

  connect ( m_fontSizeAdjust, SIGNAL ( valueChanged ( double ) ),
            this, SIGNAL ( fontAdjust ( double ) ) );

  connect ( m_fontWeight, SIGNAL ( weightChanged ( const QVariant & ) ),
            this, SIGNAL ( fontWeight ( const QVariant & ) ) );
}

/**
* http://www.w3.org/TR/css3-fonts/
*/
const QFont FontStyle::fetchFont ( const QString &family, const QString &value ) const
{
  QVariant size;
  QString buffer ( value );
  QFont fo = m_fontWidget->font();
  fo.setFamily ( family );

  int defaultSize = QWebSettings::globalSettings()->fontSize ( QWebSettings::DefaultFontSize );
  if ( value.contains ( QRegExp ( "[\\d]+(pc)$", Qt::CaseInsensitive ) ) )
  {
    /* Steht für Pica. Typografische Maßeinheit. 1 Pica entspricht 12 Punkt. */
    size = buffer.remove ( QRegExp ( "pc$", Qt::CaseInsensitive ) );
    fo.setPointSize ( ( size.toUInt() * 12 ) );
  }
  else if ( value.contains ( QRegExp ( "[\\d]+(px)$", Qt::CaseInsensitive ) ) )
  {
    /* Steht für Pixel. Abhängig von der Pixeldichte des Ausgabegeräts,
    * relativ also von Ausgabegerät zu Ausgabegerät, absolut dagegen auf
    * ein und das selbe Ausgabegerät bezogen. */
    size = buffer.remove ( QRegExp ( "px$", Qt::CaseInsensitive ) );
    fo.setPixelSize ( size.toUInt() );
  }
  else if ( value.contains ( QRegExp ( "[\\d]+(pt)$", Qt::CaseInsensitive ) ) )
  {
    /* Steht für Punkt. Typografische Maßeinheit. 1 Punkt entspricht 1/72 Inches. */
    size = buffer.remove ( QRegExp ( "pt$", Qt::CaseInsensitive ) );
    fo.setPointSize ( size.toUInt() );
  }
  else if ( value.contains ( QRegExp ( "[\\d]+(em)$", Qt::CaseInsensitive ) ) )
  {
    /* Steht für bezogen auf die Schriftgröße des Elements.
    * Ausnahme: wenn die font-size-Eigenschaft (also die Schriftgröße) selbst mit dieser
    * Maßangabe gesetzt wird, steht diese für bezogen auf die Schriftgröße des Elternelements. */
    size = buffer.remove ( QRegExp ( "em$", Qt::CaseInsensitive ) );
    if ( size.toString().contains ( "." ) )
      fo.setPointSize ( ( defaultSize * size.toFloat() ) );
    else
      fo.setPointSize ( ( defaultSize * size.toUInt() ) );
  }
  else if ( value.contains ( "smaller", Qt::CaseInsensitive ) )
  {
    // Relativ = "smaller"
    fo.setPointSize ( ( defaultSize - 1 ) );
  }
  else if ( value.contains ( "larger", Qt::CaseInsensitive ) )
  {
    // Relativ = "larger"
    fo.setPointSize ( ( defaultSize + 1 ) );
  }
  else if ( value.contains ( "xx-small", Qt::CaseInsensitive ) )
  {
    // Absolut faktor = 3/5
    fo.setPointSize ( ( ( defaultSize / 5 ) * 3 ) );
  }
  else if ( value.contains ( "x-small", Qt::CaseInsensitive ) )
  {
    // Absolut faktor = 3/4
    fo.setPointSize ( ( ( defaultSize / 4 ) * 3 ) );
  }
  else if ( value.contains ( "small", Qt::CaseInsensitive ) )
  {
    // Absolut faktor = 8/9
    fo.setPointSize ( ( ( defaultSize / 9 ) * 8 ) );
  }
  else if ( value.contains ( "xx-large", Qt::CaseInsensitive ) )
  {
    // Absolut faktor = 2/1
    fo.setPointSize ( ( defaultSize * 2 ) );
  }
  else // Absolut e.q. medium|normal
    fo.setPointSize ( defaultSize );

  buffer.clear();
  return fo;
}

const QStringList FontStyle::getAttributes() const
{
  QStringList list;
  // list << "font-effect" << "font-emphasize" << "font-emphasize-position";
  // list << "font-emphasize-style" << "font-smooth";
  list << "font-family" << "font-size" << "font-size-adjust";
  list << "font-style" << "font-variant" << "font-weight";
  return list;
}

/**
* Zuerst alle relevanten font Prädikate in den Hash schreiben.
*/
void FontStyle::readFontAttributes ( const QWebElement &element )
{
  hash.clear();
  foreach ( QString p, getAttributes() )
  {
    QString value = element.styleProperty ( p, QWebElement::ComputedStyle );
    if ( value.isEmpty() )
      continue;

    // qDebug() << Q_FUNC_INFO << p << value;
    hash.insert ( p, value );
  }

  QFont f = fetchFont ( hash.value ( "font-family" ).toString(), hash.value ( "font-size" ).toString() );
  if ( hash.value ( "font-variant" ).toString().contains ( "small-caps", Qt::CaseInsensitive ) )
    f.setCapitalization ( QFont::SmallCaps );

  if ( hash.value ( "font-weight" ).toString().contains ( "bold", Qt::CaseInsensitive ) )
    f.setWeight ( QFont::Bold );

  m_fontWidget->setFont ( f );
}

FontStyle::~FontStyle()
{
  hash.clear();
}
