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

#include "application.h"
#include "historymanager.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "downloadmanager.h"
#include "dbmanager.h"

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include <iostream>
#include <cstdlib>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QGlobalStatic>
#include <QtCore/QProcessEnvironment>

/* QtGui */
#include <QtGui/QX11Info>

/* QtDBus */
#include <QtDBus/QDBusConnection>

HistoryManager* Application::p_historyManager = 0;
NetworkAccessManager* Application::p_networkAccessManager = 0;
DownloadManager* Application::p_downloadManager = 0;
DBManager* Application::p_dbManager = 0;

Application::Application ()
    : KUniqueApplication ( true, true )
    , m_server ( 0 )
{
  setObjectName ( "application" );

  // NOTE Wir verwenden nicht den KSessionManager
  disableSessionManagement();
  Q_INIT_RESOURCE ( xhtmldbg );
}

/**
* Wenn ein neues Fenster geöffnet werden soll.
*/
void Application::newConnection()
{
  QLocalSocket* soc = m_server->nextPendingConnection();
  if ( ! soc )
    return;

  soc->waitForReadyRead();
  emit sMessageReceived ( soc );
  delete soc;
}

/**
* Setzt den lokalen Socket Namen für den Descriptor
*/
QString Application::myName () const
{
  QString name = applicationName();
  Q_ASSERT ( ! name.isEmpty() );
  QProcessEnvironment pe ( QProcessEnvironment::systemEnvironment () );
  name.append ( "_" );
  name.append ( pe.value ( QLatin1String ( "USER" ), QString::number ( getuid() ) ) );
  name.append ( "_" );
  name.append ( QString::number ( QX11Info::appScreen() ) );
  return name;
}

/**
* Anwendung Starten
*/
bool Application::startUniqueServer()
{
  bool b = false;
  if ( m_server )
    return b;

  m_server = new QLocalServer ( this );
  m_server->setMaxPendingConnections ( 2 );
  connect ( m_server, SIGNAL ( newConnection() ),
            this, SLOT ( newConnection() ) );

  if ( ! m_server->listen ( myName() ) )
  {
    if ( QAbstractSocket::AddressInUseError == m_server->serverError() )
    {
      QString fullServerName = QDir::tempPath() + QDir::separator() + myName();
      if ( QFile::exists ( fullServerName ) )
        QFile::remove ( fullServerName );

      if ( m_server->listen ( myName() ) )
        b = true;
    }
    if ( ! b )
      qWarning() << "(xhtmldbg) Unable to listen:" << m_server->errorString();
  }
  else
  {
    b = true;
  }

  if ( b )
  {
    QFile file ( m_server->fullServerName() );
    if ( ! file.setPermissions ( QFile::ReadUser | QFile::WriteUser ) )
      qWarning() << "(xhtmldbg) Permissions Denied:" << file.fileName() << file.errorString();
  }

  if ( ! b )
  {
    delete m_server;
    m_server = 0;
  }

  QString open = arguments().last();
  if ( open.contains ( QRegExp ( "^(http[s]?|file)://" ) ) )
    sendMessage ( open.toUtf8(), 500 );

  return b;
}

/**
* Sende Socket Nachricht
*/
bool Application::sendMessage ( const QByteArray &mess, int rwait )
{
  bool b = false;
  QLocalSocket soc;
  soc.connectToServer ( myName() );
  if ( ! soc.waitForConnected ( 500 ) )
    return b;

  soc.write ( mess );
  soc.flush();
  soc.waitForBytesWritten();

  b = true;
  if ( soc.error() != QLocalSocket::UnknownSocketError )
    b = false;

  if ( b )
  {
    soc.waitForReadyRead ( rwait );
    if ( soc.bytesAvailable() > 0 )
      emit sMessageReceived ( &soc );
  }

  return b;
}

/**
* Starte den SQLite Datenbank-Manager
*/
DBManager* Application::dbManager()
{
  if ( ! p_dbManager )
    p_dbManager = DBManager::createConnection ( applicationName() );

  return p_dbManager;
}

/**
* Starte den Download-Manager
*/
DownloadManager* Application::downloadManager()
{
  if ( ! p_downloadManager )
    p_downloadManager = new DownloadManager ();

  return p_downloadManager;
}

/**
* Starte den URL Historien-Manager
*/
HistoryManager* Application::historyManager()
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
NetworkAccessManager* Application::networkAccessManager()
{
  if ( ! p_networkAccessManager )
    p_networkAccessManager = new NetworkAccessManager ();

  return p_networkAccessManager;
}

/**
* Starte den Keks-Manager
*/
NetworkCookie* Application::cookieManager()
{
  return networkAccessManager()->cookieJar();
}

/**
* Beim runterfahren können jene Instanzen die Statisch
* aufgerufen wurden nur hier wieder zerstört werden.
*/
Application::~Application()
{
  delete p_historyManager;
  delete p_networkAccessManager;
  delete p_downloadManager;
  delete p_dbManager;
}
