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
#include "keywordstoolbar.h"
#include "webviewer.h"
#include "sourcewidget.h"
#include "domviewer.h"
#include "messanger.h"
#include "bookmark.h"
#include "openurldialog.h"
#include "aboutdialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QKeySequence>

Window::Window() : QMainWindow()
{
  // Window Properties
  setWindowTitle ( trUtf8 ( "XHTML Debugger" ) );
  setObjectName ( "xhtmldbgwindow" );
  QIcon qTidyIcon ( QString::fromUtf8 ( ":/icons/qtidy.png" ) );
  setWindowIcon ( qTidyIcon );
  // Settings
  m_settings = new QSettings ( QSettings::NativeFormat,
                               QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  // StatusBar
  m_statusBar = statusBar();
  m_statusBar->setSizeGripEnabled ( true );

  // Browser DockWidget
  m_centralWidget = new QTabWidget ( this );
  m_centralWidget->setObjectName ( QLatin1String ( "centralwidget" ) );
  m_centralWidget->setTabPosition ( QTabWidget::South );
  m_centralWidget->setTabsClosable ( false );

  // WebViewer
  m_webViewer = new WebViewer ( m_centralWidget );
  m_centralWidget->insertTab ( 0, m_webViewer, trUtf8 ( "Browser" ) );
  m_centralWidget->setTabIcon ( 0, qTidyIcon );
  m_centralWidget->setCurrentIndex ( 0 );

  // Show XHTML Source
  m_sourceWidget = new SourceWidget ( this );
  m_centralWidget->insertTab ( 1, m_sourceWidget, trUtf8 ( "Source" ) );
  m_centralWidget->setTabIcon ( 1, qTidyIcon );

  // Show Document DomTree
  m_domViewer = new DomViewer ( this );
  m_centralWidget->insertTab ( 2, m_domViewer, trUtf8 ( "DOM" ) );
  m_centralWidget->setTabIcon ( 2, qTidyIcon );

  // XHTML & JavaScript Messanger DockWidget
  m_messanger = new Messanger ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_messanger );

  // finalize WindowDesign
  createMenus();
  createToolBars();

  setCentralWidget ( m_centralWidget );

  connect ( m_webViewer, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( requestsFinished ( bool ) ) );

  // Widget SIGNALS
  // Load Settings
  restoreState ( m_settings->value ( "MainWindowState" ).toByteArray() );
  restoreGeometry ( m_settings->value ( "MainWindowGeometry" ).toByteArray() );

  // FIXME Open File from Global Default's
  QUrl fallback ( "file:///home/heinemann/hjcms/QTidy/tests/in_588061.html" );
  QUrl recent = m_settings->value ( QLatin1String ( "RecentFile" ), fallback ).toUrl();
  openFile ( recent );

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
  connect ( actionOpenUrl, SIGNAL ( triggered() ), this, SLOT ( openUrlDialog() ) );

  // Action Open File from Location
  actionOpenHtml = m_applicationMenu->addAction ( trUtf8 ( "Open Html File" ) );
  actionOpenHtml->setStatusTip ( trUtf8 ( "Open Html from System" ) );
  actionOpenHtml->setShortcut ( Qt::CTRL + Qt::Key_O );
  actionOpenHtml->setIcon ( icon.fromTheme ( QLatin1String ( "document-open" ) ) );
  connect ( actionOpenHtml, SIGNAL ( triggered() ), this, SLOT ( openFileDialog() ) );

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
  connect ( actionParse, SIGNAL ( triggered() ), m_sourceWidget, SLOT ( check() ) );

  // Action Prepare and Format Document Source
  actionClean = m_debuggerMenu->addAction ( trUtf8 ( "Format" ) );
  actionClean->setStatusTip ( trUtf8 ( "Prepare and Format Document Source" ) );
  actionClean->setShortcut ( Qt::ALT + Qt::Key_F );
  actionClean->setIcon ( icon.fromTheme ( QLatin1String ( "format-list-ordered" ) ) );
  connect ( actionClean, SIGNAL ( triggered() ), m_sourceWidget, SLOT ( format() ) );

  // Viewer Menu
  m_viewMenu = m_menuBar->addMenu ( trUtf8 ( "Browser" ) );

  // Action WebView Reload
  actionPageReload = m_viewMenu->addAction ( trUtf8 ( "Refresh" ) );
  actionPageReload->setShortcut ( QKeySequence::Refresh );
  actionPageReload->setIcon ( icon.fromTheme ( QLatin1String ( "view-refresh" ) ) );
  connect ( actionPageReload, SIGNAL ( triggered () ), m_webViewer, SLOT ( refresh () ) );

  // Action WebView Back
  actionPageBack = m_viewMenu->addAction ( trUtf8 ( "Back" ) );
  actionPageBack->setShortcut ( QKeySequence::Back );
  actionPageBack->setIcon ( icon.fromTheme ( QLatin1String ( "go-previous-view-page" ) ) );
  connect ( actionPageBack, SIGNAL ( triggered () ), m_webViewer, SLOT ( back () ) );

  // Action WebView Forward
  actionPageForward = m_viewMenu->addAction ( trUtf8 ( "Forward" ) );
  actionPageForward->setShortcut ( QKeySequence::Forward );
  actionPageForward->setIcon ( icon.fromTheme ( QLatin1String ( "go-next-view-page" ) ) );
  connect ( actionPageForward, SIGNAL ( triggered () ), m_webViewer, SLOT ( forward () ) );

  // New Empty WebView
  actionNewEmptyPage = m_viewMenu->addAction ( trUtf8 ( "New Page" ) );
  actionNewEmptyPage->setStatusTip ( trUtf8 ( "Add a new empty Tab" ) );
  actionNewEmptyPage->setShortcut ( Qt::CTRL + Qt::Key_N );
  actionNewEmptyPage->setIcon ( icon.fromTheme ( QLatin1String ( "window-new" ) ) );
  connect ( actionNewEmptyPage, SIGNAL ( triggered () ), m_webViewer, SLOT ( addEmptyViewerTab () ) );

  // Bookmark Menu
  m_bookmarkMenu = new Bookmark ( m_menuBar );
  connect ( m_bookmarkMenu, SIGNAL ( openBookmark ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );

  connect ( m_webViewer, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ),
            m_bookmarkMenu, SLOT ( addBookmark ( const QUrl &, const QString & ) ) );

  m_menuBar->addMenu ( m_bookmarkMenu );

  // Configuration Menu
  m_configurationMenu = m_menuBar->addMenu ( trUtf8 ( "Configuration" ) );
  // Action Open qtidyrc
  actionTidyConfig = m_configurationMenu->addAction ( trUtf8 ( "Configure Tidyrc" ) );
  actionTidyConfig->setIcon ( icon.fromTheme ( QLatin1String ( "configure-toolbars" ) ) );
  connect ( actionTidyConfig, SIGNAL ( triggered() ),
            this, SLOT ( openTidyConfigApplication() ) );

  // Action open Configuration Dialog
  actionConfigDialog = m_configurationMenu->addAction ( trUtf8 ( "Settings" ) );
  actionConfigDialog->setIcon ( icon.fromTheme ( QLatin1String ( "configure" ) ) );
  // TODO  Action open Configuration Dialog

  // Help and About Menu
  QMenu *m_aboutMenu = m_menuBar->addMenu ( trUtf8 ( "About" ) );
  QAction* actionAboutQt = m_aboutMenu->addAction ( trUtf8 ( "about Qt" ) );
  actionAboutQt->setIcon ( icon.fromTheme ( QLatin1String ( "documentinfo" ) ) );
  actionAboutQt->setMenuRole ( QAction::AboutQtRole );
  connect ( actionAboutQt, SIGNAL ( triggered() ), qApp, SLOT ( aboutQt() ) );

  AboutDialog* aboutDialog = new AboutDialog ( this );
  QAction* actionAboutHJCMS = m_aboutMenu->addAction ( trUtf8 ( "about hjcms" ) );
  actionAboutHJCMS->setIcon ( icon.fromTheme ( QLatin1String ( "documentinfo" ) ) );
  actionAboutHJCMS->setMenuRole ( QAction::AboutRole );
  connect ( actionAboutHJCMS, SIGNAL ( triggered() ), aboutDialog, SLOT ( open() ) );
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
  m_addressToolBar = new AddressToolBar ( this );
  connect ( m_webViewer, SIGNAL ( urlChanged ( const QUrl & ) ),
            m_addressToolBar, SLOT ( setUrl ( const QUrl& ) ) );
  connect ( m_addressToolBar, SIGNAL ( urlChanged ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );

  addToolBar ( m_addressToolBar );

  // SEO Input ToolBar
  m_keywordsToolBar =  new KeywordsToolBar ( this );
  addToolBar ( m_keywordsToolBar );
}

void Window::closeEvent ( QCloseEvent *event )
{
  m_settings->setValue ( "MainWindowState", saveState() );
  m_settings->setValue ( "MainWindowGeometry", saveGeometry() );
  QMainWindow::closeEvent ( event );
}

void Window::requestsFinished ( bool ok )
{
  // TODO IF OK do something else
  if ( ok )
  {
    qDebug() << Q_FUNC_INFO;
    m_domViewer->setDomTree ( m_webViewer->toWebElement() );
  }
}

void Window::openTidyConfigApplication()
{
  QProcess::startDetached ( QString::fromUtf8 ( "qtidyrc" ) );
}

void Window::openFileDialog()
{
  QString htmlFile;
  QStringList filters;
  filters << trUtf8 ( "HTML Document %1" ).arg ( "*.html *.htm" );
  filters << trUtf8 ( "Markup Document %1" ).arg ( "*.xml *.xslt *.xbel" );

  QUrl url;
  url.setScheme ( QLatin1String ( "file" ) );

  htmlFile = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open HTML File" ),
             QString::null, filters.join ( ";;" ) );

  if ( htmlFile.isEmpty() )
    return;

  QFileInfo info ( htmlFile );
  if ( info.exists() )
  {
    url.setPath ( info.absoluteFilePath() );
    openFile ( url );
  }
}

