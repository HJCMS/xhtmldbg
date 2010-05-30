/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef WINDOW_H
#define WINDOW_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCloseEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QIcon>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QPaintEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

/* QtDBus */
#include <QtDBus/QDBusConnection>

class NetworkAccessManager;
class DownloadManager;
class HistoryManager;
class AddressToolBar;
class KeywordsToolBar;
class ZoomBar;
class WebViewer;
class SourceWidget;
class TidyMessanger;
class JSMessanger;
class AppEvents;
class Bookmark;
class HistoryMenu;
class AutoReloadMenu;
class StatusBar;
class AutoReloader;
class DomInspector;
class CookiesDock;
class HeaderDock;
class CSSValidator;
class XHtmldbgAdaptor;
class xhtmldbgplugger;

namespace xhtmldbg
{
  class Interface;
}

class Window : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QSettings* m_settings;
    const QIcon qTidyIcon;
    // URL Scheme Validate
    const QRegExp schemePattern;
    // Network Manager
    NetworkAccessManager* m_netManager;
    // Download Manager
    DownloadManager* m_downloadManager;
    // Address History Manager
    HistoryManager* m_historyManager;
    // Main Menu
    QMenuBar* m_menuBar;
    // Status Bar
    StatusBar* m_statusBar;
    // Default ToolBar
    QToolBar* m_actionsToolBar;
    // Settings ToolBar
    QToolBar* m_settingsToolBar;
    // Address ToolBar
    AddressToolBar* m_addressToolBar;
    // SEO ToolBar
    KeywordsToolBar* m_keywordsToolBar;
    // Webviewer Zoom Bar
    ZoomBar* m_zoomBar;
    // Application Menu
    QMenu* m_applicationMenu;
    QAction* actionOpenUrl;
    QAction* actionOpenHtml;
    QAction* actionQuit;
    // Debugger Menu
    QMenu* m_debuggerMenu;
    QAction* actionParse;
    QAction* actionClean;
    // WebViewer Menu
    QMenu* m_viewMenu;
    QAction* actionPageReload;
    QAction* actionPageBack;
    QAction* actionPageForward;
    QAction* actionNewEmptyPage;
    // Bookmark Menu
    Bookmark* m_bookmarkMenu;
    // History Menu
    HistoryMenu* m_historyMenu;
    // Auto Reload WebPage Menu
    AutoReloadMenu* m_autoReloadMenu;
    // Setting Reload Interval
    AutoReloader* m_autoReloader;
    // Configuration Menu
    QMenu* m_configurationMenu;
    QAction* actionTidyConfig;
    QAction* actionConfigDialog;
    // Enable/Disable ToolBars etc.
    QMenu* m_viewBarsMenu;
    // Central Widget
    QTabWidget* m_centralWidget;
    // Browser Widget
    WebViewer* m_webViewer;
    // HTML Document Dom Tree Viewer
    DomInspector* m_domInspector;
    // Source Viewer
    SourceWidget* m_sourceWidget;
    // Tidy Messages
    TidyMessanger* m_tidyMessanger;
    // JavaScript Messages
    JSMessanger* m_jsMessanger;
    // Display Infromation from XHTMLDBG
    AppEvents* m_appEvents;
    // Cookie
    CookiesDock* m_cookiesDock;
    // Received Headers
    HeaderDock* m_headerDock;
    // CSS Validator
    CSSValidator* m_cssValidator;
    // Dbus Adaptor
    XHtmldbgAdaptor* m_xhtmldbgAdaptor;
    // Plugin Interface
    xhtmldbgplugger* plugger;
    // Plugin Menu
    QMenu* m_pluginMenu;
    // Toolbars Subsection Display Plugins
    QMenu* m_diplayPlugins;
    // Plugins
    QVector<xhtmldbg::Interface*> plugins;

    void createMenus();
    void createToolBars();
    void tabifyDockedWidgetUp ( QDockWidget * );
    void registerPlugins();

  private Q_SLOTS:
    void requestsFinished ( bool );
    void openTidyConfigApplication();
    void openFileDialog();
    void openUrlDialog();
    void openConfigDialog();

  protected:
    void closeEvent ( QCloseEvent * );
    void paintEvent ( QPaintEvent * );

  public Q_SLOTS:
    void setJavaScriptMessage ( const QString & );
    // supported by DBus Adaptor
    void setApplicationMessage ( const QString &, bool warning = false );
    // supported by DBus Adaptor
    bool setSource ( const QString & );
    void checkStyleSheet ( const QUrl & );
    // supported by DBus Adaptor
    bool openFile ( const QUrl & );
    // supported by DBus Adaptor
    bool openUrl ( const QUrl & );
    // supported by DBus Adaptor
    bool setPageUrl ( const QUrl &, const QUrl & );
    void visibleSourceChanged();
    // used by QWebPage ...
    void downloadRequest ( const QNetworkRequest & );

  public:
    Window ( QSettings * settings = 0 );
    ~Window();
};

#endif
