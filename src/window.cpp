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
#include "application.h"
#include "networkaccessmanager.h"
#include "addresstoolbar.h"
#include "keywordstoolbar.h"
#include "webviewer.h"
#include "sourcewidget.h"
#include "domviewer.h"
#include "tidymessanger.h"
#include "jsmessanger.h"
#include "bookmark.h"
#include "historymanager.h"
#include "historyitem.h"
#include "historymenu.h"
#include "openurldialog.h"
#include "aboutdialog.h"
#include "configdialog.h"
#include "statusbar.h"
#include "cookieview.h"
#include "headerview.h"
#include "formview.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
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

/* QtWebKit */
#include <QtWebKit/QWebInspector>
#include <QtWebKit/QWebPage>

/* QTidy */
#include <QTidy/QTidy>

Window::Window ( QSettings * settings )
    : QMainWindow()
    , m_settings ( settings )
{
  // Window Properties
  setWindowTitle ( trUtf8 ( "XHTML Debugger" ) );
  setObjectName ( "xhtmldbgwindow" );
  QIcon qTidyIcon ( QString::fromUtf8 ( ":/icons/qtidy.png" ) );
  setWindowIcon ( qTidyIcon );

  m_netManager = Application::networkAccessManager();

  // StatusBar
  m_statusBar = new StatusBar ( statusBar() );
  setStatusBar ( m_statusBar );

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

  // Show Document DomTree in DockWidget
  m_dockDomViewWidget = new QDockWidget ( this );
  m_dockDomViewWidget->setObjectName ( QLatin1String ( "domviewerdock" ) );
  m_dockDomViewWidget->setWindowTitle ( trUtf8 ( "DOM Viewer" ) );
  m_dockDomViewWidget->setWindowIcon ( qTidyIcon );
  m_dockDomViewWidget->setAllowedAreas ( ( m_dockDomViewWidget->allowedAreas() & ~Qt::TopDockWidgetArea ) );
  m_dockDomViewWidget->setFeatures ( ( m_dockDomViewWidget->features() & ~QDockWidget::DockWidgetFloatable ) );
  m_domViewer = new DomViewer ( m_dockDomViewWidget );
  m_dockDomViewWidget->setWidget ( m_domViewer );
  addDockWidget ( Qt::RightDockWidgetArea, m_dockDomViewWidget );

  // XHTML Messanger DockWidget
  m_tidyMessanger = new TidyMessanger ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_tidyMessanger );

  // JavaScript Messanger DockWidget
  m_jsMessanger = new JSMessanger ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_jsMessanger );

  // Show Cookie Information
  m_cookieView = new CookieView ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_cookieView );

  // Show Received Document Headers
  m_headerView = new HeaderView ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_headerView );

  // Show Posted Formsdata
  m_formView = new FormView ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_formView );

  // finalize WindowDesign
  createMenus();
  createToolBars();

  setCentralWidget ( m_centralWidget );

  // SIGNALS
  // connect ( , SIGNAL ( ), , SLOT ( ) );
  connect ( m_webViewer, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( requestsFinished ( bool ) ) );

  connect ( m_sourceWidget, SIGNAL ( clearMessages () ),
            m_tidyMessanger, SLOT ( clearItems () ) );

  connect ( m_sourceWidget, SIGNAL ( triggered ( const QTidy::QTidyDiagnosis & ) ),
            m_tidyMessanger, SLOT ( messages ( const QTidy::QTidyDiagnosis & ) ) );

  connect ( m_tidyMessanger, SIGNAL ( marking ( int, int ) ),
            m_sourceWidget, SLOT ( fetchBlock ( int, int ) ) );

  connect ( m_tidyMessanger, SIGNAL ( itemSelected () ),
            this, SLOT ( visibleSourceChanged () ) );

  connect ( m_netManager, SIGNAL ( netNotify ( const QString & ) ),
            m_statusBar, SLOT ( showMessage ( const QString & ) ) );

  connect ( m_netManager, SIGNAL ( receivedHostHeaders ( const QString &, const QMap<QString,QString> & ) ),
            m_headerView, SLOT ( setHeaders ( const QString &, const QMap<QString,QString> & ) ) );

  connect ( m_netManager, SIGNAL ( postedRefererData ( const QUrl &, const QStringList & ) ),
            m_formView, SLOT ( setPostedData ( const QUrl &, const QStringList & ) ) );

  // Load Settings
  restoreState ( m_settings->value ( "MainWindowState" ).toByteArray() );
  restoreGeometry ( m_settings->value ( "MainWindowGeometry" ).toByteArray() );

  QUrl fallback ( "http://www.hjcms.de" );
  QUrl recent = m_settings->value ( QLatin1String ( "RecentUrl" ), fallback ).toUrl();
  QUrl startup = m_settings->value ( QLatin1String ( "StartUpUrl" ), fallback ).toUrl();
  openUrl ( ( startup.isEmpty() ? recent : startup ) );

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
  connect ( actionNewEmptyPage, SIGNAL ( triggered () ), m_webViewer, SLOT ( addViewerTab () ) );

  // Bookmark/History Menues
  QIcon bookmarksIcon ( QIcon::fromTheme ( QLatin1String ( "bookmarks" ) ) );
  QMenu* m_bookmarkerMenu = m_menuBar->addMenu ( trUtf8 ( "Bookmarks" ) );
  m_menuBar->addMenu ( m_bookmarkerMenu );

  // Bookmark Menu
  m_bookmarkMenu = new Bookmark ( m_bookmarkerMenu );
  m_bookmarkerMenu->addMenu ( m_bookmarkMenu );
  connect ( m_bookmarkMenu, SIGNAL ( openBookmark ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );

  connect ( m_webViewer, SIGNAL ( addBookmark ( const QUrl &, const QString & ) ),
            m_bookmarkMenu, SLOT ( addBookmark ( const QUrl &, const QString & ) ) );

  // History Menu
  HistoryManager* m_historyManager = Application::historyManager();
  m_historyMenu = new HistoryMenu ( m_bookmarkerMenu );
  m_bookmarkerMenu->addMenu ( m_historyMenu );
  connect ( m_historyManager, SIGNAL ( updateHistoryMenu ( const QList<HistoryItem> & ) ),
            m_historyMenu, SLOT ( updateHistoryItems ( const QList<HistoryItem> & ) ) );
  connect ( m_historyMenu, SIGNAL ( openBookmark ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );

  // Bookmark Manager Action
  QIcon bookEditIcon ( icon.fromTheme ( QLatin1String ( "bookmarks-organize" ) ) );
  QAction* editorAction = m_bookmarkerMenu->addAction ( bookEditIcon, trUtf8 ( "Organize Bookmarks" ) );
  connect ( editorAction, SIGNAL ( triggered() ), m_bookmarkMenu, SLOT ( openBookmarkEditor() ) );

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
  connect ( actionConfigDialog, SIGNAL ( triggered() ), this, SLOT ( openConfigDialog() ) );

  // Show Enable/Disable Toolbars Menu
  m_viewBarsMenu = m_menuBar->addMenu ( trUtf8 ( "Display" ) );

  // Help and About Menu
  QIcon infoIcon = icon.fromTheme ( QLatin1String ( "documentinfo" ) );
  QMenu *m_aboutMenu = m_menuBar->addMenu ( infoIcon, trUtf8 ( "About" ) );
  QAction* actionAboutQt = m_aboutMenu->addAction ( infoIcon, trUtf8 ( "about Qt" ) );
  actionAboutQt->setMenuRole ( QAction::AboutQtRole );
  connect ( actionAboutQt, SIGNAL ( triggered() ), qApp, SLOT ( aboutQt() ) );

  AboutDialog* aboutDialog = new AboutDialog ( this );
  QAction* actionAboutHJCMS = m_aboutMenu->addAction ( infoIcon, trUtf8 ( "about hjcms" ) );
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
  connect ( m_keywordsToolBar, SIGNAL ( changed ( const QStringList & ) ),
            m_webViewer, SLOT ( keywords ( const QStringList & ) ) );

  addToolBar ( m_keywordsToolBar );

  // Add ToolBar View Actions to Display Menu
  m_viewBarsMenu->addAction ( m_actionsToolBar->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_settingsToolBar->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_addressToolBar->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_keywordsToolBar->toggleViewAction() );
  m_viewBarsMenu->addSeparator ();
  // Add QDockWidget View Actions to Display Menu
  m_viewBarsMenu->addAction ( m_tidyMessanger->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_jsMessanger->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_dockDomViewWidget->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_cookieView->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_headerView->toggleViewAction() );
  m_viewBarsMenu->addAction ( m_formView->toggleViewAction() );
}

void Window::closeEvent ( QCloseEvent *event )
{
  m_settings->setValue ( "MainWindowState", saveState() );
  m_settings->setValue ( "MainWindowGeometry", saveGeometry() );
  QMainWindow::closeEvent ( event );
}

void Window::paintEvent ( QPaintEvent * ev )
{
  Q_UNUSED ( ev )
  /* WARNING Currently we can not Implementation a Secondary
  * resizeEvent over QWebView Classes */
  m_statusBar->displayBrowserWidth ( m_webViewer->size() );
}

/**
* if page request is finished prepare all methodes
* for nested Widgets
* file:///home/heinemann/hjcms/libQTidy/tests/in_426885.html
*/
void Window::requestsFinished ( bool ok )
{
  if ( ok )
  {
    m_domViewer->setDomTree ( m_webViewer->toWebElement() );
    m_cookieView->cookiesFromUrl ( m_webViewer->getUrl() );
    // Make Secure
    QUrl::FormattingOptions options = ( QUrl::RemovePassword | QUrl::RemoveFragment );
    QUrl recent ( m_webViewer->getUrl().toString ( options ) );
    m_settings->setValue ( QLatin1String ( "RecentUrl" ), recent );
  }
}

void Window::setSource ( const QString &source )
{
  m_tidyMessanger->clearItems();
  if ( source.isEmpty() )
    return;

  m_sourceWidget->setSource ( source );
  // Is AutoCheck or AutoFormat Enabled?
  if ( m_settings->value ( QLatin1String ( "AutoFormat" ), false ).toBool() )
    m_sourceWidget->format();
  else if ( m_settings->value ( QLatin1String ( "AutoCheck" ), true ).toBool() )
    m_sourceWidget->check();
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

void Window::openConfigDialog()
{
  ConfigDialog* dialog = new ConfigDialog ( this, m_settings );
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
  if ( ! url.isValid() || ! url.scheme().contains ( QRegExp ( "http[s]?" ) ) )
    return;

  m_webViewer->setUrl ( url );
}

void Window::visibleSourceChanged()
{
  if ( m_centralWidget->currentIndex() != 1 )
    m_centralWidget->setCurrentIndex ( 1 );
}

JSMessanger* Window::JavaScriptMessanger() const
{
  return m_jsMessanger;
}

Window::~Window()
{}
