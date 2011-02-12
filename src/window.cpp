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
#include "window.h"
#include "application.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "downloadmanager.h"
#include "addresstoolbar.h"
#include "keywordstoolbar.h"
#include "zoombar.h"
#include "webviewer.h"
#include "sourcecache.h"
#include "sourcewidget.h"
#include "alternatelinkreader.h"
#include "tidymessanger.h"
#include "jsmessanger.h"
#include "appevents.h"
#include "cssvalidator.h"
#include "bookmark.h"
#include "pagehistory.h"
#include "historymanager.h"
#include "historyitem.h"
#include "historymenu.h"
#include "openfiledialog.h"
#include "settargetdialog.h"
#include "openurldialog.h"
#include "about.h"
#include "configuration.h"
#include "statusbar.h"
#include "dominspector.h"
#include "webinspector.h"
#include "headerdock.h"
#include "autoreloadmenu.h"
#include "autoreloader.h"
#include "colorpicker.h"
#include "colorpickerbutton.h"
#include "geolocation.h"
#include "websettings.h"
/* Experimental */
#ifdef _XHTMLDBG_EXPERIMENTAL
# include "resizeportbuttons.h"
#endif
/* Interface */
#include "plugger.h"
#include "plugininfo.h"
#include "interface.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>
#include <QtCore/QSignalMapper>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QFileDialog>
#include <QtGui/QKeySequence>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* QtNetwork */
#include <QtNetwork/QAbstractNetworkCache>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebPage>

/* QTidy */
#include <QTidy/QTidy>

/* QtSql */
#include <QtSql/QSqlDatabase>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

#define URL_SCHEME_PATTERN QRegExp ( "^(http[s]?|file)" )

