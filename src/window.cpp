/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "window.h"
#include "addresstoolbar.h"
#include "seotoolbar.h"
#include "tabbedwidget.h"
#include "messanger.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QProcess>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

Window::Window() : QMainWindow()
{
  // Window Properties
  setWindowTitle ( trUtf8 ( "XHTML Debugger" ) );
  setObjectName ( "xhtmldbgwindow" );
  setWindowIcon ( QIcon( QString::fromUtf8( ":/icons/qtidy.png" ) ) );
  // Settings
  m_settings = new QSettings ( QSettings::NativeFormat,
                               QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  // init central Design Widget
  m_centralWidget = centralWidget ();

  // StatusBar
  m_statusBar = statusBar();
  m_statusBar->setSizeGripEnabled ( true );

  // Tabbed Widget
  m_tabbedWidget = new TabbedWidget ( m_centralWidget );

  // XHTML & JavaScript Messanger DockWidget
  m_messanger = new Messanger ( m_centralWidget );
  addDockWidget ( Qt::BottomDockWidgetArea, m_messanger );

  // finalize WindowDesign
  createMenus();
  createToolBars();
  setCentralWidget ( m_tabbedWidget );

  // Load Settings
  m_tabbedWidget->setCurrentIndex ( m_settings->value ( "CurrentTabIndex", 0 ).toUInt() );
  restoreState ( m_settings->value ( "MainWindowState" ).toByteArray() );
  restoreGeometry ( m_settings->value ( "MainWindowGeometry" ).toByteArray() );

  update();
}

void Window::createMenus()
{
  m_menuBar = menuBar ();

  QIcon icon;

  // Main Menu
  m_applicationMenu = m_menuBar->addMenu ( trUtf8 ( "Application" ) );
  m_applicationMenu ->setObjectName ( QLatin1String ( "applicationmenu" ) );

  // Action Open URL Dialog
  actionOpenUrl = m_applicationMenu->addAction ( trUtf8 ( "Open Url" ) );
  actionOpenUrl->setStatusTip ( trUtf8 ( "Load Document from Url" ) );
  actionOpenUrl->setShortcut ( Qt::CTRL + Qt::Key_U );
  actionOpenUrl->setIcon ( icon.fromTheme ( QLatin1String ( "document-open-remote" ) ) );

  // Action Open File from Location
  actionOpenHtml = m_applicationMenu->addAction ( trUtf8 ( "Open Html File" ) );
  actionOpenHtml->setStatusTip ( trUtf8 ( "Open Html from System" ) );
  actionOpenHtml->setShortcut ( Qt::CTRL + Qt::Key_O );
  actionOpenHtml->setIcon ( icon.fromTheme ( QLatin1String ( "document-open" ) ) );

  // Action Application Exit
  actionQuit = m_applicationMenu->addAction ( trUtf8 ( "Quit" ) );
  actionQuit->setStatusTip ( trUtf8 ( "Close Debugger" ) );
  actionQuit->setShortcut ( Qt::CTRL + Qt::Key_Q );
  actionQuit->setMenuRole ( QAction::QuitRole );
  actionQuit->setIcon ( icon.fromTheme ( QLatin1String ( "application-exit" ) ) );
  connect ( actionQuit, SIGNAL ( triggered() ), this, SLOT ( close() ) );

  // Debugger Menu
  m_debuggerMenu = m_menuBar->addMenu ( trUtf8 ( "Debugger" ) );

  // Action Parse Document Source
  actionParse = m_debuggerMenu->addAction ( trUtf8 ( "Parse" ) );
  actionParse->setStatusTip ( trUtf8 ( "Parse current Document Source" ) );
  actionParse->setShortcut ( Qt::ALT + Qt::Key_C );
  actionParse->setIcon ( icon.fromTheme ( QLatin1String ( "document-edit-verify" ) ) );
  connect ( actionParse, SIGNAL ( triggered() ), m_tabbedWidget, SLOT ( check() ) );

  // Action Prepare and Format Document Source
  actionClean = m_debuggerMenu->addAction ( trUtf8 ( "Format" ) );
  actionClean->setStatusTip ( trUtf8 ( "Prepare and Format Document Source" ) );
  actionClean->setShortcut ( Qt::ALT + Qt::Key_F );
  actionClean->setIcon ( icon.fromTheme ( QLatin1String ( "format-list-ordered" ) ) );
  connect ( actionClean, SIGNAL ( triggered() ), m_tabbedWidget, SLOT ( format() ) );

  // Viewer Menu
  m_viewMenu = m_menuBar->addMenu ( trUtf8 ( "Browser" ) );

  // Action WebView Reload
  actionPageReload = m_viewMenu->addAction ( trUtf8 ( "Refresh" ) );
  actionPageReload->setShortcut ( QKeySequence::Refresh );
  actionPageReload->setIcon ( icon.fromTheme ( QLatin1String ( "view-refresh" ) ) );

  // Action WebView Back
  actionPageBack = m_viewMenu->addAction ( trUtf8 ( "Back" ) );
  actionPageBack->setShortcut ( QKeySequence::Back );
  actionPageBack->setIcon ( icon.fromTheme ( QLatin1String ( "go-previous-view-page" ) ) );

  // Action WebView Forward
  actionPageForward = m_viewMenu->addAction ( trUtf8 ( "Forward" ) );
  actionPageForward->setShortcut ( QKeySequence::Forward );
  actionPageForward->setIcon ( icon.fromTheme ( QLatin1String ( "go-next-view-page" ) ) );

  // New Empty WebView
  actionNewEmptyPage = m_viewMenu->addAction ( trUtf8 ( "New Page" ) );
  actionNewEmptyPage->setStatusTip ( trUtf8 ( "Add a new empty Tab" ) );
  actionNewEmptyPage->setShortcut ( Qt::CTRL + Qt::Key_N );
  actionNewEmptyPage->setIcon ( icon.fromTheme ( QLatin1String ( "window-new" ) ) );

  // Bookmark Menu
  m_bookmarkMenu = m_menuBar->addMenu ( trUtf8 ( "Bookmark" ) );

  // Configuration Menu
  m_configurationMenu = m_menuBar->addMenu ( trUtf8 ( "Configuration" ) );
  // Action Open qtidyrc
  actionTidyConfig = m_configurationMenu->addAction ( trUtf8 ( "Configure Tidyrc" ) );
  actionTidyConfig->setIcon ( icon.fromTheme ( QLatin1String ( "configure-toolbars" ) ) );

  // Action open Configuration Dialog
  actionConfigDialog = m_configurationMenu->addAction ( trUtf8 ( "Settings" ) );
  actionConfigDialog->setIcon ( icon.fromTheme ( QLatin1String ( "configure" ) ) );

  // Help and About Menu
  QMenu *m_aboutMenu = m_menuBar->addMenu ( trUtf8 ( "About" ) );
  QAction* actionAboutQt = m_aboutMenu->addAction ( trUtf8 ( "about Qt" ) );
  actionAboutQt->setIcon ( icon.fromTheme ( QLatin1String ( "documentinfo" ) ) );
  actionAboutQt->setMenuRole ( QAction::AboutQtRole );

  QAction* actionAboutHJCMS = m_aboutMenu->addAction ( trUtf8 ( "about hjcms" ) );
  actionAboutHJCMS->setIcon ( icon.fromTheme ( QLatin1String ( "documentinfo" ) ) );
  actionAboutHJCMS->setMenuRole ( QAction::AboutRole );

}

void Window::createToolBars()
{
  // Actions ToolBar
  m_actionsToolBar = addToolBar ( trUtf8 ( "Actions" ) );
  m_actionsToolBar->setObjectName ( QLatin1String ( "actionstoolbar" ) );
  m_actionsToolBar->addAction ( actionOpenUrl );
  m_actionsToolBar->addAction ( actionOpenHtml );
  m_actionsToolBar->addSeparator();
  m_actionsToolBar->addAction ( actionNewEmptyPage );
  m_actionsToolBar->addSeparator();
  m_actionsToolBar->addAction ( actionParse );
  m_actionsToolBar->addAction ( actionClean );

  // Settings ToolBar
  m_settingsToolBar = addToolBar ( trUtf8 ( "Settings" ) );
  m_settingsToolBar->setObjectName ( QLatin1String ( "settingstoolbar" ) );
  m_settingsToolBar->addAction ( actionTidyConfig );
  m_settingsToolBar->addAction ( actionConfigDialog );

  // Address Input ToolBar
  m_addressToolBar = new AddressToolBar ( m_centralWidget );
  connect( m_tabbedWidget, SIGNAL ( loadUrl ( const QUrl & ) ),
           m_addressToolBar, SLOT ( setUrl ( const QUrl& ) ) );
  connect( m_addressToolBar, SIGNAL ( urlChanged ( const QUrl & ) ),
           m_tabbedWidget, SLOT ( setUrl ( const QUrl & ) ) );

  addToolBar ( m_addressToolBar );

  // SEO Input ToolBar
  m_seoToolBar =  new SeoToolBar ( m_centralWidget );
  addToolBar ( m_seoToolBar );

  // Bookmark Toolbar
  // TODO
}

void Window::closeEvent ( QCloseEvent *event )
{
  m_settings->setValue ( "MainWindowState", saveState() );
  m_settings->setValue ( "MainWindowGeometry", saveGeometry() );
  m_settings->setValue ( "CurrentTabIndex", m_tabbedWidget->currentIndex() );
  QMainWindow::closeEvent ( event );
}

void Window::openTidyConfigApplication()
{
  QProcess::startDetached ( QString::fromUtf8 ( "qtidyrc" ) );
}

Window::~Window()
{}
