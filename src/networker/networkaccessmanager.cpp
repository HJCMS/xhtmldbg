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

#include "networkaccessmanager.h"
#include "networksettings.h"
#include "networkcookie.h"
#include "authenticationdialog.h"
#include "certdialog.h"
#include "errorsdialog.h"

#include <climits>

/* QtCore */
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>

NetworkAccessManager::NetworkAccessManager ( QObject * parent )
    : QNetworkAccessManager ( parent )
    , url ( QUrl ( "http://localhost" ) )
{
  m_networkSettings = new  NetworkSettings ( this );
  m_networkCookie = new NetworkCookie ( m_networkSettings, this );

  sslConfig = m_networkSettings->sslConfiguration();

  if ( m_networkSettings->value ( QLatin1String ( "enableProxy" ), false ).toBool() )
    setProxy ( m_networkSettings->getProxy() );

  trustedCertsHostsList << m_networkSettings->trustedCertsList();

  setCookieJar ( m_networkCookie );

  htmlReply = 0x00;

  connect ( this, SIGNAL ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ),
            this, SLOT ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ),
            this, SLOT ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( finished ( QNetworkReply * ) ),
            this, SLOT ( replyFinished ( QNetworkReply * ) ) );

  connect ( this, SIGNAL ( sslErrors ( QNetworkReply *, const QList<QSslError> & ) ),
            this, SLOT ( certErrors ( QNetworkReply *, const QList<QSslError> & ) ) );
}

