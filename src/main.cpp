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

#include "main.h"

#include <cstdlib>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, true )
    , m_server ( 0 )
{
  setObjectName ( "Application" );
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

Application::~Application()
{}

Main::Main ( int &argc, char **argv ) : Application ( argc, argv )
{
  setApplicationVersion ( "0.2.6" );
  setApplicationName ( "xhtmldbg" );
  setOrganizationDomain ( "hjcms.de" );
  setObjectName ( "DebuggerApplication" );

  connect ( this, SIGNAL ( sMessageReceived ( QLocalSocket * ) ),
            this, SLOT ( sMessageReceived ( QLocalSocket * ) ) );

  QStringList args = QCoreApplication::arguments();
  if ( args.count() > 1 )
  {
    QString message = getArgumentUrl ( args.last() );
    sendMessage ( message.toUtf8() );
  }

  QString message = QString ( QLatin1String ( "xhtmldbg://getwinid" ) );
  if ( sendMessage ( message.toUtf8(), 500 ) )
    return;

  if ( !startUniqueServer() )
    return;

  QApplication::setQuitOnLastWindowClosed ( true );
}

void Main::cleanWindows()
{
  for ( int i = m_windows.count() - 1; i >= 0; --i )
  {
    if ( m_windows.at ( i ).isNull() )
      m_windows.removeAt ( i );
  }
}

Main* Main::instance()
{
  return ( static_cast<Main*> ( QCoreApplication::instance() ) );
}

void Main::sMessageReceived ( QLocalSocket* socket )
{
  QString message;
  QTextStream stream ( socket );
  stream >> message;
  if ( message.isEmpty() )
    return;

  if ( message.startsWith ( QLatin1String ( "xhtmldbg://getwinid" ) ) )
  {
    mainWindow()->show();
    mainWindow()->setFocus();
    mainWindow()->activateWindow();
    alert ( mainWindow() );
    QString winid;
    QString message = QLatin1String ( "xhtmldbg://winid/" ) + winid;
    socket->write ( message.toUtf8() );
    socket->waitForBytesWritten();
    return;
  }

  if ( message.startsWith ( QLatin1String ( "xhtmldbg://winid" ) ) )
  {
    QString winid = message.mid ( 21 );
    return;
  }
  return;
}

Window* Main::newMainWindow()
{
  Window *debugger = new Window();
  m_windows.prepend ( debugger );

  debugger->show();
  return debugger;
}

Window* Main::mainWindow()
{
  cleanWindows();

  Window *activeWindow = 0;

  if ( m_windows.isEmpty() )
  {
    activeWindow = newMainWindow();
  }
  else
  {
    activeWindow = qobject_cast<Window*> ( QApplication::activeWindow() );
    if ( !activeWindow )
      activeWindow = m_windows[0];
  }

  return activeWindow;
}

const QString Main::getArgumentUrl ( const QString &str )
{
  if ( QFile::exists ( str ) )
  {
    QFileInfo info ( str );
    return info.canonicalFilePath();
  }
  return str;
}

Main::~Main()
{
  qDeleteAll ( m_windows );
}

int main ( int argc, char *argv[] )
{
  Main app ( argc, argv );
  if ( ! app.isRunning() )
    return EXIT_SUCCESS;

  QStringList trPaths ( QCoreApplication::applicationDirPath() );
  trPaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator ( 0 );
  foreach ( QString tf, trPaths )
  {
    if ( translator.load ( QString ( "xhtmldbg_%1.qm" ).arg ( QLocale().name() ), tf, "_" ) )
      break;
  }
  app.installTranslator ( &translator );

  app.newMainWindow();
  return app.exec();
  return EXIT_SUCCESS;
}
