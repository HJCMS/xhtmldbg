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

/* QtNetwork */
// #include <QtNetwork>

Downloader::Downloader ( QNetworkReply * reply, QWidget * parent )
    : QWidget ( parent )
    , m_reply ( reply )
    , defaultLocation ( QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) )
    , m_bytesLoaded ( 0 )
    , inProgress ( 0 )
{
  setObjectName ( QLatin1String ( "downloader" ) );
}

void Downloader::openDownload()
{
  if ( ! m_reply )
    return;

  m_reply->setParent ( this );
  connect ( m_reply, SIGNAL ( readyRead() ), this, SLOT ( downloadReadyRead() ) );
  connect ( m_reply, SIGNAL ( downloadProgress ( qint64, qint64 ) ),
            this, SLOT ( downloadProgress ( qint64, qint64 ) ) );
}

void Downloader::downloadReadyRead()
{
  qDebug() << Q_FUNC_INFO;
}

void Downloader::downloadProgress ( qint64 bReceived, qint64 bTotal )
{
  m_bytesLoaded = bReceived;
  if ( bTotal == -1 )
    return;

  inProgress = ( ( bReceived * 100.0 ) / bTotal );
  emit dataChanged();
  emit progress ( url(), true );
}

const QUrl Downloader::url()
{
  if ( ! m_reply )
    return QUrl();

  return m_reply->request().url();
}

const QString Downloader::status()
{
  QString str ( QString::number ( inProgress ) );
  str.append ( "%" );
  return str;
}

const QString Downloader::uploadTime()
{
  if ( ! m_reply )
    return QString ( "00:00" );

  return QString ( "00:00" );
}

void Downloader::setDestination ( const QUrl &url )
{
  if ( url.isValid() )
    destinationFilePath = url.path();
}

const QString Downloader::destFile()
{
  return destinationFilePath;
}

Downloader::~Downloader()
{}
