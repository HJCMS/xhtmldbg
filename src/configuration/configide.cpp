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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "configide.h"
#include "configutils.h"
#include "iconthemeselecter.h"
#include "iconthemeslist.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QListWidgetItem>
#include <QtGui/QToolButton>

ConfigIDE::ConfigIDE ( QWidget * parent )
    : PageWidget ( i18n ( "Defaults" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_ide" ) );
  setNotice ( true );
  setCheckable ( false );

  Qt::Alignment laAlignRight = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_ide_vertical_layout" ) );

  //  Welche URL beim Start Info
  //  @begin::row1 {
  QLabel* startUpUrlInfo = new QLabel ( centralWidget );
  startUpUrlInfo->setObjectName ( QLatin1String ( "config_page_ide_info_start_url_edit" ) );
  startUpUrlInfo->setText ( i18n ( "Load this URL as default on start, if empty xhtmldbg will use recently viewed Url." ) );
  startUpUrlInfo->setWordWrap ( true );
  verticalLayout->addWidget ( startUpUrlInfo );
  // } @end::row1

  //  Welche URL beim Start laden Eingabe?
  //  @begin::row2 {
  QHBoxLayout* horizontalLayoutOne = new QHBoxLayout;
  horizontalLayoutOne->setObjectName ( QLatin1String ( "config_page_ide_hlayout_one" ) );

  m_editStartUpUrl = new QLineEdit ( centralWidget );
  m_editStartUpUrl->setObjectName ( QLatin1String ( "config_page_ide_start_url_edit" ) );
  horizontalLayoutOne->addWidget ( m_editStartUpUrl );

  QToolButton* clearStartupUrlBtn = new QToolButton ( centralWidget );
  clearStartupUrlBtn->setObjectName ( QString::fromUtf8 ( "config_page_ide_start_url_button" ) );
  clearStartupUrlBtn->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );
  horizontalLayoutOne->addWidget ( clearStartupUrlBtn );

  verticalLayout->addLayout ( horizontalLayoutOne );
  // } @end::row2

  /** Den Downloadmanager beim Start nicht laden!
  * Wenn Aktiviert wird vor dem Beenden dieses Widget geschlossen!
  * Siehe @ref Window::closeEvent */
  //  @begin::row3 {
  m_hideDownloadWidget = new QCheckBox ( centralWidget );
  m_hideDownloadWidget->setObjectName ( QLatin1String ( "config_page_ide_hide_download_widget" ) );
  m_hideDownloadWidget->setText ( i18n ( "Do not open Downloadmanager on Application start." ) );
  m_hideDownloadWidget->setChecked ( true );
  verticalLayout->addWidget ( m_hideDownloadWidget );
  // } @end::row3

  /** Den W3C CSS Validator Konfigurieren */
  //  @begin::row4 {
  QGroupBox* validatorGroup = new QGroupBox ( i18n ( "W3C CSS Validator Settings" ), centralWidget );
  validatorGroup->setObjectName ( QLatin1String ( "config_page_ide_info_css" ) );
  validatorGroup->setFlat ( true );

  QGridLayout* gridLayoutCSS = new QGridLayout ( validatorGroup );
  gridLayoutCSS->setObjectName ( QLatin1String ( "config_page_ide_layout_css" ) );

  // Java Programm Zeile
  QLabel* w3c_txt_one = new QLabel ( validatorGroup );
  w3c_txt_one->setObjectName ( QLatin1String ( "config_page_ide_w3c_txt_one" ) );
  w3c_txt_one->setAlignment ( laAlignRight );
  w3c_txt_one->setText ( i18n ( "Java Application" ) );
  w3c_txt_one->setIndent ( 2 );
  gridLayoutCSS->addWidget ( w3c_txt_one, 0, 0, 1, 1 );

  m_javaApplication = new QLineEdit ( validatorGroup );
  m_javaApplication->setObjectName ( QLatin1String ( "config_page_ide_java_edit" ) );
  gridLayoutCSS->addWidget ( m_javaApplication, 0, 1, 1, 1 );

  QToolButton* w3c_btn_appl = new QToolButton ( validatorGroup );
  w3c_btn_appl->setObjectName ( QLatin1String ( "w3c_btn_appl" ) );
  w3c_btn_appl->setIcon ( ConfigUtils::folderIcon() );
  gridLayoutCSS->addWidget ( w3c_btn_appl, 0, 2, 1, 1 );

  // W3C Validator Jar Zeile
  QLabel* w3c_txt_two = new QLabel ( validatorGroup );
  w3c_txt_two->setObjectName ( QLatin1String ( "config_page_ide_w3c_txt_two" ) );
  w3c_txt_two->setAlignment ( laAlignRight );
  w3c_txt_two->setText ( i18n ( "Path to W3C Validator file" ) );
  w3c_txt_two->setIndent ( 2 );
  gridLayoutCSS->addWidget ( w3c_txt_two, 1, 0, 1, 1 );

  m_w3cJarFile = new QLineEdit ( validatorGroup );
  m_w3cJarFile->setObjectName ( QLatin1String ( "config_page_ide_w3c_jar_edit" ) );
  gridLayoutCSS->addWidget ( m_w3cJarFile, 1, 1, 1, 1 );

  QToolButton* w3c_btn_jar = new QToolButton ( validatorGroup );
  w3c_btn_jar->setObjectName ( QLatin1String ( "w3c_btn_jar" ) );
  w3c_btn_jar->setIcon ( ConfigUtils::folderIcon() );
  gridLayoutCSS->addWidget ( w3c_btn_jar, 1, 2, 1, 1 );

  // W3C Validator Klassenpfad Zeile
  QLabel* w3c_txt_three = new QLabel ( validatorGroup );
  w3c_txt_three->setObjectName ( QLatin1String ( "config_page_ide_w3c_txt_three" ) );
  w3c_txt_three->setAlignment ( laAlignRight );
  w3c_txt_three->setText ( i18n ( "W3C Validator CLASSPATH" ) );
  w3c_txt_three->setIndent ( 2 );
  gridLayoutCSS->addWidget ( w3c_txt_three, 2, 0, 1, 1 );

  m_w3cClasspath = new QLineEdit ( validatorGroup );
  m_w3cClasspath->setObjectName ( QLatin1String ( "config_page_ide_w3c_classpath_edit" ) );
  gridLayoutCSS->addWidget ( m_w3cClasspath, 2, 1, 1, 1 );

  QToolButton* w3c_btn_class = new QToolButton ( validatorGroup );
  w3c_btn_class->setObjectName ( QLatin1String ( "w3c_btn_class" ) );
  w3c_btn_class->setIcon ( ConfigUtils::folderIcon() );
  gridLayoutCSS->addWidget ( w3c_btn_class, 2, 2, 1, 1 );

  validatorGroup->setLayout ( gridLayoutCSS );
  verticalLayout->addWidget ( validatorGroup );

  m_missingW3CMissing = new QLabel ( centralWidget );
  m_missingW3CMissing->setObjectName ( QLatin1String ( "config_page_ide_warn_w3c_missing" ) );
  m_missingW3CMissing->setWordWrap ( true );
  m_missingW3CMissing->setOpenExternalLinks ( true );
  m_missingW3CMissing->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  verticalLayout->addWidget ( m_missingW3CMissing );

  // } @end::row4

  /** Programm Icon Thema Optionen */
  //  @begin::row5 {
  QGroupBox* iconThemeGroup = new QGroupBox ( i18n ( "Application Theme Settings" ), centralWidget );
  iconThemeGroup->setObjectName ( QLatin1String ( "config_page_ide_icon_theme" ) );
  iconThemeGroup->setFlat ( true );

  QGridLayout* iconThemeLayout = new QGridLayout ( iconThemeGroup );
  iconThemeLayout->setObjectName ( QLatin1String ( "config_page_ide_icon_theme_layout" ) );

  // Icon Thema Pfad Zeile
  QLabel* theme_path_txt = new QLabel ( iconThemeGroup );
  theme_path_txt->setObjectName ( QLatin1String ( "config_page_ide_theme_path_txt" ) );
  theme_path_txt->setText ( i18n ( "On Application start, Qt are trying to search with all Icon Theme paths on your System and read them recursively for Theme Icons used by xhtmldbg. On ix86 Systems this makes your Application startup slower. With this Entry list you can perform this operation. Attention! When set this Path's to make sure where your Selected Icon Theme located." ) );
  theme_path_txt->setIndent ( 2 );
  theme_path_txt->setWordWrap ( true );
  iconThemeLayout->addWidget ( theme_path_txt, 0, 0, 1, 2 );

  m_iconThemesList = new IconThemesList ( iconThemeGroup );
  iconThemeLayout->addWidget ( m_iconThemesList, 1, 0, 1, 2 );

  // Icon Thema Zeile Standard Oxygen
  QLabel* theme_name_txt = new QLabel ( iconThemeGroup );
  theme_name_txt->setObjectName ( QLatin1String ( "config_page_ide_theme_name_txt" ) );
  theme_name_txt->setAlignment ( laAlignRight );
  theme_name_txt->setText ( i18n ( "Default Icon Theme" ) );
  theme_name_txt->setIndent ( 2 );
  iconThemeLayout->addWidget ( theme_name_txt, 2, 0, 1, 1 );

  m_iconThemeSelecter = new IconThemeSelecter ( iconThemeGroup );
  m_iconThemeSelecter->setObjectName ( QLatin1String ( "config_page_ide_icon_theme_path" ) );
  m_iconThemeSelecter->setToolTip ( i18n ( "The Default Icon Theme is (Oxygen)" ) );
  iconThemeLayout->addWidget ( m_iconThemeSelecter, 2, 1, 1, 1 );

  iconThemeGroup->setLayout ( iconThemeLayout );
  verticalLayout->addWidget ( iconThemeGroup );
  // } @end::row5

  // finalize layout
  centralWidget->setLayout ( verticalLayout );

  connect ( m_editStartUpUrl, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( editingFinished ( const QString & ) ) );

  connect ( clearStartupUrlBtn, SIGNAL ( clicked() ),
            m_editStartUpUrl, SLOT ( clear() ) );

  connect ( m_javaApplication, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( editingFinished ( const QString & ) ) );

  connect ( m_w3cJarFile, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( editingFinished ( const QString & ) ) );

  connect ( m_w3cClasspath, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( editingFinished ( const QString & ) ) );

  connect ( w3c_btn_appl, SIGNAL ( clicked() ),
            this, SLOT ( openJavaApplicationDialog() ) );

  connect ( w3c_btn_jar, SIGNAL ( clicked() ),
            this, SLOT ( openCSSValidatorDialog() ) );

  connect ( w3c_btn_class, SIGNAL ( clicked() ),
            this, SLOT ( openCSSValidatorClassPathDialog() ) );

  connect ( m_iconThemesList, SIGNAL ( modified ( bool ) ),
            this, SLOT ( clearingThemePaths ( bool ) ) );

  connect ( m_iconThemesList, SIGNAL ( setPathClicked() ),
            this, SLOT ( openThemePathDialog() ) );

  connect ( m_iconThemeSelecter, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );
}

