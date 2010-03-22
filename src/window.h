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

#ifndef WINDOW_H
#define WINDOW_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

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

class NetworkAccessManager;
class HistoryManager;
class AddressToolBar;
class KeywordsToolBar;
class WebViewer;
class SourceWidget;
class TidyMessanger;
class JSMessanger;
class AppEvents;
class Bookmark;
class HistoryMenu;
class StatusBar;
class DomInspector;
class CookiesDock;
class HeaderDock;
class CSSValidator;

class Window : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSettings* m_settings;
    const QIcon qTidyIcon;
    // Network Manager
    NetworkAccessManager* m_netManager;
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

    void createMenus();
    void createToolBars();
    void tabifyDockedWidgetUp ( QDockWidget * );

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
    void setApplicationMessage ( const QString &, bool warning = false );
    bool setSource ( const QString & );
    void checkStyleSheet ( const QUrl & );
    bool openFile ( const QUrl & );
    bool openUrl ( const QUrl & );
    void visibleSourceChanged();

  public:
    Window ( QSettings * settings = 0 );
    ~Window();
};

#endif
