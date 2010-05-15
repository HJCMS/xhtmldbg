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

#include "version.h"
#include "window.h"
#include "application.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "addresstoolbar.h"
#include "keywordstoolbar.h"
#include "webviewer.h"
#include "sourcewidget.h"
#include "tidymessanger.h"
#include "jsmessanger.h"
#include "appevents.h"
#include "cssvalidator.h"
#include "bookmark.h"
#include "historymanager.h"
#include "historyitem.h"
#include "historymenu.h"
#include "openurldialog.h"
#include "aboutdialog.h"
#include "configdialog.h"
#include "statusbar.h"
#include "dominspector.h"
#include "cookiesdock.h"
#include "headerdock.h"
#include "autoreloadmenu.h"
#include "autoreloader.h"
/* DBus */
#include "xhtmldbgadaptor.h"
/* Interface */
#include "xhtmldbgplugger.h"
#include "xhtmldbgplugininfo.h"
#include "xhtmldbginterface.h"


/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtCore/QProcess>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QKeySequence>
#include <QtGui/QPalette>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebPage>

/* QTidy */
#include <QTidy/QTidy>

Window::Window ( QSettings * settings )
    : QMainWindow()
    , m_settings ( settings )
    , qTidyIcon ( QIcon ( QString::fromUtf8 ( ":/icons/qtidy.png" ) ) )
{
  // Standard Fenster optionen
  setObjectName ( "xhtmldbgwindow" );
  setWindowIcon ( qTidyIcon );

  QColor bgColor = palette().color ( QPalette::Background );
  setStyleSheet ( QString ( "QToolTip{background-color:%1;padding:1px;}" ).arg ( bgColor.name() ) );

  QString winTitle = QString ( "XHTML Debugger (v%1)" ).arg ( XHTMLDBG_VERSION_STRING );
  setWindowTitle ( winTitle );

  m_netManager = Application::networkAccessManager();

  // Zentrales TabWidget für Quelltext und Browser
  // TabWidgets {
  m_centralWidget = new QTabWidget ( this );
  m_centralWidget->setObjectName ( QLatin1String ( "centralwidget" ) );
  m_centralWidget->setTabPosition ( QTabWidget::South );
  m_centralWidget->setTabsClosable ( false );

  // Browser Anzeige
  m_webViewer = new WebViewer ( m_centralWidget );
  m_centralWidget->insertTab ( 0, m_webViewer, trUtf8 ( "Browser" ) );
  m_centralWidget->setTabIcon ( 0, qTidyIcon );
  m_centralWidget->setCurrentIndex ( 0 );

  // Quelltext Anzeige
  m_sourceWidget = new SourceWidget ( m_centralWidget );
  m_centralWidget->insertTab ( 1, m_sourceWidget, trUtf8 ( "Source" ) );
  m_centralWidget->setTabIcon ( 1, qTidyIcon );
  // } TabWidgets

  // StatusBar
  m_statusBar = new StatusBar ( statusBar() );
  setStatusBar ( m_statusBar );

  // Automatische Seiten Aktualisierung
  m_autoReloader = new AutoReloader ( this );

  // DockWidgets {
  // Ansicht Dokumenten Baum
  m_domInspector = new DomInspector ( this, m_settings );
  addDockWidget ( Qt::RightDockWidgetArea, m_domInspector );

  // QTidy Nachrichtenfenster
  m_tidyMessanger = new TidyMessanger ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_tidyMessanger, Qt::Horizontal );

  // Programm Nachrichtenfenster für interne Nachrichten
  m_appEvents = new AppEvents ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_appEvents, Qt::Horizontal );

  // JavaScript Nachrichtenfenster
  m_jsMessanger = new JSMessanger ( this );
  addDockWidget ( Qt::BottomDockWidgetArea, m_jsMessanger, Qt::Horizontal );

  // CSS Validierer Prozess
  m_cssValidator = new CSSValidator ( this, m_settings );
  addDockWidget ( Qt::BottomDockWidgetArea, m_cssValidator, Qt::Horizontal );

  // Zeige Keks Informationen
  m_cookiesDock = new CookiesDock ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_cookiesDock );

  // Zeige Datenköpfe für CGI GET/POST und HTTP Header an.
  m_headerDock = new HeaderDock ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_headerDock );
  // } DockWidgets

  // WARNING Muss vor createMenus() und createToolBars() kommen!
  // xhtmldbgplugger {
  plugger = new xhtmldbgplugger ( this );
  // } xhtmldbgplugger

  // erstelle alle Menü einträge
  createMenus();

  /* WARNING Muss wegen dem HistoryManager nach createMenus() kommen. */
  createToolBars();

  // Design abschliessen
  setCentralWidget ( m_centralWidget );

  // bei DBus anmelden
  m_xhtmldbgAdaptor = new XHtmldbgAdaptor ( this );
  m_xhtmldbgAdaptor->registerSubObject ( m_domInspector );

  // jetzt die Plugins laden
  // xhtmldbgplugger {
  registerPlugins();
  // } xhtmldbgplugger

  // WebViewer {
  connect ( m_webViewer, SIGNAL ( loadFinished ( bool ) ),
            this, SLOT ( requestsFinished ( bool ) ) );

  connect ( m_webViewer, SIGNAL ( hitTestResult ( const QWebElement & ) ),
            m_domInspector, SLOT ( findItem ( const QWebElement & ) ) );

  connect ( m_webViewer, SIGNAL ( statusBarMessage ( const QString & ) ),
            m_statusBar, SLOT ( showMessage ( const QString & ) ) );

  connect ( m_webViewer, SIGNAL ( bytesLoaded ( qint64 ) ),
            m_statusBar, SLOT ( setLoadedPageSize ( qint64 ) ) );
  // } WebViewer
  // SourceWidget {
  connect ( m_sourceWidget, SIGNAL ( clearMessages () ),
            m_tidyMessanger, SLOT ( clearItems () ) );

  connect ( m_sourceWidget, SIGNAL ( triggered ( const QTidy::QTidyDiagnosis & ) ),
            m_tidyMessanger, SLOT ( messages ( const QTidy::QTidyDiagnosis & ) ) );
  // } SourceWidget
  // TidyMessanger {
  connect ( m_tidyMessanger, SIGNAL ( marking ( int, int ) ),
            m_sourceWidget, SLOT ( fetchBlock ( int, int ) ) );

  connect ( m_tidyMessanger, SIGNAL ( itemSelected () ),
            this, SLOT ( visibleSourceChanged () ) );

  connect ( m_tidyMessanger, SIGNAL ( itemsChanged ( bool ) ),
            m_statusBar, SLOT ( notice ( bool ) ) );
  // } TidyMessanger
  // NetworkAccessManager {
  connect ( m_netManager, SIGNAL ( netNotify ( const QString & ) ),
            m_appEvents, SLOT ( warningMessage ( const QString & ) ) );

  connect ( m_netManager, SIGNAL ( receivedHostHeaders ( const QString &, const QMap<QString,QString> & ) ),
            m_headerDock, SLOT ( setHeaderData ( const QString &, const QMap<QString,QString> & ) ) );

  connect ( m_netManager, SIGNAL ( postedRefererData ( const QUrl &, const QStringList & ) ),
            m_headerDock, SLOT ( setPostedData ( const QUrl &, const QStringList & ) ) );

  connect ( m_netManager, SIGNAL ( postReplySource ( const QString & ) ),
            this, SLOT ( setSource ( const QString & ) ) );
  // } NetworkAccessManager
  // AutoReload {
  connect ( m_autoReloadMenu, SIGNAL ( reloadInterval ( int ) ),
            m_autoReloader, SLOT ( setInterval ( int ) ) );
  connect ( m_autoReloader, SIGNAL ( status ( int, int ) ),
            m_statusBar, SLOT ( timerStatus ( int, int ) ) );
  connect ( m_autoReloader, SIGNAL ( reload () ),
            m_webViewer, SLOT ( refresh() ) );
  // } AutoReload

  // Wenn noch kein Eintrag vorhanden öffne about:welcome
  QUrl startup = m_settings->value ( QLatin1String ( "StartUpUrl" ) ).toUrl();
  QUrl recentUrl = m_settings->value ( QLatin1String ( "RecentUrl" ) ).toUrl();
  if ( startup.isValid() && ! startup.isEmpty() )
  {
    openUrl ( startup );
  }
  else if ( recentUrl.isValid() && ! recentUrl.isEmpty() )
  {
    if ( recentUrl.scheme().contains ( "http" ) )
      openUrl ( recentUrl );
    else
      m_webViewer->setAboutPage ( QLatin1String ( "welcome" ) );
  }
  else
    m_webViewer->setAboutPage ( QLatin1String ( "welcome" ) );

  // lade Fenster Einstellungen
  restoreState ( m_settings->value ( "MainWindowState" ).toByteArray() );
  restoreGeometry ( m_settings->value ( "MainWindowGeometry" ).toByteArray() );
  // zum abschluss den focus auf den Browser setzen
  m_webViewer->setWebFocus();
}