/**
* Wenn der Validierer nicht gefunden wird dann hier
* ein QLabel mit der Meldung erzeugen und unten anhängen.
*/
void ConfigIDE::checkCSSValidator()
{
  QFileInfo db ( m_w3cJarFile->text() );
  if ( ! db.exists() )
    m_missingW3CMissing->setText ( i18n ( "<div style=\"color:red;\">W3C CSS Validator not found!</div><div>You can download it from <a href=\"http://xhtmldbg.hjcms.de\">xhtmldbg</a> Homepage.</div>" ) );
  else
    m_missingW3CMissing->clear();
}

/**
* Dieser Slot wird immer dann aufgerufen wenn ein
* QLineEdit verändert wurde.
*/
void ConfigIDE::editingFinished ( const QString & )
{
  mod = true;
  emit modified ( true );
}

/**
* Dialog zum finden des "java" Programms
*/
void ConfigIDE::openJavaApplicationDialog ()
{
  QString bin = ConfigUtils::findBinaryDialog ( m_javaApplication->text(), this );
  m_javaApplication->setText ( bin );
}

/**
* Dialog zum finden der css-validator.jar Datei.
*/
void ConfigIDE::openCSSValidatorDialog ()
{
  QString pn;
  QString po = m_w3cJarFile->text().isEmpty() ? QString ( "/usr/share/java" ) : m_w3cJarFile->text();

  QStringList fl;
  fl << i18n ( "Java-Archive %1" ).arg ( "*.jar" );
  fl << i18n ( "All %1" ).arg ( "*" );

  pn = QFileDialog::getOpenFileName ( this, i18n ( "W3C CSS Validator" ), po, fl.join ( ";;" ) );

  QFileInfo db ( pn );
  if ( db.exists() )
    m_w3cJarFile->setText ( db.absoluteFilePath() );

  checkCSSValidator();
}

