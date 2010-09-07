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
#include "featurebox.h"

/* QtCore */
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/**
* In dieser Map werden alle Auswahl Boxen registriert!
* Dabei ist der Objektname identisch mit dem Parameter
* der Einstellungen. Siehe auch @ref QWebSettings
* Der Boolische Wert ist der Standardwert.
* http://doc.qt.nokia.com/main-snapshot/qwebsettings.html#WebAttribute-enum
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
  map["LocalContentCanAccessRemoteUrls"] = true;
  map["LocalContentCanAccessFileUrls"] = false;
  map["XSSAuditingEnabled"] = false;
  map["AcceleratedCompositingEnabled"] = true;
  map["TiledBackingStoreEnabled"] = false;
  return map;
}

ConfigBrowser::ConfigBrowser ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Browser Settings" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_browser" ) );
  setNotice ( true );
  setCheckable ( false );

  QSizePolicy sizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
  sizePolicy.setHorizontalStretch ( 0 );
  sizePolicy.setVerticalStretch ( 0 );

  Qt::Alignment labelAlign = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );

  FeatureBox* cb1 = new FeatureBox ( trUtf8 ( "Do Not Buffer Upload Data when doing a HTTP POST request." ), centralWidget );
  cb1->setObjectName ( QLatin1String ( "DoNotBufferUploadDataAttribute" ) );
  verticalLayout->addWidget ( cb1 );

  FeatureBox* cb2 = new FeatureBox ( trUtf8 ( "Allow HTTP pipelining with requests." ), centralWidget );
  cb2->setObjectName ( QLatin1String ( "HttpPipeliningAllowedAttribute" ) );
  verticalLayout->addWidget ( cb2 );

  FeatureBox* cb3 = new FeatureBox ( trUtf8 ( "Indicates whether the HTTP Pipelining was used for receiving data Requests." ), centralWidget );
  cb3->setObjectName ( QLatin1String ( "HttpPipeliningWasUsedAttribute" ) );
  verticalLayout->addWidget ( cb3 );

  FeatureBox* cb4 = new FeatureBox ( trUtf8 ( "Enable AutoLoad Images" ), centralWidget );
  cb4->setObjectName ( QLatin1String ( "AutoLoadImages" ) );
  verticalLayout->addWidget ( cb4 );

  FeatureBox* cb5 = new FeatureBox ( trUtf8 ( "Enable Javascript" ), centralWidget );
  cb5->setObjectName ( QLatin1String ( "JavascriptEnabled" ) );
  verticalLayout->addWidget ( cb5 );

  FeatureBox* cb6 = new FeatureBox ( trUtf8 ( "Enable Plugins" ), centralWidget );
  cb6->setObjectName ( QLatin1String ( "PluginsEnabled" ) );
  verticalLayout->addWidget ( cb6 );

  FeatureBox* cb7 = new FeatureBox ( trUtf8 ( "Enable Java" ), centralWidget );
  cb7->setObjectName ( QLatin1String ( "JavaEnabled" ) );
  verticalLayout->addWidget ( cb7 );

  FeatureBox* cb8 = new FeatureBox ( trUtf8 ( "Zoom only text" ), centralWidget );
  cb8->setObjectName ( QLatin1String ( "ZoomTextOnly" ) );
  verticalLayout->addWidget ( cb8 );

  FeatureBox* cb9 = new FeatureBox ( trUtf8 ( "Fetch DNS entries to speed up browsing." ), centralWidget );
  cb9->setObjectName ( QLatin1String ( "DnsPrefetchEnabled" ) );
  verticalLayout->addWidget ( cb9 );

  FeatureBox* cb10 = new FeatureBox ( trUtf8 ( "JavaScript programs can open new windows." ), centralWidget );
  cb10->setObjectName ( QLatin1String ( "JavascriptCanOpenWindows" ) );
  verticalLayout->addWidget ( cb10 );

  FeatureBox* cb11 = new FeatureBox ( trUtf8 ( "JavaScript programs can read or write to the clipboard." ), centralWidget );
  cb11->setObjectName ( QLatin1String ( "JavascriptCanAccessClipboard" ) );
  verticalLayout->addWidget ( cb11 );

  FeatureBox* cb12 = new FeatureBox ( trUtf8 ( "The background color and images are also drawn when the page is printed." ), centralWidget );
  cb12->setObjectName ( QLatin1String ( "PrintElementBackgrounds" ) );
  verticalLayout->addWidget ( cb12 );

  FeatureBox* cb13 = new FeatureBox ( trUtf8 ( "Locally loaded documents are allowed to access remote urls." ), centralWidget );
  cb13->setObjectName ( QLatin1String ( "LocalContentCanAccessRemoteUrls" ) );
  verticalLayout->addWidget ( cb13 );

  FeatureBox* cb14 = new FeatureBox ( trUtf8 ( "Locally loaded documents are allowed to access other local urls." ), centralWidget );
  cb14->setObjectName ( QLatin1String ( "LocalContentCanAccessFileUrls" ) );
  verticalLayout->addWidget ( cb14 );

  FeatureBox* cb15 = new FeatureBox ( trUtf8 ( "Specifies whether load requests should be monitored for cross-site scripting attempts. Suspicious scripts will be blocked and reported in the inspector's JavaScript console. Warning: To enabe this feature might have an impact on performance." ), centralWidget );
  cb15->setObjectName ( QLatin1String ( "XSSAuditingEnabled" ) );
  verticalLayout->addWidget ( cb15 );

  FeatureBox* cb16 = new FeatureBox ( trUtf8 ( "If enabled - CSS animations of the transform and opacity properties will be rendered\nby composing the cached content of the animated elements." ), centralWidget );
  cb16->setObjectName ( QLatin1String ( "AcceleratedCompositingEnabled" ) );
  verticalLayout->addWidget ( cb16 );

  FeatureBox* cb17 = new FeatureBox ( trUtf8 ( "This setting enables the tiled backing store feature for Graphics rendering. When enabled, the web page contents in and around the current visible area is speculatively cached to bitmap tiles. The Image content automatically kept in sync with the web page as it changes. It does not work well with contents using CSS fixed positioning. This property allows application to temporarily freeze the contents of the backing store." ), centralWidget );
  cb17->setObjectName ( QLatin1String ( "TiledBackingStoreEnabled" ) );
  verticalLayout->addWidget ( cb17 );

  /** Bitte nicht vergessen die Objektnamen in die Map ein zu tragen! */
  // Template:
  // FeatureBox* cb = new FeatureBox ( trUtf8 ( "" ), centralWidget );
  // cb->setObjectName ( QLatin1String ( "" ) );
  // verticalLayout->addWidget ( cb );

  // Registriere alle verwendeten Auswahlboxen für setWindowModified
  registerCheckBoxes();

  // Font Einstellungen
  QGroupBox* fontGroup = new QGroupBox ( trUtf8 ( "Font Settings" ), centralWidget );
  fontGroup->setObjectName ( QLatin1String ( "config_page_browser_font_group" ) );
  fontGroup->setFlat ( true );

  QHBoxLayout* horizontalLayout = new QHBoxLayout ( fontGroup );
  horizontalLayout->setObjectName ( QLatin1String ( "config_page_browser_font_grid_layout" ) );

  QLabel* label_df = new QLabel ( trUtf8 ( "Default Font Size:" ), fontGroup );
  label_df->setAlignment ( labelAlign );
  label_df->setSizePolicy ( sizePolicy );
  horizontalLayout->addWidget ( label_df );

  DefaultFontSize = new QSpinBox ( fontGroup );
  DefaultFontSize->setObjectName ( QLatin1String ( "DefaultFontSize" ) );
  DefaultFontSize->setMinimum ( 8 );
  DefaultFontSize->setMaximum ( 50 );
  horizontalLayout->addWidget ( DefaultFontSize );

  QLabel* label_ff = new QLabel ( trUtf8 ( "Fixed Font Size:" ), fontGroup );
  label_ff->setAlignment ( labelAlign );
  label_ff->setSizePolicy ( sizePolicy );
  horizontalLayout->addWidget ( label_ff );

  DefaultFixedFontSize = new QSpinBox ( fontGroup );
  DefaultFixedFontSize->setObjectName ( QLatin1String ( "DefaultFixedFontSize" ) );
  DefaultFixedFontSize->setMinimum ( 8 );
  DefaultFixedFontSize->setMaximum ( 50 );
  horizontalLayout->addWidget ( DefaultFixedFontSize );

  fontGroup->setLayout ( horizontalLayout );
  verticalLayout->addWidget ( fontGroup );

  // Header Definitionen
  headerDefinitions = new ConfigHeaderDefinitions ( centralWidget );
  verticalLayout->addWidget ( headerDefinitions );

  // Pluginpfad auswählen
  pluginPathChooser = new DirectoryChooser ( trUtf8 ( "Plugins Directory" ),
          QString::fromUtf8 ( "webkit_plugin_path" ), centralWidget );
  verticalLayout->addWidget ( pluginPathChooser );

  centralWidget->setLayout ( verticalLayout );

  connect ( DefaultFontSize, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );

  connect ( DefaultFixedFontSize, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );

  connect ( headerDefinitions, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );

  connect ( pluginPathChooser, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );
}

