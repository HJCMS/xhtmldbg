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
#include "autosaver.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDesktopServices>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

DownloadManager::DownloadManager ( QWidget * parent, QSettings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
    , m_autoSaver ( new AutoSaver ( this ) )
{
  setObjectName ( QLatin1String ( "downloadmanager" ) );
  setWindowTitle ( trUtf8 ( "Downloads" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  QScrollArea* m_scrollArea = new QScrollArea ( this );
  m_scrollArea->setObjectName ( QLatin1String ( "downloadmanagerscrollarea" ) );
  m_scrollArea->setContentsMargins ( 0, 0, 0, 0 );
  m_scrollArea->setWidgetResizable ( true );

  m_table = new DownloadsTable ( m_scrollArea );
  m_scrollArea->setWidget ( m_table );

  setWidget ( m_scrollArea );

  openDownloadsList.clear();
}

void DownloadManager::save()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void DownloadManager::startDownload ( QNetworkReply *reply, const QUrl &destination )
{
  qDebug() << Q_FUNC_INFO << destination;
  foreach ( Downloader *it, openDownloadsList )
  {
    if ( it->url() == reply->request().url() )
      return;
  }
  Downloader *item = new Downloader ( reply, this );
  item->setDestination ( destination );
  m_table->setDownloadItem ( item );
  openDownloadsList << item;
}

void DownloadManager::download ( QNetworkReply *reply, const QUrl &destination )
{
  if ( !reply || reply->request().url().isEmpty() )
    return;

  QVariant header = reply->header ( QNetworkRequest::ContentLengthHeader );
  bool ok;
  int size = header.toInt ( &ok );
  if ( ok && size == 0 )
    return;

  startDownload ( reply, destination );
}

DownloadManager::~DownloadManager()
{
  openDownloadsList.clear();
  m_autoSaver->changeOccurred();
  m_autoSaver->saveIfNeccessary();
}
