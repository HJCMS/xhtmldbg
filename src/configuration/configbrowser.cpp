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

#include "configbrowser.h"

/* QtCore */
#include <QtCore/QMap>
#include <QtCore/QMapIterator>

/* QtGui */
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QVBoxLayout>

/**
* In dieser Map werden alle Auswahl Boxen registriert!
* Dabei ist der Objektname identisch mit dem Parameter
* der Einstellungen. Siehe auch @ref QWebSettings
* Der Boolische Wert ist der Standardwert.
*/
static inline const QMap<QString,bool> settingsMap()
{
  QMap<QString,bool> map;
  map["DoNotBufferUploadDataAttribute"] = true;
  map["HttpPipeliningAllowedAttribute"] = true;
  map["HttpPipeliningWasUsedAttribute"] = true;
  map["AutoLoadImages"] = true;
  map["JavascriptEnabled"] = true;
  map["PluginsEnabled"] = false;
  map["JavaEnabled"] = true;
  map["ZoomTextOnly"] = true;
  map["DnsPrefetchEnabled"] = true;
  map["JavascriptCanOpenWindows"] = false;
  map["JavascriptCanAccessClipboard"] = false;
  map["PrintElementBackgrounds"] = false;
  return map;
}

ConfigBrowser::ConfigBrowser ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Browser Settings" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_browser" ) );
  setNotice ( true );
  setCheckable ( false );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );

  QCheckBox* cb1 = new QCheckBox ( trUtf8 ( "Do Not Buffer Upload Data when doing a HTTP POST request." ), centralWidget );
  cb1->setObjectName ( QLatin1String ( "DoNotBufferUploadDataAttribute" ) );
  verticalLayout->addWidget ( cb1 );

  QCheckBox* cb2 = new QCheckBox ( trUtf8 ( "Allow HTTP pipelining with requests." ), centralWidget );
  cb2->setObjectName ( QLatin1String ( "HttpPipeliningAllowedAttribute" ) );
  verticalLayout->addWidget ( cb2 );

  QCheckBox* cb3 = new QCheckBox ( trUtf8 ( "Indicates whether the HTTP Pipelining was used for receiving data Requests." ), centralWidget );
  cb3->setObjectName ( QLatin1String ( "HttpPipeliningWasUsedAttribute" ) );
  verticalLayout->addWidget ( cb3 );

  QCheckBox* cb4 = new QCheckBox ( trUtf8 ( "Enable AutoLoad Images" ), centralWidget );
  cb4->setObjectName ( QLatin1String ( "AutoLoadImages" ) );
  verticalLayout->addWidget ( cb4 );

  QCheckBox* cb5 = new QCheckBox ( trUtf8 ( "Enable Javascript" ), centralWidget );
  cb5->setObjectName ( QLatin1String ( "JavascriptEnabled" ) );
  verticalLayout->addWidget ( cb5 );

  QCheckBox* cb6 = new QCheckBox ( trUtf8 ( "Enable Plugins" ), centralWidget );
  cb6->setObjectName ( QLatin1String ( "PluginsEnabled" ) );
  verticalLayout->addWidget ( cb6 );

  QCheckBox* cb7 = new QCheckBox ( trUtf8 ( "Enable Java" ), centralWidget );
  cb7->setObjectName ( QLatin1String ( "JavaEnabled" ) );
  verticalLayout->addWidget ( cb7 );

  QCheckBox* cb8 = new QCheckBox ( trUtf8 ( "Zoom only text" ), centralWidget );
  cb8->setObjectName ( QLatin1String ( "ZoomTextOnly" ) );
  verticalLayout->addWidget ( cb8 );

  QCheckBox* cb9 = new QCheckBox ( trUtf8 ( "Fetch DNS entries to speed up browsing." ), centralWidget );
  cb9->setObjectName ( QLatin1String ( "DnsPrefetchEnabled" ) );
  verticalLayout->addWidget ( cb9 );

  QCheckBox* cb10 = new QCheckBox ( trUtf8 ( "JavaScript programs can open new windows." ), centralWidget );
  cb10->setObjectName ( QLatin1String ( "JavascriptCanOpenWindows" ) );
  verticalLayout->addWidget ( cb10 );

  QCheckBox* cb11 = new QCheckBox ( trUtf8 ( "JavaScript programs can read or write to the clipboard." ), centralWidget );
  cb11->setObjectName ( QLatin1String ( "JavascriptCanAccessClipboard" ) );
  verticalLayout->addWidget ( cb11 );

  QCheckBox* cb12 = new QCheckBox ( trUtf8 ( "The background color and images are also drawn when the page is printed." ), centralWidget );
  cb12->setObjectName ( QLatin1String ( "PrintElementBackgrounds" ) );
  verticalLayout->addWidget ( cb12 );

  /** Bitte nicht vergessen die Objektnamen in die Map ein zu tragen! */
//   QCheckBox* cb = new QCheckBox ( trUtf8 ( "" ), centralWidget );
//   cb->setObjectName ( QLatin1String ( "" ) );
//   verticalLayout->addWidget ( cb );

  // TODO
  // QWebSettings::DefaultFontSize => DefaultFontSize
  // QWebSettings::DefaultFixedFontSize => DefaultFixedFontSize

  centralWidget->setLayout ( verticalLayout );

  // Registriere alle Auswahlboxen für setWindowModified
  registerCheckBoxes();
}

void ConfigBrowser::registerCheckBoxes()
{
  int id = 0;
  QButtonGroup* btnGroup = new QButtonGroup ( centralWidget );
  btnGroup->setObjectName ( QLatin1String ( "config_page_browser_checkbox_group" ) );
  btnGroup->setExclusive ( false ); // Keine Radio Gruppe

  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    btnGroup->addButton ( findChild<QCheckBox*> ( it.key() ), id++ );
  }

  connect ( btnGroup, SIGNAL ( buttonClicked ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );
}

void ConfigBrowser::itemClicked ( int )
{
  emit modified ( true );
}

void ConfigBrowser::load ( QSettings * cfg )
{
  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    findChild<QCheckBox*> ( it.key() )->setChecked ( cfg->value ( it.key(), it.value() ).toBool() );
  }
}

void ConfigBrowser::save ( QSettings * cfg )
{
  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    cfg->setValue ( it.key(), findChild<QCheckBox*> ( it.key() )->isChecked() );
  }
}

void ConfigBrowser::defaults()
{
  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    findChild<QCheckBox*> ( it.key() )->setChecked ( it.value() );
  }
}

ConfigBrowser::~ConfigBrowser()
{}
