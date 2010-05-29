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

#include "downloader.h"

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>

Downloader::Downloader ( QNetworkReply * reply, QWidget * parent )
    : QWidget ( parent )
    , m_reply ( reply )
    , defaultLocation ( QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) )
    , m_bytesLoaded ( 0 )
    , inProgress ( 0 )
    , m_uploadTime ( 0, 0, 0 )
    , m_progressTime ( 0, 0, 0 )
    , m_metaType ( "application/octet-stream" )
{
  setObjectName ( QLatin1String ( "downloader" ) );
}

/**
* Initialisiere die Signale für QNetworkRelpy und setze auf parent
*/
void Downloader::openDownload()
{
  if ( ! m_reply )
    return;

  m_reply->setParent ( this );
  connect ( m_reply, SIGNAL ( readyRead() ), this, SLOT ( downloadReadyRead() ) );
  connect ( m_reply, SIGNAL ( metaDataChanged() ),
            this, SLOT ( getChangedMetaData() ) );
  connect ( m_reply, SIGNAL ( downloadProgress ( qint64, qint64 ) ),
            this, SLOT ( downloadProgress ( qint64, qint64 ) ) );
  connect ( m_reply, SIGNAL ( finished() ), this, SLOT ( finished() ) );

  // jetzt den Timer starten
  m_progressTime.start();
}

/**
* Einen aktiven Download abbrechen
*/
void Downloader::abort()
{
  if ( m_reply->isRunning() )
  {
    m_output.close();
    m_reply->close();
  }
}

/**
* Einen Vorhandenen Download neu starten!
*/
void Downloader::restart()
{
  if ( ! m_reply->isRunning() && inProgress <= 100 )
    openDownload();
}

/**
* Einkommende Daten in @ref destinationFilePath schreiben.
* Wenn @ref QFile::m_output noch nicht offen ist dann öffnen
* und im Notfall noch einige Fehlermeldungen auspucken.
*/
void Downloader::downloadReadyRead()
{
  if ( destinationFilePath.isEmpty() )
    return;

  if ( ! m_output.isOpen () )
  {
    if ( ! m_output.open ( QIODevice::WriteOnly ) )
      qWarning ( "Error opening output file: %s", qPrintable ( m_output.errorString() ) );
  }

  if ( -1 == m_output.write ( m_reply->readAll() ) )
  {
    qWarning ( "Error saving: %s", qPrintable ( m_output.errorString() ) );
  }
}

/**
* Kopfdaten aus dem Header Ziehen
*/
void Downloader::getChangedMetaData()
{
  if ( m_reply->hasRawHeader ( "Content-Type" ) )
    m_metaType = m_reply->rawHeader ( "Content-Type" );
}

/**
* Nehme den aktuellen Download status engegen und schreibe
* in @ref inProgress, danach Signal @ref progress abstoßen.
*/
void Downloader::downloadProgress ( qint64 bReceived, qint64 bTotal )
{
  m_bytesLoaded = bReceived;
  if ( bTotal == -1 )
    return;

  inProgress = ( qint64 ) ( ( bReceived * 100.0 ) / bTotal );
  emit dataChanged ( progressIndex );
}

/**
* Wenn der Download Beendet ist Prozentzahl auf 100% setzen.
* Und wieder Signal @ref progress abstoßen.
*/
void Downloader::finished()
{
  if ( inProgress >= 100 )
    emit dataChanged ( progressIndex );
}

/**
* Das Zielverzeichnis mit Ausgabe Datei setzen
*/
void Downloader::setDestination ( const QUrl &url )
{
  if ( url.isValid() )
    destinationFilePath = url.path();
}

/**
* Setze den ModelIndex für die Prozentanzeige!
* Wenn diese gesetzt ist wird der Download gestartet!
*/
void Downloader::setStartProgressModel ( const QModelIndex &modelIndex )
{
  progressIndex = modelIndex;
  if ( destinationFilePath.isEmpty() )
    return;

  m_output.setFileName ( destinationFilePath );
  openDownload();
}

/**
* Gibt die aktuellen Download Status als \d% zurück.
*/
const QString Downloader::status()
{
  QString str ( QString::number ( inProgress ) );
  str.append ( "%" );
  return str;
}

/**
* Gibt die Download zeit aus.
*/
const QString Downloader::uploadTime()
{
  if ( ! m_reply->isRunning() || inProgress >= 100 )
    return m_uploadTime.toString ( "hh:mm:ss" );

  // Erstelle neue Leere Zeit
  QTime t ( 0, 0, 0 );
  // Schreibe Zeit mit Milisekunden in m_uploadTime
  m_uploadTime = t.addMSecs ( m_progressTime.elapsed() );
  return m_uploadTime.toString ( "hh:mm:ss" );
}

/**
* Gibt die aktuelle größe der Datei zurück.
* SI-Präfixe zur Basis 10 (Binärpräfixe)
*/
const QString Downloader::fileSize()
{
  QFileInfo info ( destinationFilePath );
  if ( info.exists() )
  {
    QString out;
    QTextStream stream ( &out );
    qint64 bytes = info.size ();
    if ( bytes > 1073741824 )
    {
      stream << ( bytes / 1073741824 ) << " GiB";
      return stream.readAll();
    }
    else if ( bytes > 1048576 )
    {
      stream << ( bytes / 1048576 ) << " MiB";
      return stream.readAll();
    }
    else if ( bytes > 1024 )
    {
      stream << ( bytes / 1024 ) << " KiB";
      return stream.readAll();
    }
    else
    {
      out = QString::number ( ( bytes ) );
      out.append ( " Bytes" );
      return out;
    }
  }
  else
    return QString ( "0 Bytes" );
}

/**
* Gibt die aktuelle Download URL zurück.
*/
const QUrl Downloader::url()
{
  if ( ! m_reply )
    return QUrl();

  return m_reply->request().url();
}

/**
* Gibt den aktuellen Pfad zur Zieldatei aus.
*/
const QString Downloader::destFile()
{
  return destinationFilePath;
}

const QString Downloader::metaType()
{
  return QString ( m_metaType );
}

Downloader::~Downloader()
{}