Window::Window ( Settings * settings )
    : KMainWindow()
    , m_settings ( settings )
    , xhtmldbgIcon ( KIcon ( "xhtmldbg" ) )
{
  // Standard Fenster optionen
  setObjectName ( "Window" );
  setWindowIcon ( xhtmldbgIcon );
  setAutoSaveSettings ( "xhtmldbg", false );

  QString winTitle = QString ( "XHTML Debugger (v%1)" ).arg ( XHTMLDBG_VERSION_STRING );
  setWindowTitle ( winTitle );

  // Quelltext Zwichen Speicher initialisieren
  m_sourceCache = new SourceCache ( this );

  m_netManager = Application::networkAccessManager();

  /* Das lesen aller Web Einstellungen muss vor dem ersten erstellen ein tabs erfolgen. */
  WebSettings websettings ( this );
  websettings.setDefaults();

  // Zentrales TabWidget für Quelltext und Browser
  // TabWidgets {
  m_centralWidget = new KTabWidget ( this );
  m_centralWidget->setObjectName ( QLatin1String ( "centralwidget" ) );
  m_centralWidget->setTabPosition ( QTabWidget::South );
  m_centralWidget->setTabsClosable ( false );

  // Browser Anzeige
  m_webViewer = new WebViewer ( m_centralWidget );
  // NOTE Wir brauchen eine gültige WebPage für den WebInspector
  m_webViewer->setAboutPage ( "welcome" );
  // Viewer einfügen
  m_centralWidget->insertTab ( 0, m_webViewer, i18n ( "Browser" ) );
  m_centralWidget->setTabIcon ( 0, xhtmldbgIcon );
  m_centralWidget->setCurrentIndex ( 0 );

  // Quelltext Anzeige
  m_sourceWidget = new SourceWidget ( m_centralWidget );
  m_centralWidget->insertTab ( 1, m_sourceWidget, i18n ( "Source" ) );
  m_centralWidget->setTabIcon ( 1, xhtmldbgIcon );

  // Tabulatur unten Rechts
  QWidget* tabCornerBottomWidget = new QWidget ( m_centralWidget );
  tabCornerBottomWidget->setObjectName ( QLatin1String ( "tabcornerbottomwidget" ) );
  tabCornerBottomWidget->setContentsMargins ( 0, 0, 0, 0 );

  QHBoxLayout* tabCornerBottomWidgetLayout = new QHBoxLayout ( tabCornerBottomWidget );
  tabCornerBottomWidgetLayout->setObjectName ( QLatin1String ( "tabcornerbottomwidgetlayout" ) );
  tabCornerBottomWidgetLayout->setContentsMargins ( 0, 0, 0, 0 );

#ifdef _XHTMLDBG_EXPERIMENTAL
  // Browser Fensterbreite Manipulieren
  ResizePortButtons* m_resizePortButtons = new ResizePortButtons ( tabCornerBottomWidget );
  // TODO Resize arbeitet im Moment nicht korrekt, deshalb nur bei mir aktiv :-/
  m_resizePortButtons->setEnabled ( false );
  tabCornerBottomWidgetLayout->addWidget ( m_resizePortButtons );
#endif

  // Farben Pipette Ein/Ausschalten
  ColorPickerButton* m_colorPickerButton = new ColorPickerButton ( tabCornerBottomWidget );
  tabCornerBottomWidgetLayout->addWidget ( m_colorPickerButton );

  // Gebiete mit der IP Adresse ermitteln
  m_geoLocation = new GeoLocation ( tabCornerBottomWidget, m_settings );
  tabCornerBottomWidgetLayout->addWidget ( m_geoLocation );

  // RSS/ATOM/RDF Validierer
  m_alternateLinkReader = new AlternateLinkReader ( tabCornerBottomWidget );
  tabCornerBottomWidgetLayout->addWidget ( m_alternateLinkReader );

  tabCornerBottomWidget->setLayout ( tabCornerBottomWidgetLayout );
  m_centralWidget->setCornerWidget ( tabCornerBottomWidget, Qt::BottomRightCorner );
  // } TabWidgets

  // StatusBar
  m_statusBar = new StatusBar ( statusBar() );
  setStatusBar ( m_statusBar );

  // Automatische Seiten Aktualisierung
  m_autoReloader = new AutoReloader ( this );

  // DownloadManager {
  m_downloadManager = new DownloadManager ( this, m_settings );
  addDockWidget ( Qt::RightDockWidgetArea, m_downloadManager );
  // } DownloadManager

  // DockWidgets {
  // Ansicht Dokumenten Baum
  m_domInspector = new DomInspector ( m_settings, this );
  addDockWidget ( Qt::RightDockWidgetArea, m_domInspector );

  // WebInspector
  // NOTE Bitte sicher stellen das eine Page vorhanden ist!!!
  m_webInspector = new WebInspector ( m_webViewer->startPage(), this );
  addDockWidget ( Qt::RightDockWidgetArea, m_webInspector );

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

  // ColorPicker
  m_colorPicker = new ColorPicker ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_colorPicker );

  // Zeige Datenköpfe für CGI GET/POST und HTTP Header an.
  m_headerDock = new HeaderDock ( this );
  addDockWidget ( Qt::RightDockWidgetArea, m_headerDock );
  // } DockWidgets

  // WARNING Muss vor createMenus() und createToolBars() kommen!
  // xhtmldbg::Plugger {
  m_plugger = new xhtmldbg::Plugger ( this );
  // } xhtmldbg::Plugger

  // erstelle alle Menü einträge
  createMenus();

  /* WARNING Muss wegen dem HistoryManager nach createMenus() kommen. */
  createToolBars();

  // Design abschliessen
  setCentralWidget ( m_centralWidget );

  // WebViewer {
  connect ( m_webViewer, SIGNAL ( currentChanged ( int ) ),
            this, SLOT ( tabChanged ( int ) ) );

  connect ( m_webViewer, SIGNAL ( elementsTree ( const QUrl &, const QWebElement & ) ),
            m_domInspector, SLOT ( setDomTree ( const QUrl &, const QWebElement & ) ) );

  connect ( m_webViewer, SIGNAL ( elementsTree ( const QUrl &, const QWebElement & ) ),
            m_alternateLinkReader, SLOT ( setDomTree ( const QUrl &, const QWebElement & ) ) );

  connect ( m_webViewer, SIGNAL ( hitTestResult ( const QWebElement & ) ),
            m_domInspector, SLOT ( findItem ( const QWebElement & ) ) );

  connect ( m_webViewer, SIGNAL ( statusBarMessage ( const QString & ) ),
            m_statusBar, SLOT ( showMessage ( const QString & ) ) );

  connect ( m_webViewer, SIGNAL ( loadStarted () ),
            m_headerDock, SLOT ( clearAll() ) );

  connect ( m_webViewer, SIGNAL ( loadStarted () ),
            m_sourceWidget, SLOT ( restore() ) );

  connect ( m_webViewer, SIGNAL ( bytesLoaded ( qint64 ) ),
            m_statusBar, SLOT ( setLoadedPageSize ( qint64 ) ) );
  // } WebViewer
  // SourceWidget {
  connect ( m_sourceWidget, SIGNAL ( clearMessages () ),
            m_tidyMessanger, SLOT ( clearItems () ) );

  connect ( m_sourceWidget, SIGNAL ( triggered ( const QTidy::QTidyDiagnosis & ) ),
            m_tidyMessanger, SLOT ( messages ( const QTidy::QTidyDiagnosis & ) ) );
  // } SourceWidget
  // DomInspector {
  connect ( m_domInspector, SIGNAL ( errorMessage ( const QString & ) ),
            this, SLOT ( setApplicationMessage ( const QString & ) ) );
  // } DomInspector
  // WebInspector {
  connect ( m_webViewer, SIGNAL ( pageEntered ( QWebPage * ) ),
            m_webInspector, SLOT ( setPage ( QWebPage * ) ) );
  connect ( m_webInspector, SIGNAL ( errorMessage ( const QString & ) ),
            this, SLOT ( setApplicationMessage ( const QString & ) ) );
  // } WebInspector
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

  connect ( m_netManager, SIGNAL ( receivedHostHeaders ( const QUrl &, const QMap<QString,QString> & ) ),
            m_headerDock, SLOT ( setHeaderData ( const QUrl &, const QMap<QString,QString> & ) ) );

  connect ( m_netManager, SIGNAL ( postedRefererData ( const QUrl &, const QStringList & ) ),
            m_headerDock, SLOT ( setPostedData ( const QUrl &, const QStringList & ) ) );

  connect ( m_netManager, SIGNAL ( urlLoadFinished ( const QUrl & ) ),
            this, SLOT ( intimateWidgets ( const QUrl & ) ) );

  // NOTE postReplySource muss in der Page gesetzt werden!
  connect ( m_netManager, SIGNAL ( localReplySource ( const QUrl &, const QString & ) ),
            this, SLOT ( setSource ( const QUrl &, const QString & ) ) );
  // } NetworkAccessManager
  // AutoReload {
  connect ( m_autoReloadMenu, SIGNAL ( reloadInterval ( int ) ),
            m_autoReloader, SLOT ( setInterval ( int ) ) );
  connect ( m_autoReloader, SIGNAL ( status ( int, int ) ),
            m_statusBar, SLOT ( timerStatus ( int, int ) ) );
  connect ( m_autoReloader, SIGNAL ( reload () ),
            m_webViewer, SLOT ( refresh() ) );
  // } AutoReload
  // ColorPicker {
  connect ( m_colorPickerButton, SIGNAL ( clicked ( bool ) ),
            m_colorPicker, SLOT ( tapping ( bool ) ) );
  // } ColorPicker
