/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

/* QtNetwork */
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslError>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslConfiguration>

class NetworkSettings;
class NetworkCookie;
class ErrorsDialog;
class NetworkCache;

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QUrl requestUrl;

    NetworkSettings* m_networkSettings;
    NetworkCookie* m_networkCookie;
    ErrorsDialog* m_errorsDialog;
    NetworkCache* m_networkCache;

    QNetworkReply* m_networkReply;
    QSharedPointer<QNetworkReply> m_replyPointer;
    QByteArray peekPostData;
    QList<QString> trustedCertsHostsList;
    QStringList pendingCerts;
    QAbstractNetworkCache* xhtmlCache;
    QSslConfiguration sslConfig;

    QTextCodec* fetchHeaderEncoding ( QNetworkReply * reply );
    void fetchPostedData ( const QNetworkRequest &req, QIODevice * );
    const QByteArray peekDeviceData ( QIODevice * );
    void openLocalFile ( const QUrl & );

  private Q_SLOTS:
    /** cache for Url exists for read */
    void cacheReadyRead ( const QUrl & );

    /** open authentication dialog */
    void authenticationRequired ( QNetworkReply *, QAuthenticator * );

    /** open ssl authentication dialog */
    void proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * );

    /** read SSL Errors */
    void certErrors ( QNetworkReply *, const QList<QSslError> & );

    /** read Network Errors */
    void replyErrors ( QNetworkReply::NetworkError );

    /** peek data from reply device */
    void peekReplyProcess ();

  Q_SIGNALS:
    /** Error Messages from ErrorsDialog */
    void netNotify ( const QString & );

    /** send statusBar Messages */
    void statusBarMessage ( const QString & );

    /** returned content from http post */
    void postReplySource ( const QUrl &url, const QString & );

    /** returned content from local file */
    void localReplySource ( const QUrl &url, const QString & );

    /** HTTP-Content Headers */
    void receivedHostHeaders ( const QUrl &, const QMap<QString,QString> & );

    /** Posted Data from Client */
    void postedRefererData ( const QUrl &, const QStringList & );

    /** Request Link from Client finished */
    void urlLoadFinished ( const QUrl & );

  protected:
    /** send requests and return an opened networkreply state */
    virtual QNetworkReply* createRequest ( QNetworkAccessManager::Operation op,
                                           const QNetworkRequest &req, QIODevice *data = 0 );

  public Q_SLOTS:
    /** if NetworManager finished read Reply Headers */
    void replyFinished ( QNetworkReply * );

    /** set Current Requested URL */
    void setUrl ( const QUrl & );

  public:
    NetworkAccessManager ( QObject *parent = 0 );

    /** get Current Request URL */
    const QUrl getUrl();

    /** Network Cookie Manager */
    NetworkCookie* cookieJar() const;

    /** sends an HTTP HEAD request */
    QNetworkReply* head ( const QNetworkRequest & );

    /** sends an HTTP POST request */
    QNetworkReply* post ( const QNetworkRequest &, QIODevice * );

    /** sends an HTTP POST request */
    QNetworkReply* post ( const QNetworkRequest &, const QByteArray & );

    /** sends an HTTP GET request  */
    QNetworkReply* get ( const QNetworkRequest &req );

    /** get cached Source for URL */
    void triggerCache ( const QUrl &url );

    virtual ~NetworkAccessManager();
};

#endif
