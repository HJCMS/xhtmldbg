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

#include "downloadmanager.h"
#include "downloadstable.h"
#include "downloader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

DownloadManager::DownloadManager ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "downloadmanager" ) );
  setWindowTitle ( trUtf8 ( "(xhtmldbg) Download Manager" ) );
  setMinimumWidth ( 300 );
  setMinimumHeight ( 150 );
  setSizeGripEnabled ( true );
  setWindowModality ( Qt::NonModal );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );

  m_table = new DownloadsTable ( this );
  verticalLayout->addWidget ( m_table );

  setLayout ( verticalLayout );

  openDownloads.clear();
  show();
}

void DownloadManager::startDownload ( QNetworkReply *reply )
{
  foreach ( Downloader *it, openDownloads )
  {
    if ( it->url() == reply->request().url() )
    {
      setVisible ( true );
      setFocus ( Qt::ActiveWindowFocusReason );
      activateWindow();
      raise();
      return;
    }
  }
  Downloader *item = new Downloader ( reply, this );
  openDownloads << item;
}

void DownloadManager::download ( QNetworkReply *reply )
{
  if ( !reply || reply->request().url().isEmpty() )
    return;

  QVariant header = reply->header ( QNetworkRequest::ContentLengthHeader );
  bool ok;
  int size = header.toInt ( &ok );
  if ( ok && size == 0 )
    return;

  startDownload ( reply );
}

DownloadManager::~DownloadManager()
{
  openDownloads.clear();
}