/**
* Suche bei der Netzwerk antwort nach dem Content-Type und ermittle
* den Zeichensatz. Dabei ist der Standard "UTF-8" der zurück gegeben wird.
*/
QTextCodec* NetworkAccessManager::fetchHeaderEncoding ( QNetworkReply * reply )
{
  QString encoding ( "UTF-8" );
  if ( reply )
  {
    QByteArray cType = reply->rawHeader ( QByteArray ( "Content-Type" ) );
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

/**
* Wenn die Netzwerk Anfrage eine Authentifizierung verlangt.
* Dann, an dieser Setlle noch mal einen Dialog anzeigen.
*/
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

/**
* Wenn es ein Proxy Verbindung ist und keine Konfiguration
* vorhanden oder die Authentifizierung fehlschlägt.
* Dann, an dieser Setlle noch mal einen Dialog anzeigen.
*/
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

/**
* Zertifizierungs Fehler Meldungen verarbeiten.
*/
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

/**
* Netzwerkfehler Meldungen verarbeiten.
*/
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

/**
* Server antworten vom Speicher lesen.
* @see peekReplyProcess
*/
const QByteArray NetworkAccessManager::peekDeviceData ( QIODevice * device )
{
  QByteArray readBytes;
  qint64 maxBufferSize = ( UCHAR_MAX * 1024 );
  qint64 bytesToRead = qMax<qint64> ( 0, maxBufferSize );
  bytesToRead = qBound<qint64> ( 1, bytesToRead, device->bytesAvailable() );
  readBytes.resize ( bytesToRead );
  qint64 topicalBytes = device->peek ( readBytes.data(), readBytes.size() );
  if ( topicalBytes != -1 )
    readBytes.resize ( topicalBytes );

  return readBytes;
}

/**
* An dieser Stelle werden die Antworten abgefangen.
* Weil QWebKit keinen Originalen Quelltext zurück gibt muss dies
* an dieser erfolgen. Dabei ist es wichtig das die Daten vom Device
* abgegriffen und nicht ausgelesen werden. Weil sonst die IO Device
* den Speicher freigeben würde und nichts mehr beim WebKit ankommt.
* Siehe auch @ref peekDeviceData
*/
void NetworkAccessManager::peekReplyProcess()
{
  if ( htmlReply )
  {
    QString mimeType = htmlReply->header ( QNetworkRequest::ContentTypeHeader ).toString();
    if ( ! mimeType.contains ( "text/html" ) )
      return;

    if ( htmlReply->hasRawHeader ( QByteArray ( "location" ) ) )
    {
      emit netNotify ( trUtf8 ( "Multiple Content-Location header from POST Request received. Note - Many Webservers are free to ignore this. (300)" ) );
      peekPostData.clear();
    }

    QIODevice* copyDevice = htmlReply;
    if ( ! copyDevice || ! htmlReply->isOpen() )
      return;

    peekPostData.append ( peekDeviceData ( copyDevice ) );

    // FIXME I know this is a very difficult hack :-/
    if ( peekPostData.contains ( QByteArray ( "</html>" ) ) )
    {
      QTextCodec* codec = QTextCodec::codecForHtml ( peekPostData, fetchHeaderEncoding ( htmlReply ) );
      // qDebug() << codec->toUnicode ( peekPostData );
      emit postReplySource ( codec->toUnicode ( peekPostData ) );
    }
  }
}

/**
* Wenn der Benutzer ein POST Formular absendet.
* Werden die Daten an dieser Stelle abgefangen.
*/
void NetworkAccessManager::fetchPostedData ( const QNetworkRequest &req, QIODevice * data )
{
  if ( req.rawHeader ( QByteArray ( "Referer" ) ).isEmpty() )
    return;

  if ( data->isOpen() )
  {
    QByteArray postData = QByteArray::fromPercentEncoding ( data->peek ( UCHAR_MAX * 1024 ) );
    QTextCodec* codec = QTextCodec::codecForHtml ( postData, QTextCodec::codecForName ( "UTF-8" ) );
    QString string = codec->toUnicode ( postData );
    if ( string.contains ( "WebKitFormBoundary" ) )
    {
      QStringList buffer, list;
      QStringList line = string.split ( "\n" );
      line.removeFirst();

      for ( int l = 0; l < line.size(); l++ )
      {
        QString str = line.at ( l ).trimmed();
        if ( str.contains ( "Content-Disposition" ) )
          buffer << str.split ( "name=" ).last().remove ( "\"" );
        if ( str.contains ( "WebKitFormBoundary" ) )
          buffer << line.at ( ( l - 1 ) );
        else
          continue;
      }

      if ( buffer.size() % 2 != 0 )
        buffer << "";

      for ( int i = 0; i < buffer.size(); i += 2 )
      {
        list << ( buffer.at ( i ) + "=" + buffer.at ( ( i+1 ) ) );
      }
      buffer.clear();
      // qDebug() << list;
      emit postedRefererData ( req.url(), list );
      return;
    }
    QStringList param = string.split ( QString::fromUtf8 ( "&" ) );
    if ( param.size() >= 1 )
      emit postedRefererData ( req.url(), param );
  }
}

/**
* Wenn eine Antwort vom Server eingeht und dies ein text/html Type ist.
* Wird der Content-Type ausgelesen und an das Signal @ref receivedHostHeaders
* gesendet.
*/
void NetworkAccessManager::replyFinished ( QNetworkReply *reply )
{
  if ( reply && reply->url().isValid() )
  {
    QMap<QString,QString> map;
    foreach ( QByteArray k, reply->rawHeaderList() )
    {
      map[ QString ( k ) ] = QString ( reply->rawHeader ( k ) );
    }
    emit receivedHostHeaders ( reply->url(), map );
  }
}

/**
* Setzt die Virtuelle @ref url Variable
*/
void NetworkAccessManager::setUrl ( const QUrl &u )
{
  url = u;
}

/**
* Gibt die Variable @ref url der letzten Anfrage zurück.
*/
const QUrl NetworkAccessManager::getUrl()
{
  return url;
}

/**
* Gibt den Pointer auf @ref NetworkCookie zurück.
*/
NetworkCookie* NetworkAccessManager::cookieJar() const
{
  return m_networkCookie;
}

/**
* Primäre Netzwerk anfragen verarbeitung.
*/
QNetworkReply* NetworkAccessManager::createRequest ( QNetworkAccessManager::Operation op,
        const QNetworkRequest &req,
        QIODevice * data )
{
  QNetworkRequest request = m_networkSettings->requestOptions ( req );
  setUrl ( QUrl ( request.url().toString ( QUrl::RemoveFragment ) ) );

  QNetworkReply* reply = QNetworkAccessManager::createRequest ( op, request, data );
  reply->setReadBufferSize ( ( UCHAR_MAX * 1024 ) );
  reply->setSslConfiguration ( sslConfig );
  if ( op == QNetworkAccessManager::PostOperation && data )
  {
    peekPostData.clear();
    htmlReply = reply;
    fetchPostedData ( htmlReply->request(), data );
    connect ( htmlReply, SIGNAL ( readyRead() ), this, SLOT ( peekReplyProcess() ) );
  }

  if ( reply->url() == url )
  {
    connect ( reply, SIGNAL ( error ( QNetworkReply::NetworkError ) ),
              this, SLOT ( replyErrors ( QNetworkReply::NetworkError ) ) );
  }
  return reply;
}

/**
* Umleitung zur primären Netzwerk Anfrage @ref createRequest
*/
QNetworkReply* NetworkAccessManager::get ( const QNetworkRequest &req )
{
  return createRequest ( QNetworkAccessManager::GetOperation, req );
}

/**
* Umleitung zur primären Netzwerk Anfrage @ref createRequest
*/
QNetworkReply* NetworkAccessManager::head ( const QNetworkRequest &req )
{
  return createRequest ( QNetworkAccessManager::HeadOperation, req );
}

/**
* Umleitung zur primären Netzwerk Anfrage @ref createRequest
*/
QNetworkReply* NetworkAccessManager::post ( const QNetworkRequest &req, QIODevice* data )
{
  return createRequest ( QNetworkAccessManager::PostOperation, req, data );
}

/**
* Umleitung zur primären Netzwerk Anfrage @ref createRequest
*/
QNetworkReply* NetworkAccessManager::post ( const QNetworkRequest &req, const QByteArray &arr )
{
  QBuffer* data = new QBuffer;
  data->setData ( arr );
  data->open ( QIODevice::ReadOnly );
  QNetworkReply *reply = createRequest ( QNetworkAccessManager::PostOperation, req, data );
  data->setParent ( reply );
  return reply;
}

NetworkAccessManager::~NetworkAccessManager()
{}
