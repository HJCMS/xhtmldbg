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
    : PageWidget ( trUtf8 ( "Defaults" ), parent )
{
  setObjectName ( QLatin1String ( "config_page_ide" ) );
  setNotice ( true );
  setCheckable ( false );

  Qt::Alignment laAlignRight = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_ide_vertical_layout" ) );
  verticalLayout->setContentsMargins ( 0, 5, 0, 5 );
  verticalLayout->setSpacing ( 5 );

  //  Welche URL beim Start Info
  //  @begin::row1 {
  QLabel* startUpUrlInfo = new QLabel ( centralWidget );
  startUpUrlInfo->setObjectName ( QLatin1String ( "config_page_ide_info_start_url_edit" ) );
  startUpUrlInfo->setText ( trUtf8 ( "Load this URL as default on start, if empty xhtmldbg will use recently viewed Url." ) );
  startUpUrlInfo->setWordWrap ( true );
  verticalLayout->addWidget ( startUpUrlInfo );
  // } @end::row1

  //  Welche URL beim Start laden Eingabe?
  //  @begin::row2 {
  QHBoxLayout* horizontalLayoutOne = new QHBoxLayout;
  horizontalLayoutOne->setObjectName ( QLatin1String ( "config_page_ide_hlayout_one" ) );
  horizontalLayoutOne->setContentsMargins ( 0, 0, 0, 0 );

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
  m_hideDownloadWidget->setText ( trUtf8 ( "Do not open Downloadmanager on Application start." ) );
  m_hideDownloadWidget->setChecked ( true );
  verticalLayout->addWidget ( m_hideDownloadWidget );
  // } @end::row3

  /** Den W3C CSS Validator Konfigurieren */
  //  @begin::row4 {
  QGroupBox* validatorGroup = new QGroupBox ( trUtf8 ( "W3C CSS Validator Settings" ), centralWidget );
  validatorGroup->setObjectName ( QLatin1String ( "config_page_ide_info_css" ) );
  validatorGroup->setFlat ( true );
  validatorGroup->setContentsMargins ( 0, 5, 0, 5 );

  QGridLayout* gridLayoutCSS = new QGridLayout ( validatorGroup );
  gridLayoutCSS->setObjectName ( QLatin1String ( "config_page_ide_layout_css" ) );
  gridLayoutCSS->setContentsMargins ( 0, 5, 0, 5 );

  // Java Programm Zeile
  QLabel* w3c_txt_one = new QLabel ( validatorGroup );
  w3c_txt_one->setObjectName ( QLatin1String ( "config_page_ide_w3c_txt_one" ) );
  w3c_txt_one->setAlignment ( laAlignRight );
  w3c_txt_one->setText ( trUtf8 ( "Java Application" ) );
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
  w3c_txt_two->setText ( trUtf8 ( "Path to W3C Validator file" ) );
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
  w3c_txt_three->setText ( trUtf8 ( "W3C Validator CLASSPATH" ) );
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

  /** Programm Design Optionen */
  //  @begin::row5 {
  QGroupBox* iconThemeGroup = new QGroupBox ( trUtf8 ( "Application Icon Theme" ), centralWidget );
  iconThemeGroup->setObjectName ( QLatin1String ( "config_page_ide_icon_theme" ) );
  iconThemeGroup->setFlat ( true );
  iconThemeGroup->setContentsMargins ( 0, 5, 0, 5 );

  QGridLayout* iconThemeLayout = new QGridLayout ( iconThemeGroup );
  iconThemeLayout->setObjectName ( QLatin1String ( "config_page_ide_icon_theme_layout" ) );
  iconThemeLayout->setContentsMargins ( 0, 5, 0, 5 );

  // Icon Thema Pfad Zeile
  QLabel* theme_path_txt = new QLabel ( iconThemeGroup );
  theme_path_txt->setObjectName ( QLatin1String ( "config_page_ide_theme_path_txt" ) );
  theme_path_txt->setText ( trUtf8 ( "Primary Icon Theme path" ) );
  theme_path_txt->setIndent ( 2 );
  iconThemeLayout->addWidget ( theme_path_txt, 0, 0, 1, 3 );

  m_iconThemesList = new IconThemesList ( iconThemeGroup );
  iconThemeLayout->addWidget ( m_iconThemesList, 1, 0, 1, 3 );

  // Icon Thema Zeile Standard Oxygen
  QLabel* theme_name_txt = new QLabel ( iconThemeGroup );
  theme_name_txt->setObjectName ( QLatin1String ( "config_page_ide_theme_name_txt" ) );
  theme_name_txt->setAlignment ( laAlignRight );
  theme_name_txt->setText ( trUtf8 ( "Default Icon Theme" ) );
  theme_name_txt->setIndent ( 2 );
  iconThemeLayout->addWidget ( theme_name_txt, 2, 0, 1, 1 );

  m_iconThemeSelecter = new IconThemeSelecter ( iconThemeGroup );
  m_iconThemeSelecter->setObjectName ( QLatin1String ( "config_page_ide_icon_theme_path" ) );
  iconThemeLayout->addWidget ( m_iconThemeSelecter, 2, 1, 1, 1 );

  QToolButton* theme_btn = new QToolButton ( iconThemeGroup );
  theme_btn->setObjectName ( QLatin1String ( "config_page_ide_open_theme_button" ) );
  theme_btn->setToolTip ( trUtf8 ( "Add Icon Theme Search path" ) );
  theme_btn->setIcon ( ConfigUtils::folderIcon() );
  iconThemeLayout->addWidget ( theme_btn, 2, 2, 1, 1 );

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
            this, SIGNAL ( modified ( bool ) ) );

  connect ( m_iconThemeSelecter, SIGNAL ( modified ( bool ) ),
            this, SIGNAL ( modified ( bool ) ) );

  connect ( theme_btn, SIGNAL ( clicked() ),
            this, SLOT ( openThemePathDialog() ) );
}

