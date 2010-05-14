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

#include "cssconfigure.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QIcon>
#include <QtGui/QPushButton>

CSSConfigure::CSSConfigure ( QWidget * parent, QSettings  * settings )
    : QDialog ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "cssconfigure" ) );
  setupUi ( this );

  // Das CSS-Profil für die Validierung. (Auswahl Box) Type String
  profileList << "css1" << "css2" << "css21" << "css3" << "svg" << "svgbasic" << "svgtiny" << "mobile" << "atsc-tv" << "tv";
  profileList.sort();
  css_profile->addItems ( profileList );

  // Die Ausgabesprache. (Auswahl Box) type String
  langList << "en" << "fr" << "it" << "ko" << "ja" << "es" << "zh-cn" << "nl" << "de";
  langList.sort();
  css_lang->addItems ( langList );

  // Menge der ausgegebenen Warnungen. (Auswahl Box) Type INTEGER
  warningList << trUtf8 ( "no" ) << trUtf8 ( "less" ) << trUtf8 ( "more" );
  css_warning->addItems ( warningList );

  // Das Medium für die Validierung. (Auswahl Box) Type String
  mediumList << "all" << "braille" << "embossed" << "handheld" << "print" << "projection" << "screen" << "speech" << "tty" << "tv";
  mediumList.sort();
  css_medium->addItems ( mediumList );

  QPushButton* ok = buttonBox->addButton ( trUtf8 ( "Save && Exit" ), QDialogButtonBox::ActionRole );
  ok->setIcon ( QIcon::fromTheme ( QLatin1String ( "dialog-ok-apply" ) ) );
  connect ( ok, SIGNAL ( clicked() ), this, SLOT ( saveAndExit() ) );
  connect ( buttonBox, SIGNAL ( rejected() ), this, SLOT ( reject() ) );
  if ( loadSettings() )
    update();
}

void CSSConfigure::setComboBoxItem ( const QString &object, const QString &value )
{
  QComboBox* box = findChild<QComboBox*> ( object );
  if ( ! box )
    return;

  int index = box->findText ( value, Qt::MatchExactly );
  if ( index != -1 )
    box->setCurrentIndex ( index );
}

bool CSSConfigure::loadSettings()
{
  setComboBoxItem ( "css_profile", cfg->value ( "css_profile", "css21" ).toString() );
  setComboBoxItem ( "css_lang", cfg->value ( "css_lang", "en" ).toString() );
  setComboBoxItem ( "css_medium", cfg->value ( "css_medium", "all" ).toString() );
  css_warning->setCurrentIndex ( cfg->value ( "css_warning", 1 ).toUInt() );
  return true;
}

bool CSSConfigure::saveSettings()
{
  cfg->setValue ( "css_profile", css_profile->currentText() );
  cfg->setValue ( "css_lang", css_lang->currentText() );
  cfg->setValue ( "css_medium", css_medium->currentText() );
  cfg->setValue ( "css_warning", css_warning->currentIndex() );
  return true;
}

void CSSConfigure::saveAndExit()
{
  if ( saveSettings() )
    accept();
}

CSSConfigure::~CSSConfigure()
{}