/**
* Registriere alle Auswahlboxen für die Signal
* verabeitung von @ref modified
*/
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
    btnGroup->addButton ( findChild<FeatureBox*> ( it.key() ), id++ );
  }

  connect ( btnGroup, SIGNAL ( buttonClicked ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );
}

void ConfigBrowser::itemClicked ( int )
{
  emit modified ( true );
}

void ConfigBrowser::load ( Settings * cfg )
{
  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    findChild<FeatureBox*> ( it.key() )->setChecked ( cfg->value ( it.key(), it.value() ).toBool() );
  }
  DefaultFontSize->setValue ( cfg->value ( QLatin1String ( "DefaultFontSize" ), 16 ).toUInt() );
  DefaultFixedFontSize->setValue ( cfg->value ( QLatin1String ( "DefaultFixedFontSize" ), 16 ).toUInt() );
  // Header Definitions
  headerDefinitions->loadHeaderDefinitions ( cfg );
  // Plugin Pfad
  pluginPathChooser->setDirectory ( cfg->value ( QLatin1String ( "webkit_plugin_path" ) ).toString() );
}

void ConfigBrowser::save ( Settings * cfg )
{
  QMapIterator<QString,bool> it ( settingsMap() );
  while ( it.hasNext() )
  {
    it.next();
    cfg->setValue ( it.key(), findChild<FeatureBox*> ( it.key() )->isChecked() );
  }
  cfg->setValue ( QLatin1String ( "DefaultFontSize" ), DefaultFontSize->value() );
  cfg->setValue ( QLatin1String ( "DefaultFixedFontSize" ), DefaultFixedFontSize->value() );
  // Header Definitions
  headerDefinitions->saveHeaderDefinitions ( cfg );
  // Plugin Pfad
  cfg->setValue ( QLatin1String ( "webkit_plugin_path" ), pluginPathChooser->getDirectory() );
}

ConfigBrowser::~ConfigBrowser()
{}
