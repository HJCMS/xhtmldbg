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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

/* QtCore */
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QUrl>
#include <QtCore/QModelIndex>

/* QtGui */
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>

class Downloader : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QNetworkReply* m_reply;
    const QString defaultLocation;
    QModelIndex progressIndex;
    qint64 m_bytesLoaded;
    int inProgress;
    QString destinationFilePath;
    QTime m_progressTime;
    QFile m_output;
    void openDownload();

  private Q_SLOTS:
    void downloadReadyRead();
    void downloadProgress ( qint64 bReceived, qint64 bTotal );
    void finished();

  Q_SIGNALS:
    void progress ( const QModelIndex & );

  public Q_SLOTS:
    void abort();
    void restart();

  public:
    explicit Downloader ( QNetworkReply * reply, QWidget * parent = 0 );
    void setStartProgressModel ( const QModelIndex & );
    const QUrl url();
    const QString status();
    const QString uploadTime();
    void setDestination ( const QUrl & );
    const QString destFile();
    ~Downloader();
};

#endif