/**
* Dialog zum finden der css-validator Archive.
*/
void ConfigIDE::openCSSValidatorClassPathDialog ()
{
  QString p;
  QString po = m_w3cClasspath->text().isEmpty() ? QString ( "/usr/share/java" ) : m_w3cClasspath->text();
  p = ConfigUtils::findDirectoryDialog ( this, i18n ( "W3C CSS Validator Java-Archive" ), po );

  if ( p.isEmpty() )
    return;

  m_w3cClasspath->setText ( p );
}

/**
* Dialog zum finden eines Icon Thema Pfades.
*/
void ConfigIDE::openThemePathDialog ()
{
  QString p;
  p = ConfigUtils::findDirectoryDialog ( this, i18n ( "Icon Theme path" ), QString ( "/usr/share/icons" ) );

  if ( p.isEmpty() )
    return;

  m_iconThemesList->addPath ( p );
}

/**
* Wenn ein Update auf die Pfade ausgeführt wird
* die Themen Auswahl neu einlesen.
*/
void ConfigIDE::clearingThemePaths ( bool )
{
  QString currentTheme = m_iconThemeSelecter->selectedTheme();
  m_iconThemeSelecter->findThemeIndexes ( m_iconThemesList->iconPaths() );
  m_iconThemeSelecter->setTheme ( currentTheme );
}

