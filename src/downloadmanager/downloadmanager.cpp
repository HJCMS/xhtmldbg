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
#include "downloadstablemodel.h"
#include "downloader.h"

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
{
  setObjectName ( QLatin1String ( "downloadmanager" ) );
  setWindowTitle ( trUtf8 ( "Downloads" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  QWidget* layoutWidget = new QWidget ( this );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layoutWidget );
  verticalLayout->setContentsMargins ( 0, 0, 0, 10 );

  QScrollArea* m_scrollArea = new QScrollArea ( layoutWidget );
  m_scrollArea->setObjectName ( QLatin1String ( "downloadmanagerscrollarea" ) );
  m_scrollArea->setContentsMargins ( 0, 0, 0, 0 );
  verticalLayout->addWidget ( m_scrollArea );

  m_table = new QTableView ( m_scrollArea );
  m_table->setObjectName ( QLatin1String ( "downloadstableviewer" ) );
  m_scrollArea->setWidget ( m_table );

  m_model = new DownloadsTableModel ( m_table );
  m_table->setModel ( m_model );

  QHBoxLayout* horizontalLayout = new QHBoxLayout;
  horizontalLayout->setObjectName ( QLatin1String ( "horizontallayout" ) );
  horizontalLayout->setContentsMargins ( 0, 0, 0, 0 );
  horizontalLayout->setSpacing ( 5 );
  horizontalLayout->addStretch ( 1 );

  m_stopButton = new QPushButton ( trUtf8 ( "Stop" ), layoutWidget );
  m_stopButton->setIcon ( QIcon::fromTheme ( QLatin1String ( "process-stop" ) ) );
  m_stopButton->setObjectName ( QLatin1String ( "stoptdownloadbutton" ) );
  horizontalLayout->addWidget ( m_stopButton );

  m_removeButton = new QPushButton ( trUtf8 ( "Remove" ), layoutWidget );
  m_removeButton->setIcon ( QIcon::fromTheme ( QLatin1String ( "process-stop" ) ) );
  m_removeButton->setObjectName ( QLatin1String ( "removetdownloadbutton" ) );
  horizontalLayout->addWidget ( m_removeButton );

  m_clearButton = new QPushButton ( trUtf8 ( "Clear" ), layoutWidget );
  m_clearButton->setIcon ( QIcon::fromTheme ( QLatin1String ( "process-stop" ) ) );
  m_clearButton->setObjectName ( QLatin1String ( "cleardownloadbutton" ) );
  horizontalLayout->addWidget ( m_clearButton );

  verticalLayout->addLayout ( horizontalLayout );
  layoutWidget->setLayout ( verticalLayout );
  setWidget ( layoutWidget );

  connect ( m_stopButton, SIGNAL ( clicked() ), this, SLOT ( abortActiveDownload() ) );
  connect ( m_removeButton, SIGNAL ( clicked() ), this, SLOT ( removeDownload() ) );
  connect ( m_clearButton, SIGNAL ( clicked() ), this, SLOT ( removeAllDownloads() ) );

  openDownloadsList.clear();
}

void DownloadManager::abortActiveDownload()
{
  qDebug() << Q_FUNC_INFO;
}

void DownloadManager::removeDownload()
{
  qDebug() << Q_FUNC_INFO;
}

void DownloadManager::removeAllDownloads()
{
  qDebug() << Q_FUNC_INFO;
}

void DownloadManager::startDownload ( QNetworkReply *reply, const QUrl &destination )
{
  qDebug() << Q_FUNC_INFO << destination;
  foreach ( Downloader *it, openDownloadsList )
  {
    if ( it->url() == reply->request().url() )
    {
      setFocus ( Qt::TabFocusReason );
      return;
    }
  }
  Downloader *item = new Downloader ( reply, this );
  item->setDestination ( destination );
  m_model->addDownload ( item );
  openDownloadsList << item;

  item->openDownload();
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
}
