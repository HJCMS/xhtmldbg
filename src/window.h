/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2013
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
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCloseEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QPaintEvent>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* KDE */
#include <KDE/KMainWindow>
#include <KDE/KMenuBar>
#include <KDE/KMenu>
#include <KDE/KTabWidget>

/* XHTMLBG */
#include "settings.h"

class AlternateLinkReader;
class AppEvents;
class AutoReloader;
class AutoReloadMenu;
class Bookmark;
class ColorPicker;
class CSSValidator;
class DomInspector;
class DownloadManager;
class GeoLocation;
class HeaderDock;
class HistoryManager;
class HistoryMenu;
class JSMessanger;
class KeywordsToolBar;
class NetworkAccessManager;
class StatusBar;
class UrlToolBar;
class WebInspector;
class WebViewer;
class xhtmldbgplugger;
class ZoomBar;

namespace xhtmldbg
{
  class Interface;
  class Plugger;
}

class Window : public KMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "J??rgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    Settings* m_settings;
    const QIcon xhtmldbgIcon;
    // Network Manager
    NetworkAccessManager* m_netManager;
    // Download Manager
    DownloadManager* m_downloadManager;
    // Address History Manager
    HistoryManager* m_historyManager;
    // Main Menu
    KMenuBar* m_menuBar;
    // Status Bar
    StatusBar* m_statusBar;
    // Default ToolBar
    QToolBar* m_actionsToolBar;
    // Settings ToolBar
    QToolBar* m_settingsToolBar;
    // Address ToolBar
    UrlToolBar* m_urlToolBar;
    // SEO ToolBar
    KeywordsToolBar* m_keywordsToolBar;
    // Webviewer Zoom Bar
    ZoomBar* m_zoomBar;
    // Application Menu
    QMenu* m_applicationMenu;
    QAction* actionOpenUrl;
    QAction* actionOpenHtml;
    QAction* actionQuit;
    // Window View Menu
    QMenu* m_mainViewMenu;
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
    QAction* actionConfigDialog;
    // Enable/Disable ToolBars etc.
    QMenu* m_viewBarsMenu;
    // Browser Widget
    WebViewer* m_webViewer;
    // HTML Document Dom Tree Viewer
    DomInspector* m_domInspector;
    // Formular Manager
    // FormManager* m_formManager;
    // WebInspector
    WebInspector* m_webInspector;
    // Read link:rev Alternate Elements
    AlternateLinkReader* m_alternateLinkReader;
    // Find Country by Hostname
    GeoLocation* m_geoLocation;
    // JavaScript Messages
    JSMessanger* m_jsMessanger;
    // Display Infromation from XHTMLDBG
    AppEvents* m_appEvents;
    // Received (HTTP/1.0) Headers
    HeaderDock* m_headerDock;
    // CSS Validator
    CSSValidator* m_cssValidator;
    // ColorPicker
    ColorPicker* m_colorPicker;
    // Plugin Interface
    xhtmldbg::Plugger* m_plugger;
    // Plugin Menu
    QMenu* m_pluginMenu;
    // Toolbars Subsection Display Plugins
    QMenu* m_diplayPlugins;
    // Plugins
    QVector<xhtmldbg::Interface*> plugins;

    void createMenus();
    void createToolBars();

    void tabifyDockedWidgetUp ( QDockWidget * );

    // Url and Tab Management
    void loadPageHistory();
    void savePageHistory();

    // Plugin Management
    bool registerPlugins();
    bool unregisterDatabases();
    bool unregisterPlugins();

  private Q_SLOTS:
    void intimateWidgets ( const QUrl &url );
    void tabChanged ( int );
    void openFileDialog();
    void openUrlDialog();
    void openConfigDialog();
    void toggleWindowFullScreen();

  protected:
    void closeEvent ( QCloseEvent * );
    void paintEvent ( QPaintEvent * );
    void showAboutApplication();

  public Q_SLOTS:
    void setJavaScriptMessage ( const QString & );
    // supported by DBus Adaptor and Service Interface
    void setApplicationMessage ( const QString &, bool warning = false );
    // supported by DBus Adaptor and Service Interface
    void checkStyleSheet ( const QUrl & );
    // supported by DBus Adaptor and Service Interface
    bool openUrl ( const QUrl &, bool addtab = false );
    // supported by DBus Adaptor and Service Interface
    bool setPageUrl ( const QUrl &, const QUrl & );
    // supported by DBus Adaptor and Service Interface
    bool urlRequest ( const QUrl & );

    // used by QWebPage ...
    void downloadRequest ( const QNetworkRequest & );

  public:
    Window ( Settings * settings = 0 );
    ~Window();
};

#endif
