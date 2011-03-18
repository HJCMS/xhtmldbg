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

#include "downloadmanager.h"
#include "downloadstable.h"
#include "downloadsinfo.h"
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
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

DownloadManager::DownloadManager ( QWidget * parent, Settings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
    , m_autoSaver ( new AutoSaver ( this ) )
{
  setObjectName ( QLatin1String ( "downloadmanager" ) );
  setWindowTitle ( i18n ( "Downloads" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  QScrollArea* m_scrollArea = new QScrollArea ( this );
  m_scrollArea->setObjectName ( QLatin1String ( "downloadmanagerscrollarea" ) );
  m_scrollArea->setContentsMargins ( 0, 0, 0, 0 );
  m_scrollArea->setWidgetResizable ( true );

  QSplitter* m_splitter = new QSplitter ( Qt::Vertical, m_scrollArea );
  m_splitter->setObjectName ( QLatin1String ( "downloadmanagersplitter" ) );
  m_splitter->setContentsMargins ( 0, 1, 0, 1 );

  // Eigentliche Download Darstellung
  m_table = new DownloadsTable ( m_splitter );
  m_splitter->insertWidget ( 0, m_table );

  // Informations Widget
  m_info = new DownloadsInfo ( m_splitter );
  m_splitter->insertWidget ( 1, m_info );

  m_scrollArea->setWidget ( m_splitter );
  setWidget ( m_scrollArea );

  connect ( m_table, SIGNAL ( itemClicked ( Downloader * ) ),
            m_info, SLOT ( setInfoData ( Downloader * ) ) );
}

/**
* @todo Offene Downloads in eine Datenbank schreiben!
*/
void DownloadManager::save()
{}

/**
* Erstelle ein Object von @ref Downloader und setze die Datenfelder.
* @note Der Download startet Automatisch sobald mit @ref DownloadsTableModel \n
*       der ModelIndex für den Progress Eintrag gesetz wurde!
*/
void DownloadManager::startDownload ( QNetworkReply *reply, const QUrl &destination )
{
  Downloader *item = new Downloader ( reply, this );
  item->setDestination ( destination );
  if ( ! m_table->setDownloadItem ( item ) )
  {
    qWarning ( "Download \"%s\" already exists.", qPrintable ( destination.toString() ) );
    delete item;
  }
}

/**
* Nehme einen Download entgegen und prüfe ob es sich bei @param destination
* um eine Valide Url handelt. Zusätzlich wird der HTTP/1.0 ContentLength
* Header auf seine Datengröße geprüft. Ist alles ok dann an die Methode
* @ref startDownload übergeben!
*/
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

/**
* Beim Beenden
* @li Downloads Liste Leeren
* @li Überprüfen ob ein Process noch läuft
* @li Wenn ja offene Aufträge erst beenden
*/
DownloadManager::~DownloadManager()
{
  m_autoSaver->changeOccurred();
  m_autoSaver->saveIfNeccessary();
}
