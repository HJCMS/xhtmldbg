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
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QMap>
#include <QtCore/QTemporaryFile>
#include <QtCore/QVariant>
#include <QtCore/QProcessEnvironment>

Validator::Validator ( QObject * parent )
    : QProcess ( parent )
    , errorLog ( QString::fromUtf8 ( "xhtmldbg_validator_XXXXXX" ), parent )
    , parameters ( 0 )
    , javaAppl ( QLatin1String ( "java" ) )
{
  setObjectName ( QLatin1String ( "validator" ) );
  setProcessChannelMode ( QProcess::SeparateChannels );
  setReadChannel ( QProcess::StandardOutput );
  if ( errorLog.open () )
    setStandardErrorFile ( QDir::tempPath() + QDir::separator() + errorLog.fileName() );

  connect ( this, SIGNAL ( readyRead () ), this, SLOT ( cleaning() ) );
}

const QString Validator::param ( const QString &key, QSettings * cfg ) const
{
  QMap<QString,QString> map;
  map["css_appl"] = "java";
  map["css_classpath"] = "/usr/share/java/css-validator";
  map["css_validator"] = "/usr/share/java/css-validator.jar";
  map["css_profile"] = "css2";
  map["css_lang"] = "en";
  map["css_medium"] = "all";
  map["css_warning"] = "0";
  return cfg->value ( key, map[key] ).toString();
}

void Validator::cleaning()
{
  url.clear();
  if ( parameters.contains ( "http" ) )
    parameters.removeLast();
}

void Validator::setEnviromentVariable ( QSettings * cfg )
{
  if ( !cfg )
    return;

  // Pfad zum Java Programm
  javaAppl = param ( "css_appl", cfg );

  QStringList clList ( "." );
  // Klassen Pfad des W3C Validierers
  QDir dir ( param ( "css_classpath", cfg ) );
  if ( dir.exists() )
  {
    foreach ( QString jar, dir.entryList ( QStringList ( "*.jar" ), QDir::Files, QDir::Name ) )
    {
      clList << dir.dirName() + dir.separator() + jar;
    }

    if ( clList.isEmpty() )
    {
      qWarning ( "(%d) empty validador parameter list", __LINE__ );
      return;
    }

    // Pfad zur W3C Validierer Datei
    QFileInfo info ( param ( "css_validator", cfg ) );
    if ( info.exists() )
    {
      parameters.clear();
      parameters << "-classpath" << clList.join ( ":" );
      parameters << "-jar" << info.fileName();
      parameters << "--output=soap12";
      parameters << QString ( "--warning=%1" ).arg ( param ( "css_warning", cfg ) );
      parameters << QString ( "--profile=%1" ).arg ( param ( "css_profile", cfg ) );
      parameters << QString ( "--lang=%1" ).arg ( param ( "css_lang", cfg ) );
      parameters << QString ( "--medium=%1" ).arg ( param ( "css_medium", cfg ) );
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
      return false;
  }
}

const QString Validator::getValidation ()
{
  return url;
}

bool Validator::setValidation ( const QString &str )
{
  if ( str.isEmpty() )
    return false;

  url = str;

  if ( parameters.size() < 5 )
    return false;

  if ( parameters.contains ( url ) )
    return false;

  return true;
}

void Validator::validate()
{
  if ( parameters.size() < 5 )
    return;

  if ( url.isEmpty() )
  {
    emit criticalError ( trUtf8 ( "to deny access for this reason missing url. Please note: The CSS Validator discarded shared url's! Remedy: reload the page." ) );
    return;
  }

  QStringList cmd;
  cmd << parameters << url;

  start ( javaAppl, cmd );
}

Validator::~Validator()
{
  if ( isRunning() )
    kill();
}