void ConfigIDE::load ( Settings * cfg )
{
  m_editStartUpUrl->setText ( cfg->value ( QLatin1String ( "StartUpUrl" ) ).toString() );
  m_hideDownloadWidget->setChecked ( cfg->value ( QLatin1String ( "HideDownloadManager" ), true ).toBool() );
  m_javaApplication->setText ( cfg->value ( QLatin1String ( "css_appl" ), QLatin1String ( "java" ) ).toString() );
  m_w3cJarFile->setText ( cfg->value ( QLatin1String ( "css_validator" ) ).toString() );
  m_w3cClasspath->setText ( cfg->value ( QLatin1String ( "css_classpath" ) ).toString() );
  m_iconThemesList->insertPaths ( cfg->value ( QLatin1String ( "iconthemepaths" ), QIcon::themeSearchPaths() ).toStringList() );
  m_iconThemeSelecter->findThemeIndexes ( m_iconThemesList->iconPaths() );
  m_iconThemeSelecter->setTheme ( cfg->value ( QLatin1String ( "icontheme" ), QLatin1String ( "oxygen" ) ).toString() );
  checkCSSValidator();
  sighted = true;
}

void ConfigIDE::save ( Settings * cfg )
{
  cfg->setValue ( QLatin1String ( "StartUpUrl" ), m_editStartUpUrl->text() );
  cfg->setValue ( QLatin1String ( "HideDownloadManager" ), m_hideDownloadWidget->isChecked() );
  cfg->setValue ( QLatin1String ( "css_appl" ), m_javaApplication->text() );
  cfg->setValue ( QLatin1String ( "css_validator" ), m_w3cJarFile->text() );
  cfg->setValue ( QLatin1String ( "css_classpath" ), m_w3cClasspath->text() );

  // Thema Einstellungen
  QString theme = m_iconThemeSelecter->selectedTheme();
  cfg->setValue ( QLatin1String ( "icontheme" ), theme );
  cfg->setValue ( QLatin1String ( "iconthemepaths" ), m_iconThemesList->iconPaths() );
}

bool ConfigIDE::isModified ()
{
  return mod;
}

bool ConfigIDE::isSighted ()
{
  return sighted;
}

ConfigIDE::~ConfigIDE()
{}
