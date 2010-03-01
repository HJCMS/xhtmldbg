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

#include "xhtmldbg.h"

#include <cstdlib>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QGlobalStatic>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>
#include <QtCore/QUrl>

xhtmldbg::xhtmldbg ( int &argc, char **argv ) : Application ( argc, argv )
{
  setApplicationVersion ( "0.2.6" );
  setApplicationName ( "xhtmldbg" );
  setOrganizationDomain ( "hjcms.de" );
  setObjectName ( "xhtmldbg" );

  // Settings
  m_settings = new QSettings ( QSettings::NativeFormat,
                               QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

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

#if defined(Q_WS_MAC)
  QApplication::setQuitOnLastWindowClosed ( false );
#else
  QApplication::setQuitOnLastWindowClosed ( true );
#endif
}

void xhtmldbg::setWindowFocus()
{
  if ( ! mainWindow() )
    return;

#ifndef Q_OS_WIN

  mainWindow()->show();
  mainWindow()->setFocus();
  mainWindow()->activateWindow();
  alert ( mainWindow() );

#endif
}

void xhtmldbg::cleanWindows()
{
  for ( int i = m_windows.count() - 1; i >= 0; --i )
  {
    if ( m_windows.at ( i ).isNull() )
      m_windows.removeAt ( i );
  }
}

xhtmldbg* xhtmldbg::instance()
{
  return ( static_cast<xhtmldbg*> ( QCoreApplication::instance() ) );
}

void xhtmldbg::sMessageReceived ( QLocalSocket* socket )
{
  QString message;
  QTextStream stream ( socket );
  stream >> message;
  if ( message.isEmpty() )
    return;

  if ( message.startsWith ( QLatin1String ( "http://" ) ) )
  {
    QUrl url ( message.toUtf8() );
    if ( url.isValid() )
    {
      setWindowFocus();
      mainWindow()->openUrl ( url );
      socket->write ( message.toUtf8() );
      socket->waitForBytesWritten();
      return;
    }
  }
  else if ( message.startsWith ( QLatin1String ( "file://" ) ) )
  {
    QUrl url ( message.toUtf8() );
    if ( url.isValid() )
    {
      setWindowFocus();
      mainWindow()->openFile ( url );
      socket->write ( message.toUtf8() );
      socket->waitForBytesWritten();
      return;
    }
  }
  else if ( message.startsWith ( QLatin1String ( "xhtmldbg://getwinid" ) ) )
  {
    QString winid;
#ifdef Q_OS_WIN
    winid = QString ( QLatin1String ( "%1" ) ).arg ( ( qlonglong ) mainWindow()->winId() );
#endif
    setWindowFocus();
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

Window* xhtmldbg::newMainWindow()
{
  Window *debugger = new Window ( m_settings );
  m_windows.prepend ( debugger );

  debugger->show();
  return debugger;
}

Window* xhtmldbg::mainWindow()
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

const QString xhtmldbg::getArgumentUrl ( const QString &str )
{
  if ( QFile::exists ( str ) )
  {
    QFileInfo info ( str );
    return info.canonicalFilePath();
  }
  return str;
}

xhtmldbg::~xhtmldbg()
{
  qDeleteAll ( m_windows );
}
