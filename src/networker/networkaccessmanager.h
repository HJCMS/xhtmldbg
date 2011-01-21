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
    QNetworkReply* m_networkReply;
    QByteArray peekPostData;
    QList<QString> trustedCertsHostsList;
    QStringList certCustodyPending;
    QAbstractNetworkCache* xhtmlCache;
    QSslConfiguration sslConfig;
    QTextCodec* fetchHeaderEncoding ( QNetworkReply * reply );
    void fetchPostedData ( const QNetworkRequest &req, QIODevice * );
    const QByteArray peekDeviceData ( QIODevice * );
    void openLocalFile ( const QUrl & );

  private Q_SLOTS:
    void authenticationRequired ( QNetworkReply *, QAuthenticator * );
    void proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * );
    void certErrors ( QNetworkReply *, const QList<QSslError> & );
    void replyErrors ( QNetworkReply::NetworkError );
    void peekReplyProcess ();

  Q_SIGNALS:
    void netNotify ( const QString & );
    void statusBarMessage ( const QString & );
    void postReplySource ( const QString & );
    void receivedHostHeaders ( const QUrl &, const QMap<QString,QString> & );
    void postedRefererData ( const QUrl &, const QStringList & );

  protected:
    virtual QNetworkReply* createRequest ( QNetworkAccessManager::Operation op,
                                           const QNetworkRequest &req, QIODevice *data = 0 );

  public Q_SLOTS:
    void replyFinished ( QNetworkReply * );
    void setUrl ( const QUrl & );

  public:
    NetworkAccessManager ( QObject *parent = 0 );
    const QUrl getUrl();
    NetworkCookie* cookieJar() const;
    QNetworkReply* head ( const QNetworkRequest & );
    QNetworkReply* post ( const QNetworkRequest &, QIODevice * );
    QNetworkReply* post ( const QNetworkRequest &, const QByteArray & );
    QNetworkReply* get ( const QNetworkRequest &req );
    virtual ~NetworkAccessManager();
};

#endif
