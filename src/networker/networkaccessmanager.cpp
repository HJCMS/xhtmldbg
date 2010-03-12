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

#include "networkaccessmanager.h"
#include "networksettings.h"
#include "authenticationdialog.h"
#include "certdialog.h"
#include "errorsdialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/* QtNetwork */
#include <QtNetwork/QAbstractNetworkCache>
#include <QtNetwork/QNetworkDiskCache>

NetworkAccessManager::NetworkAccessManager ( QObject * parent )
    : QNetworkAccessManager ( parent )
    , url ( QUrl ( "http://localhost" ) )
{
  m_networkSettings = new  NetworkSettings ( this );

  sslConfig = m_networkSettings->sslConfiguration();

  if ( m_networkSettings->value ( QLatin1String ( "enableProxy" ), false ).toBool() )
    setProxy ( m_networkSettings->getProxy() );

  trustedCertsHostsList << m_networkSettings->trustedCertsList();

  connect ( this, SIGNAL ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ),
            this, SLOT ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ),
            this, SLOT ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( finished ( QNetworkReply * ) ),
            this, SLOT ( replyFinished ( QNetworkReply * ) ) );

  connect ( this, SIGNAL ( sslErrors ( QNetworkReply *, const QList<QSslError> & ) ),
            this, SLOT ( certErrors ( QNetworkReply *, const QList<QSslError> & ) ) );

  QNetworkDiskCache *diskCache = new QNetworkDiskCache ( this );
  diskCache->setCacheDirectory ( m_networkSettings->storageDirectory() );
  setCache ( diskCache );
  xhtmlCache = cache();
}

QTextCodec* NetworkAccessManager::fetchHeaderEncoding ( QNetworkReply * reply )
{
  QString encoding ( "UTF-8" );
  if ( reply )
  {
    QByteArray cType = reply->rawHeader ( "Content-Type" );
    if ( ! cType.isEmpty() )
    {
      QString Charset ( cType );
      foreach ( QString param, Charset.split ( QRegExp ( "[\\s ]?;[\\s ]?" ) ) )
      {
        param.trimmed();
        if ( param.contains ( "charset", Qt::CaseInsensitive ) )
        {
          QString buf = param.split ( QRegExp ( "[\\s ]?=[\\s ]?" ) ).last();
          encoding = buf.toUpper();
          break;
        }
      }
    }
  }
  return QTextCodec::codecForName ( encoding.toAscii() );
}

void NetworkAccessManager::authenticationRequired ( QNetworkReply * reply, QAuthenticator * auth )
{
  AuthenticationDialog authDialog;
  authDialog.setRealm ( auth->realm(), reply->url().toString() );
  if ( authDialog.exec() == QDialog::Accepted )
  {
    auth->setUser ( authDialog.login() );
    auth->setPassword ( authDialog.pass() );
  }
}

void NetworkAccessManager::proxyAuthenticationRequired ( const QNetworkProxy &proxy, QAuthenticator * auth )
{
  AuthenticationDialog authDialog;
  authDialog.setRealm ( auth->realm(), proxy.hostName() );
  authDialog.setLogin ( m_networkSettings->value ( QLatin1String ( "proxyUser" ) ).toString() );
  if ( authDialog.exec() == QDialog::Accepted )
  {
    auth->setUser ( authDialog.login() );
    auth->setPassword ( authDialog.pass() );
  }
}

void NetworkAccessManager::certErrors ( QNetworkReply * reply, const QList<QSslError> &errors )
{
  QString certHost ( reply->url().host() );

  if ( trustedCertsHostsList.isEmpty() )
    trustedCertsHostsList << m_networkSettings->trustedCertsList();

  if ( trustedCertsHostsList.contains ( certHost ) )
  {
    reply->ignoreSslErrors();
    return;
  }
  else
  {
    QStringList messages;
    QSslCertificate cert = errors.at ( 0 ).certificate ();
    foreach ( QSslError err, errors )
    {
      messages << err.errorString();
    }
    CertDialog certDialog ( m_networkSettings );
    certDialog.setCertificate ( cert, certHost );
    certDialog.setMessages ( messages );
    if ( certDialog.exec() == QDialog::Accepted )
    {
      reply->ignoreSslErrors();
      trustedCertsHostsList.append ( certHost );
    }
  }
}

void NetworkAccessManager::replyErrors ( QNetworkReply::NetworkError err )
{
  if ( err == QNetworkReply::NoError )
    return;

  ErrorsDialog* errdial = new ErrorsDialog;
  connect ( errdial, SIGNAL ( errorMessage ( const QString & ) ),
            this, SIGNAL ( netNotify ( const QString & ) ) );

  if ( errdial->setError ( err ) )
    errdial->exec();

  delete errdial;
}

void NetworkAccessManager::replyFinished ( QNetworkReply *reply )
{
  if ( reply->url() == url )
  {
    QByteArray data;
    QString mimeType = reply->header ( QNetworkRequest::ContentTypeHeader ).toString();
    if ( mimeType.contains ( "text/html" ) )
    {
      QIODevice* dev = xhtmlCache->data ( reply->url() );
      if ( ! dev )
      {
        qDebug() << Q_FUNC_INFO << "Invalid IODevice";
        emit xhtmlSourceChanged ( QString::null );
        return;
      }

      data = dev->readAll();
      if ( data.isEmpty() )
      {
        qDebug() << Q_FUNC_INFO << "No Cache";
        emit xhtmlSourceChanged ( QString::null );
        return;
      }

      QTextCodec* codec = QTextCodec::codecForHtml ( data, fetchHeaderEncoding ( reply ) );
      QString content = codec->toUnicode ( data );
      emit xhtmlSourceChanged ( content );
      delete dev;
    }
  }
}

void NetworkAccessManager::setUrl ( const QUrl &u )
{
  url = u;
}

QNetworkReply* NetworkAccessManager::createRequest ( QNetworkAccessManager::Operation op,
        const QNetworkRequest &req,
        QIODevice *data )
{
  QNetworkRequest request = m_networkSettings->requestOptions ( req );

  QNetworkReply* reply = QNetworkAccessManager::createRequest ( op, request, data );
  reply->setSslConfiguration ( sslConfig );

  if ( reply->url() == url )
  {
    connect ( reply, SIGNAL ( error ( QNetworkReply::NetworkError ) ),
              this, SLOT ( replyErrors ( QNetworkReply::NetworkError ) ) );
  }
  return reply;
}

const QUrl NetworkAccessManager::getUrl()
{
  return url;
}

NetworkAccessManager::~NetworkAccessManager()
{
  if ( xhtmlCache )
    xhtmlCache->clear();
}
