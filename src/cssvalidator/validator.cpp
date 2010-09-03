/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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
#include <QtCore/QVariant>
#include <QtCore/QProcessEnvironment>

static inline const QString tempFilePath()
{
  QString p ( QDir::tempPath() );
  p.append ( QDir::separator() );
  p.append ( QString::fromUtf8 ( "XHTMLDBG_Validator_XXXXXX.log" ) );
  return p;
}

Validator::Validator ( QObject * parent )
    : QProcess ( parent )
    , tempFile ( tempFilePath(), parent )
    , errorLog ( tempFilePath() )
    , parameters ( 0 )
    , javaAppl ( QLatin1String ( "java" ) )
{
  setObjectName ( QLatin1String ( "validator" ) );
  setProcessChannelMode ( QProcess::SeparateChannels );
  setReadChannel ( QProcess::StandardOutput );

  /* Generiere einen Temporären Dateinamen und setzte
  * mit dieser den Protokoll Dateipfad für die Fehler Meldungen */
  if ( tempFile.open () )
  {
    errorLog = tempFile.fileName(); // Wenn OK dann überschreiben.
    setStandardErrorFile ( errorLog, QIODevice::Append );
  }
  else
    setStandardErrorFile ( errorLog );

  connect ( this, SIGNAL ( readyRead () ), this, SLOT ( cleaning() ) );
}

/**
* Lese Konfigurations Parameter und falle bei bedarf
* in die Grundeinstellungen zurück.
*/
const QString Validator::param ( const QString &key, Settings * cfg ) const
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

/**
* Wird aufgerufen wenn ein Prozess beendet wurde.
*/
void Validator::cleaning()
{
  url.clear();
  if ( parameters.contains ( "http" ) )
    parameters.removeLast();
}

/**
* Umgebungs Variablen aus den Einstellungen lesen
* und komplett neu setzen. Wird unter anderem auch
* dem schließen des Konfigurations Dialoges aufgerufen.
*/
void Validator::setEnviromentVariable ( Settings * cfg )
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

/**
* Methode für den aktuellen Prozess Status ab zu fragen.
*/
bool Validator::isRunning()
{
  switch ( state() )
  {
    case QProcess::NotRunning:
      return false;

    case QProcess::Starting:
      return true;

    case QProcess::Running:
      return true;

    default:
      return false;
  }
}

/**
* Welche Adresse wird zu Zeit verwendet.
*/
const QString Validator::getValidation ()
{
  return url;
}

/**
* Setze die Adresse, welche geprüft werden soll.
*/
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

/**
* Starte einen neuen Prozess.
*/
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

/**
* Stelle den Pfad zu Fehlerprotokoll Datei
* für andere Klasse zu verfügung.
*/
const QString Validator::errorsLogFile()
{
  return errorLog;
}

/**
* Sicher gehen das nicht noch ein Prozess
* am laufen ist. Wenn doch knall hart schließen.
*/
Validator::~Validator()
{
  if ( isRunning() )
    kill();
}
