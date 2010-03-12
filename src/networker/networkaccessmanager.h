/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

/* QtNetwork */
#include <QtNetwork/QAbstractNetworkCache>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslError>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslConfiguration>

class NetworkSettings;

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( const QUrl url READ getUrl WRITE setUrl )

  private:
    QList<QString> trustedCertsHostsList;
    NetworkSettings* m_networkSettings;
    QAbstractNetworkCache* xhtmlCache;
    QSslConfiguration sslConfig;
    QTextCodec* fetchHeaderEncoding ( QNetworkReply * );
    QUrl url;

  private Q_SLOTS:
    void authenticationRequired ( QNetworkReply *, QAuthenticator * );
    void proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * );
    void certErrors ( QNetworkReply *, const QList<QSslError> & );
    void replyErrors ( QNetworkReply::NetworkError );

  Q_SIGNALS:
    void netNotify ( const QString & );
    void statusBarMessage ( const QString & );
    void xhtmlSourceChanged ( const QString & );
    void receivedHeaders ( const QMap<QString,QString> & );

  public Q_SLOTS:
    void replyFinished ( QNetworkReply * );
    void setUrl ( const QUrl & );

  public:
    NetworkAccessManager ( QObject *parent = 0 );
    QNetworkReply* createRequest ( QNetworkAccessManager::Operation op,
                                   const QNetworkRequest &, QIODevice * );
    const QUrl getUrl();
    ~NetworkAccessManager();
};

#endif