void ConfigIDE::checkCSSValidator()
{
  QFileInfo db ( m_w3cJarFile->text() );
  if ( ! db.exists() )
    m_missingW3CMissing->setText ( trUtf8 ( "<div style=\"color:red;\">W3C CSS Validator not found!</div><div>You can download it from <a href=\"http://xhtmldbg.hjcms.de\">xhtmldbg</a> Homepage.</div>" ) );
  else
    m_missingW3CMissing->clear();
}

/**
* Dieser Slot wird immer dann aufgerufen wenn ein
* QLineEdit verändert wurde.
*/
void ConfigIDE::editingFinished ( const QString &t )
{
  Q_UNUSED ( t )
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
  fl << trUtf8 ( "Java-Archive %1" ).arg ( "*.jar" );
  fl << trUtf8 ( "All %1" ).arg ( "*" );

  pn = QFileDialog::getOpenFileName ( this, trUtf8 ( "W3C CSS Validator" ), po, fl.join ( ";;" ) );

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
  p = ConfigUtils::findDirectoryDialog ( this, trUtf8 ( "W3C CSS Validator Java-Archive" ), po );

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
  p = ConfigUtils::findDirectoryDialog ( this, trUtf8 ( "Icon Theme path" ), QString ( "/usr/share/icons" ) );

  if ( p.isEmpty() )
    return;

  m_iconThemesList->addPath ( p );
}

void ConfigIDE::load ( QSettings * cfg )
{
  m_editStartUpUrl->setText ( cfg->value ( QLatin1String ( "StartUpUrl" ) ).toString() );
  m_hideDownloadWidget->setChecked ( cfg->value ( QLatin1String ( "HideDownloadManager" ), true ).toBool() );
  m_javaApplication->setText ( cfg->value ( QLatin1String ( "css_appl" ), QLatin1String ( "java" ) ).toString() );
  m_w3cJarFile->setText ( cfg->value ( QLatin1String ( "css_validator" ) ).toString() );
  m_w3cClasspath->setText ( cfg->value ( QLatin1String ( "css_classpath" ) ).toString() );
  m_iconThemesList->insertPaths ( cfg->value ( QLatin1String ( "iconthemepaths" ), QIcon::themeSearchPaths() ).toStringList() );
  m_iconThemeSelecter->findThemeIndexes ( m_iconThemesList->paths() );
  m_iconThemeSelecter->setTheme ( cfg->value ( QLatin1String ( "icontheme" ), QLatin1String ( "oxygen" ) ).toString() );
  checkCSSValidator();
}

void ConfigIDE::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "StartUpUrl" ), m_editStartUpUrl->text() );
  cfg->setValue ( QLatin1String ( "HideDownloadManager" ), m_hideDownloadWidget->isChecked() );
  cfg->setValue ( QLatin1String ( "css_appl" ), m_javaApplication->text() );
  cfg->setValue ( QLatin1String ( "css_validator" ), m_w3cJarFile->text() );
  cfg->setValue ( QLatin1String ( "css_classpath" ), m_w3cClasspath->text() );

  // Thema Einstellungen
  QString theme = m_iconThemeSelecter->selectedTheme();
  cfg->setValue ( QLatin1String ( "icontheme" ), theme );
  cfg->setValue ( QLatin1String ( "iconthemepaths" ), m_iconThemesList->paths() );
}

void ConfigIDE::defaults()
{
  m_editStartUpUrl->clear();
  m_hideDownloadWidget->setChecked ( true );
  m_javaApplication->setText ( QLatin1String ( "java" ) );
  m_w3cJarFile->clear();
  m_w3cClasspath->clear();
  m_iconThemesList->insertPaths ( QIcon::themeSearchPaths() );
  m_iconThemeSelecter->findThemeIndexes ( m_iconThemesList->paths() );;
  m_iconThemeSelecter->setTheme ( QLatin1String ( "oxygen" ) );
}

ConfigIDE::~ConfigIDE()
{}
