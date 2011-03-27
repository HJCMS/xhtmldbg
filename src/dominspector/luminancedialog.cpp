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

/* std */
#include <cmath>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QGridLayout>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

/**
* \link http://www.w3.org/TR/2007/WD-WCAG20-TECHS-20070517/Overview.html#G18
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

  int row = 0;
  gridLayout->addWidget ( new QLabel ( i18n ( "Foreground Color" ) ), row, 0, 1, 1 );
  gridLayout->addWidget ( new QLabel ( i18n ( "Background Color" ) ), row, 1, 1, 1 );

  m_foregroundColor = new QLabel ( mainWidget() );
  m_foregroundColor->setFrameStyle ( QFrame::StyledPanel );
  gridLayout->addWidget ( m_foregroundColor, ++row, 0, 1, 1 );

  m_backgroundColor = new QLabel ( mainWidget() );
  m_backgroundColor->setFrameStyle ( QFrame::StyledPanel );
  gridLayout->addWidget ( m_backgroundColor, row, 1, 1, 1 );

  m_foregroundName = new QLabel ( mainWidget() );
  gridLayout->addWidget ( m_foregroundName, ++row, 0, 1, 1 );

  m_backgroundName = new QLabel ( mainWidget() );
  gridLayout->addWidget ( m_backgroundName, row, 1, 1, 1 );

  // Contrast Ratio
  m_foregroundContrastRatio = new QLabel ( mainWidget() );
  m_foregroundContrastRatio->setToolTip ( i18n ( "Contrast Ratio" ) );
  gridLayout->addWidget ( m_foregroundContrastRatio, ++row, 0, 1, 1 );

  m_backgroundContrastRatio = new QLabel ( mainWidget() );
  m_backgroundContrastRatio->setToolTip ( i18n ( "Contrast Ratio" ) );
  gridLayout->addWidget ( m_backgroundContrastRatio, row, 1, 1, 1 );

  m_messanger = new QComboBox ( mainWidget() );
  m_messanger->insertItem ( 0, KIcon ( "task-complete" ), i18n ( "The difference in colour between the two colours is sufficient." ) );
  m_messanger->insertItem ( 1, KIcon ( "task-attention" ), i18n ( "The difference in colour between the two colours is not sufficient." ) );
  m_messanger->insertItem ( 2, KIcon ( "task-reminder" ), i18n ( "The difference in colour between the two colours is not sufficient." ) );
  gridLayout->addWidget ( m_messanger, ++row, 0, 1, 2 );

  m_differenceBar = new QProgressBar ( mainWidget() );
  m_differenceBar->setFormat ( i18n ( "Colour Difference %p%" ) );
  m_differenceBar->setTextVisible ( true );
  // Der Maximale Differenz Wert im RGB Bereich liegt bei Schwarz und Weiss auf 765
  m_differenceBar->setRange ( 0, 765 );
  m_differenceBar->setValue ( 0 );
  gridLayout->addWidget ( m_differenceBar, ++row, 0, 1, 2 );

  m_brightnessBar = new QProgressBar ( mainWidget() );
  m_brightnessBar->setFormat ( i18n ( "Colour Brightness %p%" ) );
  m_brightnessBar->setTextVisible ( true );
  m_brightnessBar->setRange ( 0, 125 );
  m_brightnessBar->setValue ( 0 );
  gridLayout->addWidget ( m_brightnessBar, ++row, 0, 1, 2 );

  mainWidget()->setLayout ( gridLayout );
}

/**
* Ermittle den relativen Helligkeit Wert eines Punktes in einem Farbraum
* \note 0 für das dunkelste Schwarz und 1 für das hellste Weiß!
*/
double LuminanceDialog::contrastRatio ( const QColor &col ) const
{
  double R,G,B;
  double RsRGB = ( col.red() / 255.0 );
  if ( RsRGB <= 0.03928 )
    R = ( RsRGB / 12.92 );
  else
    R = pow ( ( ( RsRGB + 0.055 ) / 1.055 ), 2.4 );

  double GsRGB = ( col.green() / 255.0 );
  if ( GsRGB <= 0.03928 )
    G = ( GsRGB / 12.92 );
  else
    G = pow ( ( ( GsRGB + 0.055 ) / 1.055 ), 2.4 );

  double BsRGB = ( col.blue() / 255.0 );
  if ( BsRGB <= 0.03928 )
    B = ( BsRGB / 12.92 );
  else
    B = pow ( ( ( BsRGB + 0.055 ) / 1.055 ), 2.4 );

  double L = ( ( 0.2126 * R ) + ( 0.7152 * G ) + ( 0.0722 * B ) );

  /*
    qDebug() << Q_FUNC_INFO
    << " r:" << col.red() << RsRGB << R
    << " g:" << col.green() << GsRGB << G
    << " b:" << col.blue() << BsRGB << B
    << " L=" << L;
  */
  return L;
}

