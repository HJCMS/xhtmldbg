/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "validator.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

Validator::Validator ( QObject * parent )
    : QProcess ( parent )
    , p_env ( QProcessEnvironment::systemEnvironment() )
    , parameters ( 0 )
{
  setObjectName ( QLatin1String ( "validator" ) );
  setProcessChannelMode ( QProcess::SeparateChannels );
  setReadChannel ( QProcess::StandardOutput );
  setStandardErrorFile ( "/tmp/xhtmldbg.log" );
}

void Validator::setEnviromentVariable ( QSettings * cfg )
{
  if ( !cfg )
    return;

  QStringList clList ( "." );
  QString libdir = cfg->value ( "classpath", QLatin1String ( "/usr/share/java/css-validator" ) ).toString();
  QDir dir ( libdir );
  if ( dir.exists() )
  {
    foreach ( QString jar, dir.entryList ( QStringList ( "*.jar" ), QDir::Files, QDir::Name ) )
    {
      clList << dir.dirName() + dir.separator() + jar;
    }

    if ( clList.isEmpty() )
      return;

//     p_env.insert ( "CLASSPATH", clList.join ( ":" ) );
//     setProcessEnvironment ( p_env );

    QString w3c = cfg->value ( "validator", QLatin1String ( "/usr/share/java/css-validator.jar" ) ).toString();
    QFileInfo info ( w3c );
    if ( info.exists() )
    {
      parameters.clear();
      parameters << "-classpath" << clList.join ( ":" );
      parameters << "-jar" << info.fileName() << "--output=soap12" << "--warning=2";
      parameters << "--profile=css21" << "--lang=de";
      setWorkingDirectory ( info.absolutePath() );
    }
  }
}

bool Validator::isRunning()
{
  switch ( state() )
  {
    case QProcess::NotRunning:
    {
      emit down ();
      return false;
    }

    case QProcess::Starting:
    {
      emit running ();
      return true;
    }

    case QProcess::Running:
    {
      emit running ();
      return true;
    }

    default:
    {
      emit down ();
      return false;
    }
  }
}

bool Validator::validate ( const QString &url )
{
  if ( url.isEmpty() )
    return false;

  if ( parameters.size() < 5 )
    return false;

  parameters << url;
  start ( "java", parameters );
  // qDebug() << Q_FUNC_INFO << parameters;
  return true;
}

Validator::~Validator()
{}
