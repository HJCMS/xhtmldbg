/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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
#include <QtCore/QTextStream>

/* KDE */
#include <KDE/KLocale>

LogListener::LogListener ( QObject * parent )
    : KDirWatch ( parent )
{
  setObjectName ( QLatin1String ( "LogListener" ) );
  logFilePath.clear();
  connect ( this, SIGNAL ( dirty ( const QString & ) ),
            this, SLOT ( hasChanged ( const QString & ) ) );
}

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
    if ( !buffer.isEmpty() )
      str = buffer;
  }
  while ( ! buffer.isNull() );
  log.close();

  if ( ! str.isEmpty() )
    emit logChanged ( str );
}

bool LogListener::setLogfile ( const QString &logfile )
{
  if ( ! logFilePath.isEmpty() )
    destroyLogfile();

  logFilePath = logfile;
  QFileInfo info ( logFilePath );
  if ( info.permission ( QFile::ReadUser ) )
  {
    addDir ( info.path(), KDirWatch::WatchFiles );
    addFile ( info.fileName() );
    setDirty ( info.absoluteFilePath() );
    return true;
  }
  else
    logFilePath.clear();

  emit logChanged ( i18n ( "Permission Denied" ) );
  return false;
}

void LogListener::destroyLogfile()
{
  QFileInfo info ( logFilePath );
  if ( info.exists () )
  {
    removeFile ( info.fileName() );
    stopDirScan ( info.path() );
    removeDir ( info.path() );
  }
}

void LogListener::hasChanged ( const QString &fd )
{
  if ( fd == logFilePath )
    openLogFileJob ( fd );
}

void LogListener::toggle ()
{
  if ( ! logFilePath.isEmpty() )
    restartDirScan ( logFilePath );
}

void LogListener::shutdown()
{
  destroyLogfile();
}

LogListener::~LogListener()
{}
