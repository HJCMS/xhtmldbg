/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include <iostream>
#include <fstream>

#include "loglistener.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

/* KDE */
#include <KDE/KLocale>

LogListener::LogListener ( QObject * parent )
    : KDirWatch ( parent )
    , config ( parent )
{
  setObjectName ( QLatin1String ( "LogListener" ) );
  connect ( this, SIGNAL ( dirty ( const QString & ) ),
            this, SLOT ( hasChanged ( const QString & ) ) );

  setListener();
}

/**
* Setze das Basis Verzeichnis von der Letzten Datei aus der Log!
* TODO Mehrere Verschiedene Verzeichnisse Möglich?
*/
void LogListener::setListener()
{
  QString scdir = config.value ( "ApacheLoggerPlugin/scandir" ).toString();
  if ( scdir.isEmpty() )
    return;

  QFileInfo info ( scdir );
  if ( info.permission ( QFile::ReadUser ) )
    addDir ( info.absoluteFilePath(), KDirWatch::WatchFiles );
  else
    emit logChanged ( info.absoluteFilePath(), i18n ( "Permission Denied" ) );
}

/**
* Alle Verzeichnisse entfernen und KDirWatch beenden
*/
void LogListener::destroyListener()
{
  QString scdir = config.value ( "ApacheLoggerPlugin/scandir" ).toString();
  if ( scdir.isEmpty() )
    return;

  QFileInfo info ( scdir );
  if ( info.exists () )
  {
    stopDirScan ( info.absoluteFilePath() );
    removeDir ( info.absoluteFilePath() );
  }
}

/**
* Öffne die übergebene Logdatei und sende die Letzte Zeile
* mit dem Signal \ref logChanged
* TODO Halte ich nicht für besondern Performant :-/
*/
void LogListener::openLogFileJob ( const QString &logfile )
{
  QFile log ( logfile );
  if ( ! log.open ( ( QIODevice::ReadOnly | QIODevice::Text ) ) )
    return;

  QTextStream stream ( &log );
  QString str;
  QString buffer;
  do
  {
    buffer = stream.readLine();
    if ( ! buffer.isEmpty() )
      str = buffer;
  }
  while ( ! buffer.isNull() );
  log.close();

  if ( ! str.isEmpty() )
    emit logChanged ( log.fileName(), str );
}

/**
* Wenn eine Logdatei geändert wurde!
*/
void LogListener::hasChanged ( const QString &fd )
{
  if ( config.value ( "ApacheLoggerPlugin/logs" ).toStringList().contains ( fd ) )
    openLogFileJob ( fd );
}

void LogListener::restart()
{
  destroyListener();
  if ( ! config.value ( "ApacheLoggerPlugin/scandir" ).toString().isEmpty() )
    setListener();
}

/**
* Sauber Herunterfahren
*/
void LogListener::shutdown()
{
  destroyListener();
}

LogListener::~LogListener()
{}
