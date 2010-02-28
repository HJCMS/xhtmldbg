/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://xhtmldbg.hjcms.de, (C) 2007-2010
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

#include <cstdlib>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QGlobalStatic>

/* QtGui */
#include <QtGui/QIcon>

HistoryManager* Application::p_historyManager = 0;
NetworkAccessManager* Application::p_networkAccessManager = 0;

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, true )
    , m_server ( 0 )
{
  setObjectName ( "Application" );
  Q_INIT_RESOURCE ( xhtmldbg );
  // Setting Default Application Properties
  setGraphicsSystem ( QLatin1String ( "native" ) );
  QIcon::setThemeName ( "oxygen" );
}

void Application::newConnection()
{
  QLocalSocket* soc = m_server->nextPendingConnection();
  if ( ! soc )
    return;

  soc->waitForReadyRead();
  emit sMessageReceived ( soc );
  delete soc;
}

QString Application::myName() const
{
  QString name = applicationName();
  Q_ASSERT ( ! name.isEmpty() );
  name += QString ( QLatin1String ( "_%1_%2" ) ).arg ( getuid() ).arg ( getgid() );
  return name;
}

bool Application::startUniqueServer()
{
  bool b = false;
  if ( m_server )
    return b;

  m_server = new QLocalServer ( this );
  connect ( m_server, SIGNAL ( newConnection() ),
            this, SLOT ( newConnection() ) );

  if ( ! m_server->listen ( myName() ) )
  {
    if ( QAbstractSocket::AddressInUseError == m_server->serverError() )
    {
      QString fullServerName = QDir::tempPath() + QLatin1String ( "/" ) + myName();
      if ( QFile::exists ( fullServerName ) )
        QFile::remove ( fullServerName );

      if ( m_server->listen ( myName() ) )
        b = true;
    }
    if ( ! b )
      qWarning() << "XHTML_DBG_SERVER: Unable to listen:" << m_server->errorString();
  }
  else
  {
    b = true;
  }

  if ( b )
  {
    QFile file ( m_server->fullServerName() );
    if ( ! file.setPermissions ( QFile::ReadUser | QFile::WriteUser ) )
      qWarning() << "XHTML_DBG_SERVER: Permissions Denied:" << file.fileName() << file.errorString();

  }

  if ( ! b )
  {
    delete m_server;
    m_server = 0;
  }

  QString open = arguments().last();
  if ( open.contains ( QRegExp ( "^(http|file)://" ) ) )
    sendMessage ( open.toUtf8(), 500 );

  return b;
}

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

bool Application::isRunning() const
{
  return ( 0 != m_server );
}

HistoryManager* Application::historyManager()
{
  if ( !p_historyManager )
  {
    p_historyManager = new HistoryManager();
    QWebHistoryInterface::setDefaultInterface ( p_historyManager );
  }
  return p_historyManager;
}

NetworkAccessManager* Application::networkAccessManager()
{
  if ( !p_networkAccessManager )
  {
    p_networkAccessManager = new NetworkAccessManager();
    // p_networkAccessManager->setCookieJar(new CookieJar);
  }
  return p_networkAccessManager;
}

Application::~Application()
{
  delete p_historyManager;
  delete p_networkAccessManager;
}
