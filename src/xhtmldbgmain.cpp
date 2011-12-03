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

#ifdef MAINTAINER_REPOSITORY
static bool set_unique = false;
#else
static bool set_unique = true;
#endif

/* construct */
xhtmldbgmain::xhtmldbgmain ()
    : KUniqueApplication ( true, set_unique )
    , window ( 0 )
{
  setObjectName ( "xhtmldbg" );
  setApplicationVersion ( XHTMLDBG_VERSION_STRING );
  setApplicationName ( XHTMLDBG_APPS_NAME );
  setOrganizationDomain ( XHTMLDBG_DOMAIN );
  // Settings
  m_settings = new Settings ( this );
  m_settings->setDataPaths();
  m_settings->setIconTheme();

  /* NOTE init Database Manager muss nach Settings::setDataPaths Initialisiert
  * werden und vor NetworkAccessManager aufgerufen sein!
  * WARNING Die Klasse NetworkCookie braucht diesen Pointer! */
  dbManager ( this );

  // NOTE Wir verwenden nicht den KSessionManager
  disableSessionManagement();
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

/**
* Ein neues Fenster erstellen!
*/
Window* xhtmldbgmain::newWindow()
{
  window = new Window ( m_settings );
  window->show();
  window->setFocus ( Qt::ActiveWindowFocusReason );

  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
  if ( args->allArguments().size() >= 1 )
  {
    QUrl uri ( args->getOption ( "o" ) );
    if ( uri.isValid() )
      window->openUrl ( uri );
  }
  args->clear();

  BusObserver* m_obs = new BusObserver ( QDBusConnection::sessionBus(), window );
  m_obs->watchService ( "org.kde.kded" );

  return window;
}

xhtmldbgmain* xhtmldbgmain::instance()
{
  return ( static_cast<xhtmldbgmain*> ( QCoreApplication::instance() ) );
}

/**
* Suche nach Hauptfenster, wenn nicht vorhanden dann erstellen!
*/
Window* xhtmldbgmain::mainWindow()
{
  if ( ! window )
    window = newWindow();

  return window;
}

/**
* Starte den SQLite Datenbank-Manager
*/
DBManager* xhtmldbgmain::dbManager ( QObject * parent )
{
  if ( ! p_dbManager )
    p_dbManager = DBManager::createConnection ( XHTMLDBG_APPS_NAME, parent );

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

/**
* Virtuelle Methode zum erstellen einer neuen Instanze.
* @see http://api.kde.org/4.x-api/kdelibs-apidocs/kdeui/html/classKUniqueApplication.html
*/
int xhtmldbgmain::newInstance()
{
  if ( ! window )
    mainWindow();
  else
    setWindowFocus();

  return EXIT_SUCCESS;
}

xhtmldbgmain::~xhtmldbgmain()
{
  delete p_historyManager;
  delete p_networkAccessManager;
  delete p_downloadManager;
  delete p_dbManager;
}