void Window::createMenus()
{
  m_menuBar = menuBar ();

  QIcon icon;

  // Main Menu
  m_applicationMenu = m_menuBar->addMenu ( trUtf8 ( "&Application" ) );
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
  m_debuggerMenu = m_menuBar->addMenu ( trUtf8 ( "&Debugger" ) );

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
  m_viewMenu = m_menuBar->addMenu ( trUtf8 ( "&Browser" ) );

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

  // Autoreload Menu Aciotn
  m_autoReloadMenu = new AutoReloadMenu ( m_viewMenu );
  m_viewMenu->addMenu ( m_autoReloadMenu );

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
  m_historyManager = Application::historyManager();
  m_historyMenu = new HistoryMenu ( m_bookmarkerMenu );
  m_bookmarkerMenu->addMenu ( m_historyMenu );
  connect ( m_historyManager, SIGNAL ( updateHistoryMenu ( const QList<HistoryItem> & ) ),
            m_historyMenu, SLOT ( updateHistoryItems ( const QList<HistoryItem> & ) ) );
  connect ( m_historyMenu, SIGNAL ( openBookmark ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );

  // Bookmark Manager Action
  QIcon bookEditIcon ( icon.fromTheme ( QLatin1String ( "bookmarks-organize" ) ) );
  QAction* editorAction = m_bookmarkerMenu->addAction ( bookEditIcon, trUtf8 ( "Organize Bookmarks" ) );
  editorAction->setShortcut ( Qt::CTRL + Qt::Key_B );
  connect ( editorAction, SIGNAL ( triggered() ), m_bookmarkMenu, SLOT ( openBookmarkEditor() ) );

  // Configuration Menu
  m_configurationMenu = m_menuBar->addMenu ( trUtf8 ( "S&ettings" ) );
  // Action Open qtidyrc
  actionTidyConfig = m_configurationMenu->addAction ( trUtf8 ( "Configure Tidyrc" ) );
  actionTidyConfig->setIcon ( icon.fromTheme ( QLatin1String ( "configure-toolbars" ) ) );
  connect ( actionTidyConfig, SIGNAL ( triggered() ),
            this, SLOT ( openTidyConfigApplication() ) );

  // Action open Configuration Dialog
  actionConfigDialog = m_configurationMenu->addAction ( trUtf8 ( "Configure" ) );
  actionConfigDialog->setIcon ( icon.fromTheme ( QLatin1String ( "configure" ) ) );
  connect ( actionConfigDialog, SIGNAL ( triggered() ), this, SLOT ( openConfigDialog() ) );

  // Plugin Menu
  m_pluginMenu = m_menuBar->addMenu ( trUtf8 ( "Extensions" ) );

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
  connect ( m_historyManager, SIGNAL ( updateHistoryMenu ( const QList<HistoryItem> & ) ),
            m_addressToolBar, SLOT ( updateHistoryItems ( const QList<HistoryItem> & ) ) );
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
  QIcon icon = QIcon::fromTheme ( "preferences-system-windows-actions" );
  QMenu* viewToolbarsMenu = m_viewBarsMenu->addMenu ( trUtf8 ( "Toolbars" ) );
  viewToolbarsMenu->setIcon ( icon );
  viewToolbarsMenu->addAction ( m_actionsToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_settingsToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_addressToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_keywordsToolBar->toggleViewAction() );
  viewToolbarsMenu->addSeparator ();

  // Add TOP|BOTTOM QDockWidget View Actions to Display Menu
  QMenu* impartationsMenu = m_viewBarsMenu->addMenu ( trUtf8 ( "Impartations" ) );
  impartationsMenu->setIcon ( icon );
  impartationsMenu->addAction ( m_tidyMessanger->toggleViewAction() );
  impartationsMenu->addAction ( m_jsMessanger->toggleViewAction() );
  impartationsMenu->addAction ( m_appEvents->toggleViewAction() );
  impartationsMenu->addAction ( m_cssValidator->toggleViewAction() );

  // Add RIGHT|LEFT QDockWidget View Actions to Display Menu
  QMenu* inspectorsMenu = m_viewBarsMenu->addMenu ( trUtf8 ( "Inspectors" ) );
  inspectorsMenu->setIcon ( icon );
  inspectorsMenu->addAction ( m_domInspector->toggleViewAction() );
  inspectorsMenu->addAction ( m_cookiesDock->toggleViewAction() );
  inspectorsMenu->addAction ( m_headerDock->toggleViewAction() );

  // Plugin Menu
  m_diplayPlugins = m_viewBarsMenu->addMenu ( trUtf8 ( "Extensions" ) );
  m_diplayPlugins->setIcon ( icon );
}

/**
* Standard closeEvent für das Speichern von
* Fenster- Geometrie und Status.
*/
void Window::closeEvent ( QCloseEvent *event )
{
  plugins.clear(); // Vector Leeren
  m_settings->setValue ( "MainWindowState", saveState() );
  m_settings->setValue ( "MainWindowGeometry", saveGeometry() );
  QMainWindow::closeEvent ( event );
}

/**
* Eigentlich bietet QWebPage ja das signal repaintRequest
* Diese wird aber permanent bei jedem Seiten update aufgerufen.
* Ich finde diese Methode daher etwas besser.
* Sie wird nur aufgerufen wenn sich bei Fenster, Splitter,
* DockWidget etc. die Bereiche verändern.
* Ich habe hier absichtlich kein resizeEvent verwendet weil
* das Seltsamerweise zu einem Schwarzen WebViewer führt.
*/
void Window::paintEvent ( QPaintEvent * ev )
{
  Q_UNUSED ( ev )
  /* WARNING Currently we can not Implementation a Secondary
  * resizeEvent over QWebView Classes */
  m_statusBar->displayBrowserWidth ( m_webViewer->size() );
}

/**
* Prüft ob das DockWidget ein tab besitzt und sichtbar ist.
* Wenn es ein Tab ist, dann wird es nach vorne geholt.
*/
void Window::tabifyDockedWidgetUp ( QDockWidget *dockWidget )
{
  foreach ( QDockWidget* toHide, tabifiedDockWidgets ( dockWidget ) )
  {
    tabifyDockWidget ( toHide, dockWidget );
  }
}

/**
* Beim Start nach Plugins suchen und diese entsprechent
* dem type in die einzelnen Menüs einfügen.
* @ref m_pluginMenu Im TopLevel MenuBar
* @ref m_diplayPlugins Im Anzeigen Menü
*/
void Window::registerPlugins()
{
  QIcon icon = QIcon::fromTheme ( QLatin1String ( "preferences-plugin" ) );
  plugins.clear();
  foreach ( xhtmldbg::Interface* plug, plugger->pluginsByType ( xhtmldbg::PluginInfo::PopUp ) )
  {
    xhtmldbg::PluginInfo* info = plug->pluginInfo();
    if ( info )
    {
      plugins.push_back ( plug );
      QAction* ac = m_pluginMenu->addAction ( info->getGenericName() );
      ac->setObjectName ( QString ( "plugin_action_%1" ).arg ( info->getName() ) );
      ac->setIcon ( icon );
      ac->setStatusTip ( info->getDescription() );
      connect ( ac, SIGNAL ( triggered () ), plug, SLOT ( proccess () ) );
    }
  }
}

/**
* Diese Methode wird von QWebView::loadFinished aufgerufen.
* Erst wenn das ergebnis true ergibt werden folgendes eingebunden:
* @li DomTree::setDomTree
* @li CookiesDock::cookiesFromUrl
* @li QSettings::setValue (RecentUrl)
* @li An alle geladenen Plugins die URL übergeben.
* Beim setzen von @em RecentUrl werden Passwörter und Anker entfernt.
*/
void Window::requestsFinished ( bool ok )
{
  if ( ok )
  {
    m_domInspector->setDomTree ( m_webViewer->toWebElement() );
    m_cookiesDock->cookiesFromUrl ( m_webViewer->getUrl() );
    m_cssValidator->addForValidation ( m_webViewer->getUrl() );
    // Make Secure
    QUrl::FormattingOptions options = ( QUrl::RemovePassword | QUrl::RemoveFragment );
    QUrl recent ( m_webViewer->getUrl().toString ( options ) );
    m_settings->setValue ( QLatin1String ( "RecentUrl" ), recent );
    // An alle Plugins die Url übergeben
    for ( int i = 0; i < plugins.size(); ++i )
    {
      plugins.at ( i )->setUrl ( m_webViewer->getUrl() );
    }
  }
}

/**
* Nachrichten an @ref AppEvents übermitteln.
* Wenn das Dock nicht sichtbar ist wird es nach vorne geholt!
*/
void Window::setApplicationMessage ( const QString &message, bool warning )
{
  if ( message.isEmpty() )
    return;

  if ( warning )
    m_appEvents->warningMessage ( message );
  else
    m_appEvents->statusMessage ( message );

  tabifyDockedWidgetUp ( m_appEvents );
}

/**
* Nachrichten an @ref JSMessanger übermitteln.
* Wenn das Dock nicht sichtbar ist wird es nach vorne geholt!
*/
void Window::setJavaScriptMessage ( const QString &message )
{
  if ( message.isEmpty() )
    return;

  m_jsMessanger->insertMessage ( message );
  tabifyDockedWidgetUp ( m_jsMessanger );
}

/**
* Versuche den StyleSheet für diese Url beim
* Validierer direkt auf zu rufen!
* Laufende Prozesse werden nicht angehalten!
*/
void Window::checkStyleSheet ( const QUrl &url )
{
  m_statusBar->showMessage ( trUtf8 ( "Check StyleSheet for %1" ).arg ( url.host() ), 1024 );
  m_cssValidator->forceValidation ( url );
}

/**
* @li Hier wird der Quelltext in @class SourceView eingefügt.
*  Gleichzeitig @ref TidyMessanger::clearItems aufgerufen.
* @li Danach wird der Quelletext an die geladenen Plugins übergeben.
* @li Wenn der überreichte Quelltext nicht leer ist wird in den
*  Einstellungen nachgesehen ob @em AutoFormat oder @em AutoCheck
*  aktiviert sind und entsprechend ausgeführt.
*/
bool Window::setSource ( const QString &source )
{
  m_tidyMessanger->clearItems();

  if ( source.isEmpty() )
    return false;

  // Quelltext einfügen
  m_sourceWidget->setSource ( source );

  // An alle Plugins den Quelltext übergeben
  for ( int i = 0; i < plugins.size(); ++i )
  {
    plugins.at ( i )->setContent ( source );
  }

  // Ist AutoCheck oder AutoFormat aktiviert?
  if ( m_settings->value ( QLatin1String ( "AutoFormat" ), false ).toBool() )
    m_sourceWidget->format();
  else if ( m_settings->value ( QLatin1String ( "AutoCheck" ), true ).toBool() )
    m_sourceWidget->check();

  return true;
}

/**
* Standard SLOT für den Aufruf des Programms qtidyrc
*/
void Window::openTidyConfigApplication()
{
  QProcess::startDetached ( QLatin1String ( "qtidyrc" ) );
}

/**
* Standard Dialog für das öffnen einer HTML Datei.
* Nach dem Dialog wird @ref openFile aufgerufen.
*/
void Window::openFileDialog()
{
  QString htmlFile;
  QStringList filters;
  filters << trUtf8 ( "HTML Document %1" ).arg ( "*.html *.htm *.xhtml *.html.*" );
  filters << trUtf8 ( "Markup Document %1" ).arg ( "*.xml *.xslt *.xbel" );
  filters << trUtf8 ( "Text Document %1" ).arg ( "*.txt *.text" );
  filters << trUtf8 ( "Unsupported Document Formats %1" ).arg ( "*.*" );

  QString lastDirectory = m_settings->value ( QLatin1String ( "RecentDirectory" ) ).toString();

  QUrl url;
  url.setScheme ( QLatin1String ( "file" ) );

  htmlFile = QFileDialog::getOpenFileName ( this, trUtf8 ( "Open HTML File" ),
             lastDirectory, filters.join ( ";;" ) );

  if ( htmlFile.isEmpty() )
    return;

  QFileInfo info ( htmlFile );
  if ( info.exists() )
  {
    url.setPath ( info.absoluteFilePath() );
    if ( openFile ( url ) )
    {
      m_webViewer->setUrl ( url.path() );
      m_settings->setValue ( QLatin1String ( "RecentUrl" ), url );
      m_settings->setValue ( QLatin1String ( "RecentDirectory" ), info.absolutePath() );
    }
  }
}

/**
* Standard Dialog für das öffnen einer URL
*/
void Window::openUrlDialog()
{
  OpenUrlDialog* dialog = new OpenUrlDialog ( this );
  connect ( dialog, SIGNAL ( openUrl ( const QUrl & ) ),
            this, SLOT ( openUrl ( const QUrl & ) ) );

  dialog->exec();
}

/**
* Standard Dialog für das öffnen der Konfiguration.
* Wenn kein abbruch werden Cookies neu eingelesen.
*/
void Window::openConfigDialog()
{
  ConfigDialog* dialog = new ConfigDialog ( this, m_settings );
  if ( dialog->exec() )
  {
    m_netManager->cookieJar()->reload();
  }
  delete dialog;
}

/**
* Wird von @ref openFileDialog verwendet um zu prüfen ob es
* sich um ein file Schema handelt und existiert.
* Danach wird sie in ein QTextStream eingelesen und weiter
* an @ref setSource, der Pfad @ref WebViewer::setUrl gegeben.
* Bei den Einstellungen wird @em RecentUrl modifiziert.
*/
bool Window::openFile ( const QUrl &url )
{
  if ( ! url.isValid() || url.scheme() != "file" )
    return false;

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
      return setSource ( data );
    }
  }
  return false;
}

/**
* Primitive überprüfung ob es sich um eine Url handelt.
* Wenn ja wird @ref WebViewer::setUrl aufgerufen.
*/
bool Window::openUrl ( const QUrl &url )
{
  if ( ! url.isValid() )
    return false;

  if ( url.scheme().contains ( "file" ) )
    return false; // Siehe openFile

  if ( ! url.scheme().contains ( QRegExp ( "http[s]?" ) ) )
    return false;

  m_webViewer->setUrl ( url );
  return true;
}

/**
* Methode zum prüfen ob das Quelltext Fenster sichtbar ist.
* Wenn nicht wird es nach vorne geholt.
*/
void Window::visibleSourceChanged()
{
  if ( m_centralWidget->currentIndex() != 1 )
    m_centralWidget->setCurrentIndex ( 1 );
}

Window::~Window()
{}