#ifdef _XHTMLDBG_EXPERIMENTAL
  // ResizePortButtons {
  connect ( m_resizePortButtons, SIGNAL ( itemClicked ( int ) ),
            m_webViewer, SLOT ( setViewerWidth ( int ) ) );
  // } ResizePortButtons
#endif

  // xhtmldbg::Plugger {
  // jetzt die Plugins laden
  registerPlugins();
  // } xhtmldbg::Plugger

  // lade Fenster Einstellungen
  restoreState ( m_settings->value ( "Window/MainWindowState" ).toByteArray() );
  restoreGeometry ( m_settings->value ( "Window/MainWindowGeometry" ).toByteArray() );

  // den focus auf den Browser setzen
  m_webViewer->setWebFocus();
  // zuletzt Verwendete Seiten laden
  loadPageHistory();
}

/**
* Alle Menüeinträge einfügen und die Tastenkürzel setzen.
*/
void Window::createMenus()
{
  m_menuBar = menuBar ();

  // Main Menu
  m_applicationMenu = m_menuBar->addMenu ( i18n ( "&Application" ) );
  m_applicationMenu ->setObjectName ( QLatin1String ( "applicationmenu" ) );

  // Action Open URL Dialog
  actionOpenUrl = m_applicationMenu->addAction ( i18n ( "Open Url" ) );
  actionOpenUrl->setStatusTip ( i18n ( "Load Document from Url" ) );
  actionOpenUrl->setShortcut ( Qt::CTRL + Qt::SHIFT + Qt::Key_O );
  actionOpenUrl->setIcon ( KIcon ( "document-open-remote" ) );
  connect ( actionOpenUrl, SIGNAL ( triggered() ), this, SLOT ( openUrlDialog() ) );

  // Action Open File from Location
  actionOpenHtml = m_applicationMenu->addAction ( i18n ( "Open Html File" ) );
  actionOpenHtml->setStatusTip ( i18n ( "Open Html from System" ) );
  actionOpenHtml->setShortcut ( Qt::CTRL + Qt::Key_O );
  actionOpenHtml->setIcon ( KIcon ( "document-open" ) );
  connect ( actionOpenHtml, SIGNAL ( triggered() ), this, SLOT ( openFileDialog() ) );

  // Action Application Exit
  actionQuit = m_applicationMenu->addAction ( i18n ( "Quit" ) );
  actionQuit->setStatusTip ( i18n ( "Close Debugger" ) );
  actionQuit->setShortcut ( Qt::CTRL + Qt::Key_Q );
  actionQuit->setMenuRole ( QAction::QuitRole );
  actionQuit->setIcon ( KIcon ( "application-exit" ) );
  connect ( actionQuit, SIGNAL ( triggered() ), this, SLOT ( close() ) );

  // Debugger Menu
  m_debuggerMenu = m_menuBar->addMenu ( i18n ( "&Debugger" ) );

  // Action Parse Document Source
  actionParse = m_debuggerMenu->addAction ( i18n ( "Parse" ) );
  actionParse->setStatusTip ( i18n ( "Parse current Document Source" ) );
  actionParse->setShortcut ( Qt::ALT + Qt::Key_C );
  actionParse->setIcon ( KIcon ( "document-edit-verify" ) );
  connect ( actionParse, SIGNAL ( triggered() ), m_sourceWidget, SLOT ( check() ) );

  // Action Prepare and Format Document Source
  actionClean = m_debuggerMenu->addAction ( i18n ( "Format" ) );
  actionClean->setStatusTip ( i18n ( "Prepare and Format Document Source" ) );
  actionClean->setShortcut ( Qt::ALT + Qt::Key_F );
  actionClean->setIcon ( KIcon ( "format-list-ordered" ) );
  connect ( actionClean, SIGNAL ( triggered() ), m_sourceWidget, SLOT ( format() ) );

  // Ansicht Menu
  m_mainViewMenu = m_menuBar->addMenu ( i18n ( "&View" ) );
  // Verwende Signal Mapper für die Signale an zoomBrowserContent
  QSignalMapper* zoomSignalMapper = new QSignalMapper ( m_mainViewMenu );
  // Zoom +
  QAction* actionZoomIn = m_mainViewMenu->addAction ( i18n ( "Zoom +" ) );
  actionZoomIn->setObjectName ( QLatin1String ( "action_view_zoom_in" ) );
  actionZoomIn->setIcon ( KIcon ( "zoom-in" ) );
  actionZoomIn->setShortcut ( QKeySequence::ZoomIn );
  connect ( actionZoomIn, SIGNAL ( triggered () ), zoomSignalMapper, SLOT ( map () ) );
  zoomSignalMapper->setMapping ( actionZoomIn, 1 );

  // Zoom -
  QAction* actionZoomOut = m_mainViewMenu->addAction ( i18n ( "Zoom -" ) );
  actionZoomOut->setObjectName ( QLatin1String ( "action_view_zoom_out" ) );
  actionZoomOut->setIcon ( KIcon ( "zoom-out" ) );
  actionZoomOut->setShortcut ( QKeySequence::ZoomOut );
  connect ( actionZoomOut, SIGNAL ( triggered () ), zoomSignalMapper, SLOT ( map () ) );
  zoomSignalMapper->setMapping ( actionZoomOut, 2 );

  // Zoom zurück Original Ansicht
  QAction* actionZoomOriginal = m_mainViewMenu->addAction ( i18n ( "Original" ) );
  actionZoomOriginal->setObjectName ( QLatin1String ( "action_view_zoom_original" ) );
  actionZoomOriginal->setIcon ( KIcon ( "zoom-original" ) );
  actionZoomOriginal->setShortcut ( Qt::CTRL + Qt::SHIFT + Qt::Key_0 );
  connect ( actionZoomOriginal, SIGNAL ( triggered () ), zoomSignalMapper, SLOT ( map() ) );
  zoomSignalMapper->setMapping ( actionZoomOriginal, 0 );

  // Zoom Signale registrieren
  connect ( zoomSignalMapper, SIGNAL ( mapped ( int ) ), m_webViewer, SLOT ( zoomFactor ( int ) ) );

  m_mainViewMenu->addSeparator();

  // Fullansciht Modus
  QAction* actionFullScreen = m_mainViewMenu->addAction ( i18n ( "Fullscreen" ) );
  actionFullScreen->setObjectName ( QLatin1String ( "action_view_full_screen" ) );
  actionFullScreen->setIcon ( KIcon ( "view-fullscreen" ) );
  actionFullScreen->setShortcut ( Qt::Key_F11 );
  connect ( actionFullScreen, SIGNAL ( triggered () ), this, SLOT ( toggleWindowFullScreen() ) );

  // Browser Menu
  m_viewMenu = m_menuBar->addMenu ( i18n ( "&Browser" ) );

  // Action WebView Reload
  actionPageReload = m_viewMenu->addAction ( i18n ( "Refresh" ) );
  actionPageReload->setShortcut ( QKeySequence::Refresh );
  actionPageReload->setIcon ( KIcon ( "view-refresh" ) );
  connect ( actionPageReload, SIGNAL ( triggered () ), m_webViewer, SLOT ( refresh () ) );

  // Action WebView Back
  actionPageBack = m_viewMenu->addAction ( i18n ( "Back" ) );
  actionPageBack->setShortcut ( QKeySequence::Back );
  actionPageBack->setIcon ( KIcon ( "go-previous-view-page" ) );
  connect ( actionPageBack, SIGNAL ( triggered () ), m_webViewer, SLOT ( back () ) );

  // Action WebView Forward
  actionPageForward = m_viewMenu->addAction ( i18n ( "Forward" ) );
  actionPageForward->setShortcut ( QKeySequence::Forward );
  actionPageForward->setIcon ( KIcon ( "go-next-view-page" ) );
  connect ( actionPageForward, SIGNAL ( triggered () ), m_webViewer, SLOT ( forward () ) );

  // New Empty WebView
  actionNewEmptyPage = m_viewMenu->addAction ( i18n ( "New Page" ) );
  actionNewEmptyPage->setStatusTip ( i18n ( "Add a new empty Tab" ) );
  actionNewEmptyPage->setShortcut ( Qt::CTRL + Qt::Key_N );
  actionNewEmptyPage->setIcon ( KIcon ( "window-new" ) );
  connect ( actionNewEmptyPage, SIGNAL ( triggered () ), m_webViewer, SLOT ( addViewerTab () ) );

  // Autoreload Menu Aciotn
  m_autoReloadMenu = new AutoReloadMenu ( m_viewMenu );
  m_viewMenu->addMenu ( m_autoReloadMenu );

  // Bookmark/History Menues
  KIcon bookmarksIcon ( "bookmarks" );
  QMenu* m_bookmarkerMenu = m_menuBar->addMenu ( i18n ( "Bookmarks" ) );
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
  QIcon bookEditIcon ( KIcon ( "bookmarks-organize" ) );
  QAction* editorAction = m_bookmarkerMenu->addAction ( bookEditIcon, i18n ( "Organize Bookmarks" ) );
  // NOTICE Qt::CTRL + Qt::Key_B ist von WebView Reserviert!
  editorAction->setShortcut ( Qt::CTRL + Qt::SHIFT + Qt::Key_B );
  connect ( editorAction, SIGNAL ( triggered() ), m_bookmarkMenu, SLOT ( openBookmarkEditor() ) );

  // Configuration Menu
  m_configurationMenu = m_menuBar->addMenu ( i18n ( "S&ettings" ) );
  // Action Open qtidyrc
  actionTidyConfig = m_configurationMenu->addAction ( i18n ( "Configure Tidyrc" ) );
  actionTidyConfig->setIcon ( KIcon ( "configure-toolbars" ) );
  connect ( actionTidyConfig, SIGNAL ( triggered() ),
            this, SLOT ( openTidyConfigApplication() ) );

  // Action open Configuration Dialog
  actionConfigDialog = m_configurationMenu->addAction ( i18n ( "Configure" ) );
  actionConfigDialog->setIcon ( KIcon ( "configure" ) );
  connect ( actionConfigDialog, SIGNAL ( triggered() ), this, SLOT ( openConfigDialog() ) );

  // Plugin Menu
  m_pluginMenu = m_menuBar->addMenu ( i18n ( "Extensions" ) );

  // Show Enable/Disable Toolbars Menu
  m_viewBarsMenu = m_menuBar->addMenu ( i18n ( "Display" ) );

  // Help and About Menu
  m_menuBar->addMenu ( customHelpMenu ( false ) );
}

