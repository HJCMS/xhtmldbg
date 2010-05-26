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

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>

Downloader::Downloader ( QNetworkReply * reply, QWidget * parent )
    : QWidget ( parent )
    , m_reply ( reply )
    , defaultLocation ( QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) )
    , m_bytesLoaded ( 0 )
    , inProgress ( 0 )
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
  connect ( m_reply, SIGNAL ( downloadProgress ( qint64, qint64 ) ),
            this, SLOT ( downloadProgress ( qint64, qint64 ) ) );
  connect ( m_reply, SIGNAL ( finished() ), this, SLOT ( finished() ) );

  // jetzt den Timer starten
  m_progressTime.start();
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
* Nehme den aktuellen Download status engegen und schreibe
* in @ref inProgress, danach Signal @ref progress abstoßen.
*/
void Downloader::downloadProgress ( qint64 bReceived, qint64 bTotal )
{
  m_bytesLoaded = bReceived;
  if ( bTotal == -1 )
    return;

  inProgress = ( int ) ( ( bReceived * 100.0 ) / bTotal );
  emit progress ( progressIndex );
}

/**
* Wenn der Download Beendet ist Prozentzahl auf 100% setzen.
* Und wieder Signal @ref progress abstoßen.
*/
void Downloader::finished()
{
  inProgress = 100;
  // m_progressTime.stop();
  emit progress ( progressIndex );
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
* Gibt die aktuelle Download URL zurück.
*/
const QUrl Downloader::url()
{
  if ( ! m_reply )
    return QUrl();

  return m_reply->request().url();
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
  if ( ! m_reply )
    return QString ( "00:00" );

  if ( inProgress > 100 )
    return QString ( "00:00" );

  QString st;
  st.sprintf ( "%d", m_progressTime.elapsed() );
  return st;
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
* Gibt den aktuellen Pfad zur Zieldatei aus.
*/
const QString Downloader::destFile()
{
  return destinationFilePath;
}

Downloader::~Downloader()
{}
