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
#include <QtCore/QObject>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

class AddressToolBar;
class KeywordsToolBar;
class WebViewer;
class SourceWidget;
class DomViewer;
class Messanger;
class Bookmark;

class Window : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSettings* m_settings;
    // Main Menu
    QMenuBar* m_menuBar;
    // Status Bar
    QStatusBar* m_statusBar;
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
    // Configuration Menu
    QMenu* m_configurationMenu;
    QAction* actionTidyConfig;
    QAction* actionConfigDialog;
    // Central Widget
    QTabWidget* m_centralWidget;
    // Browser Widget
    WebViewer* m_webViewer;
    // Document DomViewer
    DomViewer* m_domViewer;
    // Source Viewer
    SourceWidget* m_sourceWidget;
    // Messanger
    Messanger* m_messanger;

    void createMenus();
    void createToolBars();

  protected Q_SLOTS:
    void requestsFinished ( bool );
    void openTidyConfigApplication();
    void openFileDialog();
    void openUrlDialog();

  protected:
    void closeEvent ( QCloseEvent * );

  public Q_SLOTS:
    void openFile ( const QUrl & );
    void openUrl ( const QUrl & );

  public:
    Window();
    ~Window();
};

#endif