/**
* Zerlege eine "rgb(\d{1,3},\d{1,3},\d{1,3})" Zeichenkette
* in seine Bestanteile und Konvertieren zu einem QColor
*/
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

/**
* Suche im Element mit dem StyleSheet \param property nach den
* Werten, wenn im aktuell ausgewähltem Element diese \b nicht vorhanden
* sind gehe Schrittweise weiter zum Eltern Element und steige bei erfolg
* mit dem gefunden StyleSheet aus.
* \note bei einem Fehler wird Schwarz zurück gegeben!
*/
const QColor LuminanceDialog::getStyleColor ( const QString &property ) const
{
  QColor col;
  QString rgb;
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

/** Set den ProgressBar und die ComboBox für die Meldungen */
void LuminanceDialog::setRange ( const QColor &fr, const QColor &bg )
{
  /* The difference between the background colour and the foreground colour should be greater than 500. */
  int diff = ( qMax ( fr.red(), bg.red() ) - qMin ( fr.red(), bg.red() ) ) + ( qMax ( fr.green(), bg.green() ) - qMin ( fr.green(), bg.green() ) ) + ( qMax ( fr.blue(), bg.blue() ) - qMin ( fr.blue(), bg.blue() ) );
  // qDebug() << Q_FUNC_INFO << "Colour Difference:" << diff;
  if ( diff >= m_differenceBar->maximum() )
  {
    m_messanger->setCurrentIndex ( 0 );
    m_differenceBar->setValue ( m_differenceBar->maximum() );
  }
  else if ( diff <= m_differenceBar->maximum() && diff >= 0 )
    m_differenceBar->setValue ( diff );
  else
    m_differenceBar->setValue ( 0 );

  if ( diff >= ( ( m_differenceBar->maximum() / 4 ) * 3 ) ) // 3¼ von Maximum
    m_messanger->setCurrentIndex ( 0 );
  else if ( diff >= ( m_differenceBar->maximum() / 2 ) ) // ½ von Maximum
    m_messanger->setCurrentIndex ( 1 );
  else
    m_messanger->setCurrentIndex ( 2 );

  /* The difference between the background brightness, and the foreground brightness should be greater than 125. */
  double fr_bright = ( ( ( fr.red() * 299 ) + ( fr.green() * 587 ) + ( fr.blue() * 114 ) ) / 1000 );
  double bg_bright = ( ( ( bg.red() * 299 ) + ( bg.green() * 587 ) + ( bg.blue() * 114 ) ) / 1000 );
  int bright = 0;
  if ( bg_bright == 255 && fr_bright == 0 )
    bright = m_brightnessBar->maximum();
  else if ( bg_bright == 0 && fr_bright == 255 )
    bright = m_brightnessBar->maximum();
  else if ( bg_bright > fr_bright )
    bright = ceil ( fmod ( bg_bright, fr_bright ) );
  else
    bright = ceil ( fmod ( fr_bright, bg_bright ) );

  // qDebug() << Q_FUNC_INFO << bg_bright << fr_bright << bright;
  if ( bright >= 125.0 )
    m_brightnessBar->setValue ( 125 );
  else
    m_brightnessBar->setValue ( ( 125 - bright ) );

  // Contrast Ratio
  m_foregroundContrastRatio->setText ( QString::number ( contrastRatio ( fr ) ) );
  m_backgroundContrastRatio->setText ( QString::number ( contrastRatio ( bg ) ) );
}

bool LuminanceDialog::isValid()
{
  QColor front = getStyleColor ( "color" );
  QColor back = getStyleColor ( "background-color" );
  if ( ! front.isValid() || ! back.isValid() )
    return false;

  m_foregroundColor->setStyleSheet ( QString::fromUtf8 ( "*{background-color:%1;}" ).arg ( front.name() ) );
  m_backgroundColor->setStyleSheet ( QString::fromUtf8 ( "*{background-color:%1;}" ).arg ( back.name() ) );

  QString frontRGB = QString::fromUtf8 ( "rgb(%1,%2,%3)" )
                     .arg ( QString::number ( front.red() ), QString::number ( front.green() ), QString::number ( front.blue() ) );
  QString colorText = QString::fromUtf8 ( "%1 %2" ).arg ( front.name(), frontRGB );
  m_foregroundName->setText ( colorText );

  QString backRGB = QString::fromUtf8 ( "rgb(%1,%2,%3)" )
                    .arg ( QString::number ( back.red() ), QString::number ( back.green() ), QString::number ( back.blue() ) );
  QString backText = QString::fromUtf8 ( "%1 %2" ).arg ( back.name(), backRGB );
  m_backgroundName->setText ( backText );

  setRange ( front, back );
  return true;
}

LuminanceDialog::~LuminanceDialog()
{}
