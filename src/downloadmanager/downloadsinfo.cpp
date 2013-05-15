/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "downloadsinfo.h"
#include "downloader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QFontMetrics>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>

/* KDE */
#include <KDE/KLocale>

DownloadsInfo::DownloadsInfo ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "downloadsinfowidget" ) );
  setContentsMargins ( 1, 1, 1, 1 );

  QGridLayout* layout = new QGridLayout ( this );
  layout->setContentsMargins ( 0, 1, 0, 1 );
  layout->setSpacing ( 2 );

  int gridRow = 0; // inkrementeller Layout Zähler
  Qt::Alignment infoAlign = ( Qt::AlignRight | Qt::AlignVCenter );
  Qt::Alignment dataAlign = ( Qt::AlignLeft | Qt::AlignVCenter );
  QSizePolicy defaultSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred, QSizePolicy::Label );

  QLabel* txt_main_info = new QLabel ( this );
  txt_main_info->setObjectName ( QLatin1String ( "txt_main_info" ) );
  txt_main_info->setAlignment ( dataAlign );
  txt_main_info->setIndent ( 2 );
  txt_main_info->setText ( i18n ( "<b>Download Summary</b>" ) );
  layout->addWidget ( txt_main_info, gridRow++, 0, 1, 2, Qt::AlignLeft );

  // Download Url
  QLabel* txt_url_info = new QLabel ( this );
  txt_url_info->setObjectName ( QLatin1String ( "txt_url_info" ) );
  txt_url_info->setAlignment ( infoAlign );
  txt_url_info->setText ( i18n ( "Upload URL" ) );
  layout->addWidget ( txt_url_info, gridRow, 0 );
  layout->addWidget ( spacer(), gridRow, 1 );

  uploadUrl = new QLabel ( this );
  uploadUrl->setObjectName ( QLatin1String ( "txt_uploadUrl" ) );
  uploadUrl->setAlignment ( dataAlign );
  uploadUrl->setSizePolicy ( defaultSizePolicy );
  uploadUrl->setFrameShape ( QFrame::StyledPanel );
  layout->addWidget ( uploadUrl, gridRow++, 2 );

  // Ziel Verzeichnis
  QLabel* txt_destination_info = new QLabel ( this );
  txt_destination_info->setObjectName ( QLatin1String ( "txt_destination_info" ) );
  txt_destination_info->setAlignment ( infoAlign );
  txt_destination_info->setText ( i18n ( "Destination Path" ) );
  layout->addWidget ( txt_destination_info, gridRow, 0 );
  layout->addWidget ( spacer(), gridRow, 1 );

  destinationPath = new QLabel ( this );
  destinationPath->setObjectName ( QLatin1String ( "txt_destinationPath" ) );
  destinationPath->setAlignment ( dataAlign );
  destinationPath->setSizePolicy ( defaultSizePolicy );
  destinationPath->setFrameShape ( QFrame::StyledPanel );
  destinationPath->setOpenExternalLinks ( true );
  layout->addWidget ( destinationPath, gridRow++, 2 );

  // Datei Größe
  QLabel* txt_size_info = new QLabel ( this );
  txt_size_info->setObjectName ( QLatin1String ( "txt_size_info" ) );
  txt_size_info->setAlignment ( infoAlign );
  txt_size_info->setText ( i18n ( "Filesize" ) );
  layout->addWidget ( txt_size_info, gridRow, 0 );
  layout->addWidget ( spacer(), gridRow, 1 );

  fileSize = new QLabel ( this );
  fileSize->setObjectName ( QLatin1String ( "txt_fileSize" ) );
  fileSize->setAlignment ( dataAlign );
  fileSize->setSizePolicy ( defaultSizePolicy );
  fileSize->setFrameShape ( QFrame::StyledPanel );
  layout->addWidget ( fileSize, gridRow++, 2 );

  // Daten Mime-Type
  QLabel* txt_mime_info = new QLabel ( this );
  txt_mime_info->setObjectName ( QLatin1String ( "txt_mime_info" ) );
  txt_mime_info->setAlignment ( infoAlign );
  txt_mime_info->setText ( i18n ( "Mime-Type" ) );
  layout->addWidget ( txt_mime_info, gridRow, 0 );
  layout->addWidget ( spacer(), gridRow, 1 );

  mimeType = new QLabel ( this );
  mimeType->setObjectName ( QLatin1String ( "txt_mimeType" ) );
  mimeType->setAlignment ( dataAlign );
  mimeType->setSizePolicy ( defaultSizePolicy );
  mimeType->setFrameShape ( QFrame::StyledPanel );
  layout->addWidget ( mimeType, gridRow++, 2 );

  // Spacer
  QSpacerItem* spacerItem = new QSpacerItem ( 5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding );
  layout->addItem ( spacerItem, gridRow, 0, 1, 2, Qt::AlignHCenter );

  setLayout ( layout );
}

/**
* Doppelpunkt Label das mehrfach verwendet wird.
*/
QLabel* DownloadsInfo::spacer()
{
  QLabel* label = new QLabel ( this );
  label->setAlignment ( ( Qt::AlignCenter | Qt::AlignVCenter ) );
  label->setText ( ":" );
  QFontMetrics metrics ( label->font() );
  label->setMaximumWidth ( ( metrics.width ( ':' ) * 3 ) );
  return label;
}

/**
* Hier werden die Informationen für die Labels gesetzt.
* @li @ref uploadUrl mit @ref Downloader::url setzen
* @li @ref fileSize mit @ref Downloader::fileSize setzen
* @li @ref mimeType mit @ref Downloader::metaType setzen
* @li @ref destinationPath mit @ref Downloader::destFile
* Wenn die Zieldatei existiert wird ein Link bei @ref destinationPath erzeugt.
* Sollte kein Meta Datentype mit "Content-Type" übergeben werden dann wird
* der Standard Type "application/octet-stream" verwendet!
*/
void DownloadsInfo::setInfoData ( Downloader * item )
{
  uploadUrl->setText ( item->url().toString() );
  fileSize->setText ( item->fileSize() );
  mimeType->setText ( item->metaType() );
  // Prüfen ob die Ziel Datei existiert etc.
  QFileInfo info ( item->destFile() );
  if ( ! info.exists() )
    return;

  // Wenn der Download noch aktiv ist nur den Pfad anzeigen!
  if ( item->isRunning() )
  {
    destinationPath->setText ( item->destFile() );
    return;
  }

  // Download ist fertig und existiert dann einen Link erzeugen.
  QString html = QString ( "<a href=\"%2\" type=\"%1\" target=\"_blank\" title=\"%3\">%2</a>" )
                  .arg ( item->metaType(), item->destFile(), i18n ( "Open with external Application" ) );
  destinationPath->setText ( html );
}
