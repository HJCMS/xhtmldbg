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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>

/* xhtmldbg */
#include "settings.h"

class Downloader;
class DownloadsTable;
class DownloadsInfo;
class AutoSaver;

class DownloadManager : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    Settings* cfg;
    DownloadsTable* m_table;
    DownloadsInfo* m_info;
    AutoSaver *m_autoSaver;
    QPushButton* m_stopButton;
    QPushButton* m_removeButton;
    QPushButton* m_clearButton;
    void startDownload ( QNetworkReply *reply, const QUrl &destination );

  private Q_SLOTS:
    void save();

  public Q_SLOTS:
    void download ( QNetworkReply *reply, const QUrl &destination );

  public:
    DownloadManager ( QWidget * parent = 0, Settings * setting = 0 );
    virtual ~DownloadManager();
};

#endif