void Window::openUrlDialog()
{
  OpenUrlDialog* dialog = new OpenUrlDialog ( this );
  connect ( dialog, SIGNAL ( openUrl ( const QUrl & ) ),
            this, SLOT ( openUrl ( const QUrl & ) ) );

  dialog->exec();
}

void Window::openFile ( const QUrl &url )
{
  if ( ! url.isValid() || url.scheme() != "file" )
    return;

  QFile fp ( url.path() );
  if ( fp.open ( QFile::ReadOnly ) )
  {
    QTextStream rc ( &fp );
    QByteArray buffer = rc.device()->readAll();
    fp.close();
    if ( ! buffer.isEmpty() )
    {
      QTextCodec* codec = QTextCodec::codecForHtml ( buffer, QTextCodec::codecForName ( "UTF-8" ) );
      QString data = codec->toUnicode ( buffer );
      m_sourceWidget->setSource ( data );
      m_webViewer->setUrl ( url.path() );
      m_settings->setValue ( QLatin1String ( "RecentFile" ), url );
    }
  }
}

void Window::openUrl ( const QUrl &url )
{
  if ( ! url.isValid() || url.scheme() != "http" )
    return;

  m_webViewer->setUrl ( url );
  m_settings->setValue ( QLatin1String ( "RecentUrl" ), url );
}

Window::~Window()
{}