/**
* Für alle Toolbalken einen Menüeintrag erstellen.
*/
void Window::createToolBars()
{
  // Actions ToolBar
  m_actionsToolBar = addToolBar ( i18n ( "Actions" ) );
  m_actionsToolBar->setObjectName ( QLatin1String ( "actionstoolbar" ) );
  m_actionsToolBar->addAction ( actionOpenUrl );
  m_actionsToolBar->addAction ( actionOpenHtml );
  m_actionsToolBar->addSeparator();
  m_actionsToolBar->addAction ( actionNewEmptyPage );
  m_actionsToolBar->addSeparator();
  m_actionsToolBar->addAction ( actionParse );
  m_actionsToolBar->addAction ( actionClean );

  // Settings ToolBar
  m_settingsToolBar = addToolBar ( i18n ( "Settings" ) );
  m_settingsToolBar->setObjectName ( QLatin1String ( "settingstoolbar" ) );
  m_settingsToolBar->addAction ( actionTidyConfig );
  m_settingsToolBar->addAction ( actionConfigDialog );

  // Address Input ToolBar
  m_addressToolBar = new AddressToolBar ( this );
  connect ( m_historyManager, SIGNAL ( updateHistoryMenu ( const QList<HistoryItem> & ) ),
            m_addressToolBar, SLOT ( updateHistoryItems ( const QList<HistoryItem> & ) ) );
  connect ( m_webViewer, SIGNAL ( urlChanged ( const QUrl & ) ),
            m_addressToolBar, SLOT ( setUrl ( const QUrl & ) ) );
  // Address ToolBar
  connect ( m_addressToolBar, SIGNAL ( urlChanged ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );
  connect ( m_addressToolBar, SIGNAL ( reloadUrl ( const QUrl & ) ),
            m_webViewer, SLOT ( setUrl ( const QUrl & ) ) );
  connect ( m_addressToolBar, SIGNAL ( sendMessage ( const QString & ) ),
            this, SLOT ( setApplicationMessage ( const QString & ) ) );

  addToolBar ( m_addressToolBar );

  // SEO Input ToolBar
  m_keywordsToolBar =  new KeywordsToolBar ( this );
  connect ( m_keywordsToolBar, SIGNAL ( changed ( const QStringList & ) ),
            m_webViewer, SLOT ( keywords ( const QStringList & ) ) );

  addToolBar ( m_keywordsToolBar );

  // Zoom Toolbar
  m_zoomBar = new ZoomBar ( this );
  connect ( m_zoomBar, SIGNAL ( zoomFactor ( int ) ),
            m_webViewer, SLOT ( zoomFactor ( int ) ) );
  addToolBar ( m_zoomBar );

  // Add ToolBar View Actions to Display Menu
  QIcon icon = QIcon::fromTheme ( "preferences-system-windows-actions" );
  QMenu* viewToolbarsMenu = m_viewBarsMenu->addMenu ( i18n ( "Toolbars" ) );
  viewToolbarsMenu->setIcon ( icon );
  viewToolbarsMenu->addAction ( m_actionsToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_settingsToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_addressToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_keywordsToolBar->toggleViewAction() );
  viewToolbarsMenu->addAction ( m_zoomBar->toggleViewAction() );
  viewToolbarsMenu->addSeparator ();

  // Add TOP|BOTTOM QDockWidget View Actions to Display Menu
  QMenu* impartationsMenu = m_viewBarsMenu->addMenu ( i18n ( "Impartations" ) );
  impartationsMenu->setIcon ( icon );
  impartationsMenu->addAction ( m_tidyMessanger->toggleViewAction() );
  impartationsMenu->addAction ( m_jsMessanger->toggleViewAction() );
  impartationsMenu->addAction ( m_appEvents->toggleViewAction() );
  impartationsMenu->addAction ( m_cssValidator->toggleViewAction() );

  // Add RIGHT|LEFT QDockWidget View Actions to Display Menu
  QMenu* inspectorsMenu = m_viewBarsMenu->addMenu ( i18n ( "Inspectors" ) );
  inspectorsMenu->setIcon ( icon );
  inspectorsMenu->addAction ( m_domInspector->toggleViewAction() );
  inspectorsMenu->addAction ( m_headerDock->toggleViewAction() );
  inspectorsMenu->addAction ( m_downloadManager->toggleViewAction() );
  inspectorsMenu->addAction ( m_colorPicker->toggleViewAction() );
  inspectorsMenu->addAction ( m_webInspector->toggleViewAction() );

  // Plugin Menu
  m_diplayPlugins = m_viewBarsMenu->addMenu ( i18n ( "Extensions" ) );
  m_diplayPlugins->setIcon ( icon );
}

/**
* Lade die zuletzt verwendeten Webseiten
* \note Wird im Konstruktor aufgerufen.
*/
void Window::loadPageHistory()
{
  bool ok = false;
  QUrl startup = m_settings->value ( QLatin1String ( "StartUpUrl" ) ).toUrl();
  if ( startup.isValid() && ! startup.isEmpty() )
  {
    openUrl ( startup );
    return; // Aussteigen
  }
  else
  {
    foreach ( QUrl url, PageHistory::history ( m_settings->historyXml() ) )
    {
      ok = openUrl ( url, ok );
    }
  }

  // wenn ok == erstelle eine about Page
  if ( ! ok )
    m_webViewer->setAboutPage ( QLatin1String ( "welcome" ) );

  if ( m_webViewer->count() == 1 )
    tabChanged ( m_webViewer->currentIndex() );
}

/**
* Speichere die zuletzt verwendeten Webseiten
* \note Wird in \ref closeEvent aufgerufen.
*/
void Window::savePageHistory()
{
  if ( m_webViewer->count() < 1 )
    return;

  PageHistory hist;
  hist.addEntries ( m_webViewer->getPageUrls() );
  hist.saveHistory ( m_settings->historyXml() );
}

/**
* Wird von @ref closeEvent aufgerufen.
* Sucht nach offenen Datenbank Verbindungen und
* versucht diese sauber zu beenden!
*/
bool Window::unregisterDatabases ()
{
  if ( QSqlDatabase::database ( QString::fromUtf8 ( "cookies" ), false ).isOpen() )
    QSqlDatabase::database ( QString::fromUtf8 ( "cookies" ), false ).close();

  return true;
}

/**
* Beim Start nach Plugins suchen und diese entsprechent
* dem Type in die einzelnen Menüs einfügen.
* @ref m_pluginMenu Im TopLevel MenuBar
* @ref m_diplayPlugins Im Anzeigen Menü
*/
bool Window::registerPlugins()
{
  QIcon icon = QIcon::fromTheme ( QLatin1String ( "preferences-plugin" ) );
  plugins.clear();
  // PopUp Widgets
  foreach ( xhtmldbg::Interface* plug, m_plugger->pluginsByType ( this, xhtmldbg::PluginInfo::PopUp ) )
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
  // Dock Widgets
  foreach ( xhtmldbg::Interface* plug, m_plugger->pluginsByType ( this, xhtmldbg::PluginInfo::Dock ) )
  {
    xhtmldbg::PluginInfo* info = plug->pluginInfo();
    if ( info )
    {
      plugins.push_back ( plug );
      addDockWidget ( Qt::RightDockWidgetArea, plug->dockwidget() );
      m_diplayPlugins->addAction ( plug->dockwidget()->toggleViewAction() );
    }
  }
  return true; // Ready
}

/**
* Plugins entladen und einige Module beim nächsten start nicht Automatisch anzeigen!
*/
bool Window::unregisterPlugins()
{
  // Vector Leeren
  plugins.clear();

  // Den Download Manager nicht in die Stats schreiben!
  bool hideDM = m_settings->value ( QLatin1String ( "HideDownloadManager" ), true ).toBool();
  if ( hideDM && m_downloadManager->toggleViewAction()->isChecked() )
  {
    m_downloadManager->toggleViewAction()->setChecked ( false );
    m_downloadManager->hide();
  }

  return true;
}

/**
* Standard closeEvent für das Speichern von
* Fenster- Geometrie und Status.
*/
void Window::closeEvent ( QCloseEvent *event )
{
  if ( isFullScreen() ) // Keine Vollansicht Speichern!
    setWindowState ( windowState() & ~Qt::WindowFullScreen );

  // Offene Tab URL's speichern
  savePageHistory();

  // Plugins entladen
  unregisterDatabases ();
  unregisterPlugins ();

  // Jetzt den Fenster Status Speichern
  m_settings->setValue ( "Window/MainWindowState", saveState() );
  m_settings->setValue ( "Window/MainWindowGeometry", saveGeometry() );

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
void Window::paintEvent ( QPaintEvent *event )
{
  /* WARNING Currently we can not Implementation a Secondary
  * resizeEvent over QWebView Classes */
  m_statusBar->displayBrowserWidth ( m_webViewer->size() );

  QMainWindow::paintEvent ( event );
}

void Window::showAboutApplication()
{
  About* aboutDialog = new About ( this );
  aboutDialog->open();
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
* Allen Widgets und Plugins mitteilen das sich etwas geändert hat.
*/
void Window::intimateWidgets ( const QUrl &url )
{
  if ( url != m_webViewer->getUrl() )
    return;

  // Quelltext vom Netzwerk Manager Platten-Speicher anfordern
  m_netManager->triggerCache ( url );

  // CSS Validator die URL übergeben
  if ( m_cssValidator->toggleViewAction()->isChecked() )
    m_cssValidator->addForValidation ( url );

  // GeoLocation die URL übergeben
  if ( ! url.isRelative() )
    m_geoLocation->setHostName ( url.host() );

  // An alle Sichtbaren Plugins die Url übergeben
  for ( int i = 0; i < plugins.size(); ++i )
  {
    if ( plugins.at ( i )->type() == xhtmldbg::PluginInfo::PopUp )
      plugins.at ( i )->setUrl ( url );
    else if ( plugins.at ( i )->dockwidget()->toggleViewAction()->isChecked() )
      plugins.at ( i )->setUrl ( url );
  }

  // Quelltext einfügen
  QString source = m_sourceCache->getCache ( url );
  if ( ! source.isEmpty() )
    m_sourceWidget->setSource ( source );
}

/**
* Wenn das Webviewer Tab gewechselt wird, nachsehen ob es sich um eine
* Webpage mit gültigen Inhalt handelt und die Inhalte an den DomInspector senden!
*/
void Window::tabChanged ( int index )
{
  if ( index != -1 )
    intimateWidgets ( m_webViewer->getUrl() );
}

/**
* Nachrichten an @ref AppEvents übermitteln.
* Wenn das Dock nicht sichtbar ist wird es
* mit @ref tabifyDockedWidgetUp nach vorne geholt!
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
* Wenn das Dock nicht sichtbar ist wird es
* mit @ref tabifyDockedWidgetUp nach vorne geholt!
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
  m_statusBar->showMessage ( i18n ( "Check StyleSheet for %1" ).arg ( url.host() ), 1024 );
  m_cssValidator->forceValidation ( url );
}

/**
* Hier wird der Quelltext an @ref SourceView übergeben.
* @li Zuerst wird @ref TidyMessanger::clearItems aufgerufen.
* @li Im zweiten Schritt wird nachgesehen ob der Quelltext nicht leer ist!
* @li Wenn nicht Leer ihn in den Quelltextspeicher speichern.
* @li Jetzt den Quelltext in @ref SourceView setzen.
* @li Danach wird der Quelltext an die geladenen Plugins übergeben.
* @li Wenn der überreichte Quelltext nicht leer ist wird in den
*  Einstellungen nachgesehen ob @em AutoFormat oder @em AutoCheck
*  aktiviert sind und entsprechend ausgeführt.
*/
bool Window::setSource ( const QUrl &url, const QString &source )
{
  QString buffer;
  m_tidyMessanger->clearItems();

  if ( source.isEmpty() && url.isValid() )
    buffer = m_sourceCache->getCache ( url );
  else
    buffer.append ( source );

  if ( buffer.isEmpty() )
    return false;

  // Quelltext einfügen
  m_sourceWidget->setSource ( buffer );
  if ( url.isValid() )
    m_sourceCache->setCache ( url, buffer );

  // An alle sichtbaren Plugins den Quelltext übergeben
  for ( int i = 0; i < plugins.size(); ++i )
  {
    if ( plugins.at ( i )->type() == xhtmldbg::PluginInfo::PopUp )
      plugins.at ( i )->setContent ( buffer );
    else if ( plugins.at ( i )->dockwidget()->toggleViewAction()->isChecked() )
      plugins.at ( i )->setContent ( buffer );
  }

  // Ist AutoCheck oder AutoFormat aktiviert?
  if ( m_settings->value ( QLatin1String ( "AutoFormat" ), false ).toBool() )
    m_sourceWidget->format();
  else if ( m_settings->value ( QLatin1String ( "AutoCheck" ), true ).toBool() )
    m_sourceWidget->check();

  buffer.clear();
  return true;
}

/**
* Standard SLOT für den Prozessaufruf vom \e qtidyrc Programm.
*/
void Window::openTidyConfigApplication()
{
  QProcess::startDetached ( QLatin1String ( "qtidyrc" ) );
}

/**
* Standard Dialog für das öffnen einer HTML Datei.
* Nach dem Dialog wird @ref openUrl aufgerufen.
*/
void Window::openFileDialog()
{
  OpenFileDialog dialog ( m_settings->getRecentDirectory(), centralWidget() );
  if ( dialog.exec() == QDialog::Accepted )
  {
    if ( ! dialog.getFileUrl().isValid() )
      return;

    m_settings->setRecentDirectory ( dialog.getDirectory() );
    openUrl ( dialog.getFileUrl() );
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
  Configuration* dialog = new Configuration ( this, m_settings );
  dialog->exec();
  delete dialog;
}

/**
* Zwischen Vollansicht und Normaler Ansicht wechseln.
*/
void Window::toggleWindowFullScreen()
{
  if ( isFullScreen() )
    setWindowState ( windowState() & ~Qt::WindowFullScreen );
  else
    setWindowState ( windowState() ^ Qt::WindowFullScreen );
}

/**
* Primitive überprüfung ob es sich um eine Url handelt.
* Wenn ja wird @ref WebViewer::setUrl aufgerufen.
*/
bool Window::openUrl ( const QUrl &url, bool addtab )
{
  if ( ! url.isValid() || url.isRelative() )
  {
    m_statusBar->showMessage ( i18n ( "Invalid Url Rejected, required scheme is \"%1\"." ).arg ( "http[s]?://" ) );
    return false;
  }

  if ( ! url.scheme().contains ( URL_SCHEME_PATTERN ) )
    return false;

  if ( addtab )
    m_webViewer->addViewerUrlTab ( url );
  else
    m_webViewer->setUrl ( url );

  return true;
}

/**
* Öffne eine Neue Seite mit @param newUrl wenn @param oldUrl noch nicht geöffnet ist.
*/
bool Window::setPageUrl ( const QUrl &oldUrl, const QUrl &newUrl )
{
  // qDebug() << Q_FUNC_INFO << oldUrl << newUrl;
  if ( ! oldUrl.isValid() || ! newUrl.isValid() )
    return false;

  if ( ! m_webViewer->setViewerTabByUrl ( oldUrl, newUrl ) )
  {
    m_webViewer->addViewerTab ();
    return openUrl ( newUrl );
  }
  else
    return true;
}

/**
* Öffne eine Seite mit @param url aber stelle zuerst eine Frage an den Nutzer.
*/
bool Window::urlRequest ( const QUrl &url )
{
  if ( ! url.isValid() )
    return false;

  QString text = i18n ( "a Url Request from outsite.\nWould you like to load this \"%1\" Url?" )
                 .arg ( url.toString() );
  QMessageBox::StandardButton st;
  st = QMessageBox::information ( this, i18n ( "Url Request from Outsite" )
                                  , text, ( QMessageBox::Ok | QMessageBox::Cancel )
                                  , QMessageBox::Cancel );

  if ( st == QMessageBox::Ok )
    return openUrl ( url );

  return false;
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

/**
* In dieser Methode werden die Downloads entgegen genommen.
* Grund hierfür ist, weil der @ref DownloadManager über @ref Page nur
* Statisch abgerufen werden kann würde dies unweigerlich beim beenden
* zu einem Speicherzugriffsfehler führen.
* Das hängt mit dem auslesen der windowStats zusammen.
*/
void Window::downloadRequest ( const QNetworkRequest &request )
{
  // Nachricht an den Messanger
  QUrl url = request.url();
  QString message = i18n ( "Download Request: %1" ).arg ( url.toString() );
  setApplicationMessage ( message );

  // Ziel Verzeichnis auswählen und bei erfolg mit für den Download übergeben!
  SetTargetDialog dialog ( url, centralWidget() );
  if ( dialog.exec() )
  {
    // wenn nicht sichtbar dann andocken
    if ( ! m_downloadManager->toggleViewAction()->isChecked() )
    {
      m_downloadManager->toggleViewAction()->setChecked ( true );
      m_downloadManager->show();
    }

    // wenn verdeckt nach vorne holen
    tabifyDockedWidgetUp ( m_downloadManager );

    // Download Starten
    m_downloadManager->download ( m_netManager->get ( request ), dialog.destination() );
  }
}

/**
* Zwischen den Zentralen Fenstern wechseln
**/
void Window::setCentralTabWidget ( const QString &index )
{
  if ( index.contains ( "source", Qt::CaseSensitive ) )
    m_centralWidget->setCurrentWidget ( m_sourceWidget );
  else
    m_centralWidget->setCurrentWidget ( m_webViewer );
}

Window::~Window()
{}
