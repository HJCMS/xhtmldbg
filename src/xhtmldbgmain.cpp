/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://xhtmldbg.hjcms.de, (C) 2007-2012
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

#include "xhtmldbgmain.h"
#include "busobserver.h"
#include "dbmanager.h"
#include "historymanager.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "downloadmanager.h"

#include <iostream>
#include <cstdlib>

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QGlobalStatic>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>
#include <QtCore/QUrl>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* KDE */
#include <KDE/KCmdLineArgs>

HistoryManager* xhtmldbgmain::p_historyManager = 0;
NetworkAccessManager* xhtmldbgmain::p_networkAccessManager = 0;
DownloadManager* xhtmldbgmain::p_downloadManager = 0;
DBManager* xhtmldbgmain::p_dbManager = 0;

/* construct */
xhtmldbgmain::xhtmldbgmain ()
    : KUniqueApplication ( true, true )
    , activeWindow ( 0 )
{
  setObjectName ( "xhtmldbg" );
  setApplicationVersion ( XHTMLDBG_VERSION_STRING );
  setApplicationName ( XHTMLDBG_APPS_NAME );
  setOrganizationDomain ( XHTMLDBG_DOMAIN );
  Q_INIT_RESOURCE ( xhtmldbg );

  // Settings
  m_settings = new Settings ( this );
  m_settings->setDataPaths();
  m_settings->setIconTheme();

  // NOTE Wir verwenden nicht den KSessionManager
  disableSessionManagement();

  /* NOTE init Database Manager muss nach Settings::setDataPaths Initialisiert
  * werden und vor NetworkAccessManager aufgerufen sein!
  * WARNING Die Klasse NetworkCookie braucht diesen Pointer! */
  dbManager();
}

void xhtmldbgmain::setWindowFocus()
{
  if ( ! mainWindow() )
    return;

#ifndef Q_OS_WIN

  Window* win = mainWindow();
  if ( ! win->isVisible() )
    win->setVisible ( true );

  if ( ! win->isActiveWindow() )
  {
    win->setFocus ( Qt::ActiveWindowFocusReason );
    win->activateWindow();
    alert ( win );
  }

#endif
}

void xhtmldbgmain::cleanWindows()
{
  for ( int i = m_windows.count() - 1; i >= 0; --i )
  {
    if ( m_windows.at ( i ).isNull() )
      m_windows.removeAt ( i );
  }
}

/**
* Ein neues Fendetr erstellen!
*/
Window* xhtmldbgmain::newWindow()
{
  Window* win = new Window ( m_settings );
  win->show();
  win->setFocus ( Qt::ActiveWindowFocusReason );

  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
  if ( args->allArguments().size() >= 1 )
  {
    QUrl uri ( args->getOption ( "o" ) );
    if ( uri.isValid() )
      win->openUrl ( uri );
  }
  args->clear();

  BusObserver* m_obs = new BusObserver ( QDBusConnection::sessionBus(), win );
  m_obs->watchService ( "org.kde.kded" );

  m_windows.prepend ( win );
  return win;
}

xhtmldbgmain* xhtmldbgmain::instance()
{
  return ( static_cast<xhtmldbgmain*> ( KUniqueApplication::instance() ) );
}

Window* xhtmldbgmain::mainWindow()
{
  cleanWindows();
  if ( m_windows.isEmpty() )
  {
    activeWindow = newWindow();
  }
  else
  {
    activeWindow = qobject_cast<Window*> ( KUniqueApplication::activeWindow() );
    if ( !activeWindow )
      activeWindow = m_windows[0];
  }
  return activeWindow;
}

/**
* Starte den SQLite Datenbank-Manager
*/
DBManager* xhtmldbgmain::dbManager()
{
  if ( ! p_dbManager )
  {
    p_dbManager = DBManager::createConnection ( applicationName() );
  }
  return p_dbManager;
}

/**
* Starte den Download-Manager
*/
DownloadManager* xhtmldbgmain::downloadManager()
{
  if ( ! p_downloadManager )
    p_downloadManager = new DownloadManager ();

  return p_downloadManager;
}

/**
* Starte den URL Historien-Manager
*/
HistoryManager* xhtmldbgmain::historyManager()
{
  if ( ! p_historyManager )
  {
    p_historyManager = new HistoryManager();
    QWebHistoryInterface::setDefaultInterface ( p_historyManager );
  }
  return p_historyManager;
}

/**
* Starte das Netzwerk-Management
*/
NetworkAccessManager* xhtmldbgmain::networkAccessManager()
{
  if ( ! p_networkAccessManager )
    p_networkAccessManager = new NetworkAccessManager ();

  return p_networkAccessManager;
}

/**
* Starte den Keks-Manager
*/
NetworkCookie* xhtmldbgmain::cookieManager()
{
  return networkAccessManager()->cookieJar();
}

int xhtmldbgmain::newInstance()
{
  if ( m_windows.isEmpty() )
    mainWindow();
  else
    setWindowFocus();

  return EXIT_SUCCESS;
}

xhtmldbgmain::~xhtmldbgmain()
{
  qDeleteAll ( m_windows );
  delete p_historyManager;
  delete p_networkAccessManager;
  delete p_downloadManager;
  delete p_dbManager;
}
