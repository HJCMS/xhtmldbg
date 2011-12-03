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
#include <QtCore/QGenericReturnArgument>
#include <QtCore/QGlobalStatic>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QMetaObject>
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
xhtmldbgmain::xhtmldbgmain () : KUniqueApplication ( true, true )
{
  setObjectName ( "xhtmldbg" );
  setApplicationVersion ( XHTMLDBG_VERSION_STRING );
  setApplicationName ( XHTMLDBG_APPS_NAME );
  setOrganizationDomain ( XHTMLDBG_DOMAIN );
  setStartupId ( QByteArray( "xhtmldbg" ) );
  Q_INIT_RESOURCE ( xhtmldbg );

  // Settings
  m_settings = new Settings ( this );
  m_settings->setDataPaths();
  m_settings->setIconTheme();

  /* NOTE init Database Manager muss nach Settings::setDataPaths Initialisiert
  * werden und vor NetworkAccessManager aufgerufen sein!
  * WARNING Die Klasse NetworkCookie braucht diesen Pointer! */
  dbManager ( this );
}

void xhtmldbgmain::setWindowFocus()
{
  if ( ! mainWindow() )
    return;

  Window* win = mainWindow();
  if ( ! win->isVisible() )
    win->setVisible ( true );

  if ( ! win->isActiveWindow() )
  {
    win->setFocus ( Qt::ActiveWindowFocusReason );
    win->activateWindow();
    alert ( win );
  }
}

/**
* Ein neues Fenster erstellen!
*/
Window* xhtmldbgmain::newWindow()
{
  window = new Window ( m_settings );
  window->show();
  window->setFocus ( Qt::ActiveWindowFocusReason );

  QDBusConnection bus = QDBusConnection::sessionBus();
  bus.registerObject ( "/xhtmldbg" , this, QDBusConnection::ExportScriptableContents );

  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
  if ( args->allArguments().size() >= 1 )
  {
    QUrl uri ( args->getOption ( "o" ) );
    if ( uri.isValid() )
      window->openUrl ( uri );
  }
  args->clear();

  return window;
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

xhtmldbgmain* xhtmldbgmain::instance()
{
#ifdef MAINTAINER_REPOSITORY
  qDebug() << Q_FUNC_INFO << __LINE__;
#endif
  return ( qobject_cast<xhtmldbgmain*> ( QCoreApplication::instance() ) );
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
* Nachrichten übermittlung
*/
void xhtmldbgmain::message ( const QString &mess )
{
  QMetaObject::invokeMethod ( mainWindow(), "setApplicationMessage",
                              Q_ARG ( QString, mess ), Q_ARG ( bool, false ) );
}

/**
* Die übergebene Zeichenketten URL wird mit QUrl::StrictMode Importiert.
* Diese Methode öffnet sendet einen Dialog an die IDE und fragt zuerst!
*/
bool xhtmldbgmain::open ( const QString &url )
{
  bool b = false;
  QUrl u ( url, QUrl::StrictMode );
  if ( u.isValid() && u.scheme().contains ( "http" ) )
  {
    QMetaObject::invokeMethod ( mainWindow(), "urlRequest", Q_RETURN_ARG ( bool, b ), Q_ARG ( QUrl, u ) );
    return b;
  }
  else if ( u.isValid() && u.scheme().contains ( "file" ) )
  {
    return setFile ( u.toString ( QUrl::RemoveScheme ) );
  }
  else if ( u.isValid() && u.scheme().contains ( "ftp" ) )
  {
    message ( i18n ( "(XHTMLDBG) Reject \"%1\" FTP request!" ).arg ( u.toString() ) );
    return false;
  }
  return false;
}

/**
* Die übergebene Zeichenketten URL wird mit QUrl::StrictMode
* Importiert, dann weiter an die IDE (Wenn der import nicht
* fehlgeschlagen ist.) geleitet.
* Diese Methode öffnet eine NeueSeite wenn die alte URL nicht Vorhanden ist!
*/
bool xhtmldbgmain::setUrl ( const QString &oldUrl, const QString &newUrl )
{
  bool b = false;
  QUrl info ( newUrl, QUrl::StrictMode );
  if ( info.isValid() && info.scheme().contains ( "http" ) )
  {
    QUrl old ( oldUrl );
    QMetaObject::invokeMethod ( mainWindow(), "setPageUrl", Q_RETURN_ARG ( bool, b ),
                                Q_ARG ( QUrl, old ), Q_ARG ( QUrl, info ) );
    return b;
  }
  else if ( info.isValid() && info.scheme().contains ( "file" ) )
  {
    return setFile ( info.toString ( QUrl::RemoveScheme ) );
  }
  return false;
}

/**
* Ein Datei öffnen in dem auf die existenz geprüft wird.
* Das file:// Scheme wird immer eingefügt!
*/
bool xhtmldbgmain::setFile ( const QString &url )
{
  bool b = false;
  QString buffer ( url );
  QFileInfo file ( buffer.remove ( "file://" ) );
  if ( file.exists() && ! file.isExecutable() )
  {
    QUrl u ( file.absoluteFilePath() );
    bool addtab = true;
    u.setScheme ( "file" );
    QMetaObject::invokeMethod ( mainWindow(), "openUrl", Q_RETURN_ARG ( bool, b )
                                , Q_ARG ( QUrl, u ), Q_ARG ( bool, addtab ) );
    return b;
  }
  return false;
}

/**
* XHMLT/HTML Quelltext an die Ansicht übergeben.
*/
bool xhtmldbgmain::setSource ( const QString &url, const QString &xhtml )
{
  bool b = false;
  QUrl sendUrl ( url, QUrl::StrictMode );
  QMetaObject::invokeMethod ( mainWindow(), "setSource", Q_RETURN_ARG ( bool, b ),
                              Q_ARG ( QUrl, sendUrl ), Q_ARG ( QString, xhtml ) );
  return b;
}

bool xhtmldbgmain::checkStyleSheet ( const QString &url )
{
  QUrl sendUrl ( url, QUrl::StrictMode );
  if ( sendUrl.isValid() )
  {
    QMetaObject::invokeMethod ( mainWindow(), "checkStyleSheet", Q_ARG ( QUrl, sendUrl ) );
    return true;
  }
  return false;
}

xhtmldbgmain::~xhtmldbgmain()
{
  if ( window )
  {
    window->close();
    window->deleteLater();
  }
  delete p_historyManager;
  delete p_networkAccessManager;
  delete p_downloadManager;
  delete p_dbManager;
}
