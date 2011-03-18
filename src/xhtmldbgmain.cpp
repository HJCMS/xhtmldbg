/**
* This file is part of the xhtmldbg project
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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "xhtmldbgmain.h"
#include "dbmanager.h"
#include "xhtmldbgadaptor.h"

#include <iostream>
#include <cstdlib>

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

/* construct */
xhtmldbgmain::xhtmldbgmain ( int &argc, char **argv, bool failsafe )
    : Application ()
    , activeWindow ( 0 )
{
  setApplicationVersion ( XHTMLDBG_VERSION_STRING );
  setApplicationName ( XHTMLDBG_APPS_NAME );
  setOrganizationDomain ( XHTMLDBG_DOMAIN );
  setObjectName ( "xhtmldbgmain" );
  setGraphicsSystem ( "raster" );
  Q_UNUSED ( argv )

  // Settings
  m_settings = new Settings ( this );

  if ( failsafe )
    m_settings->setSaveMode();

  m_settings->setDataPaths();
  m_settings->setIconTheme();

  /* NOTE init Database Manager muss nach Settings::setDataPaths Initialisiert
  * werden und vor NetworkAccessManager aufgerufen sein!
  * Klasse NetworkCookie braucht diesem Pointer!
  */
  dbManager()->init();

  connect ( this, SIGNAL ( sMessageReceived ( QLocalSocket * ) ),
            this, SLOT ( sMessageReceived ( QLocalSocket * ) ) );

  if ( argc > 1 )
  {
    QStringList args = arguments();
    QString message = getArgumentUrl ( args.last() );
    sendMessage ( message.toUtf8() );
  }

  QString message = QString ( QLatin1String ( "xhtmldbg://getwinid" ) );
  if ( sendMessage ( message.toUtf8(), 500 ) )
    return;

  if ( ! startUniqueServer() )
    return;

#if defined(Q_WS_MAC)
  QApplication::setQuitOnLastWindowClosed ( false );
#else
  QApplication::setQuitOnLastWindowClosed ( true );
#endif
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

xhtmldbgmain* xhtmldbgmain::instance()
{
  return ( static_cast<xhtmldbgmain*> ( QCoreApplication::instance() ) );
}

void xhtmldbgmain::sMessageReceived ( QLocalSocket* socket )
{
  QString message;
  QTextStream stream ( socket );
  stream >> message;
  if ( message.isEmpty() )
    return;

  // TODO Regexp Überprüfen!
  if ( message.contains ( QRegExp ( "^(http[s]?|file):[\\/]{2,3}[\\w]+" ) ) )
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
  else if ( message.startsWith ( QLatin1String ( "xhtmldbg://getwinid" ) ) )
  {
    QString winid;
#ifdef Q_OS_WIN
    winid = QString ( QLatin1String ( "%1" ) ).arg ( ( qlonglong ) mainWindow()->winId() );
#endif
    setWindowFocus();
    message = QLatin1String ( "xhtmldbg://winid/" ) + winid;
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

Window* xhtmldbgmain::newMainWindow()
{
  Window *debugger = new Window ( m_settings );
  m_windows.prepend ( debugger );

  // DBUS Adaptor registrieren
  new XHtmldbgAdaptor ( debugger );

  debugger->show();
  debugger->setFocus ( Qt::ActiveWindowFocusReason );
  return debugger;
}

Window* xhtmldbgmain::mainWindow()
{
  cleanWindows();

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

/**
* Sucht nach der existens einer file:// URL Datei
*/
const QString xhtmldbgmain::getArgumentUrl ( const QString &str ) const
{
  if ( QFile::exists ( str ) )
  {
    QFileInfo info ( str );
    return info.canonicalFilePath();
  }
  return str;
}

xhtmldbgmain::~xhtmldbgmain()
{
  qDeleteAll ( m_windows );
}
