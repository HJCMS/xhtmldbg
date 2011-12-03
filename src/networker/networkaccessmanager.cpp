/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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
#include "networkcache.h"
#include "authenticationdialog.h"
#include "certdialog.h"
#include "errorsdialog.h"
#include "localsource.h"

#include <climits>

/* QtCore */
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QGlobalStatic>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

/* Create Network Error Reply */
class NetworkErrorReply : public QNetworkReply
{
  public:
    NetworkErrorReply ( const QNetworkRequest &req, QObject* parent = 0 )
        : QNetworkReply ( parent )
    {
      setRequest ( req );
      setUrl ( req.url() );
      setHeader ( QNetworkRequest::ContentLengthHeader, 0 );
      setHeader ( QNetworkRequest::ContentTypeHeader, "text/plain" );
      setError ( QNetworkReply::ContentAccessDenied, i18n ( "the operation requested on the remote content is not permitted" ) );
      setAttribute ( QNetworkRequest::User, QNetworkReply::ContentOperationNotPermittedError );
      QTimer::singleShot ( 0, this, SIGNAL ( finished() ) );
    }
    virtual void abort() {}
    virtual qint64 bytesAvailable() const
    {
      return 0;
    }

  protected:
    virtual qint64 readData ( char*, qint64 )
    {
      return -1;
    }
};

NetworkAccessManager::NetworkAccessManager ( QObject * parent )
    : KIO::Integration::AccessManager ( parent )
    , requestUrl ( QUrl ( "http://localhost" ) )
{
  setObjectName ( "NetworkAccessManager" );

  m_networkSettings = new  NetworkSettings ( this );
  m_networkCookie = new NetworkCookie ( m_networkSettings, this );
  m_errorsDialog = new ErrorsDialog;

  sslConfig = m_networkSettings->sslConfiguration();

  if ( m_networkSettings->value ( QLatin1String ( "enableProxy" ), false ).toBool() )
    setProxy ( m_networkSettings->getProxy() );

  trustedCertsHostsList << m_networkSettings->trustedCertsList();

  setCookieJar ( m_networkCookie );

  // Wir benötigen den Plattenspeicher um sicher zu gehen das kein Plugin diesen setzt!
  // @see Settings::webLocalStoragePath()
  m_networkCache = new NetworkCache ( m_networkSettings->webLocalStoragePath(), this );
  // Jetzt den Plattenspeicher setzen
  setCache ( m_networkCache );

  if ( networkAccessible() != QNetworkAccessManager::Accessible )
    setNetworkAccessible ( QNetworkAccessManager::Accessible );

  m_networkReply = 0;

  connect ( this, SIGNAL ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ),
            this, SLOT ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ),
            this, SLOT ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( finished ( QNetworkReply * ) ),
            this, SLOT ( replyFinished ( QNetworkReply * ) ) );

  connect ( this, SIGNAL ( sslErrors ( QNetworkReply *, const QList<QSslError> & ) ),
            this, SLOT ( certErrors ( QNetworkReply *, const QList<QSslError> & ) ) );

  connect ( m_errorsDialog, SIGNAL ( errorMessage ( const QString & ) ),
            this, SIGNAL ( netNotify ( const QString & ) ) );

  connect ( m_networkCache, SIGNAL ( readyRead ( const QUrl & ) ),
            this, SLOT ( cacheReadyRead ( const QUrl & ) ) );
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
    QByteArray cType = reply->header ( QNetworkRequest::ContentTypeHeader ).toByteArray();
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
* Dann, an dieser Stelle einen Dialog anzeigen.
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
* Dann - An dieser Stelle einen Dialog anzeigen.
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
*
* FIXME Wenn die SSL/TLS URL ein fehlerhaften Link sendet oder ein Header
* Moved 303 ausgelöst/gesendet wurde. Ist das QNetworkReply ungültig!
*/
void NetworkAccessManager::certErrors ( QNetworkReply * reply, const QList<QSslError> &errors )
{
  QString certHost ( reply->url().host() );

  if ( trustedCertsHostsList.isEmpty() )
    trustedCertsHostsList << m_networkSettings->trustedCertsList();

  if ( trustedCertsHostsList.contains ( certHost )
          && ( reply->error() == QNetworkReply::SslHandshakeFailedError ) )
  {
    reply->ignoreSslErrors();
    return;
  }
  else
  {
    // FIXME Zurückhalte Speicher wegen mehrfach anfragen!
    if ( pendingCerts.contains ( certHost ) )
      return;

    pendingCerts.append ( certHost );

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
      trustedCertsHostsList.append ( certHost );
      pendingCerts.clear();
      if ( ( reply->error() == QNetworkReply::NoError ) ||
              ( reply->error() == QNetworkReply::SslHandshakeFailedError ) )
        reply->ignoreSslErrors();
      else
        emit netNotify ( i18n ( "Broken SSL/TLS URL connection, handshake failed and could not be established." ) );
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

  if ( m_errorsDialog->setError ( err, requestUrl ) )
  {
    if ( ! m_errorsDialog->isVisible() )
      m_errorsDialog->show();
  }
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
* Weil Lokale Dateien nicht mit @ref peekReplyProcess gelesen werden
* könne hier eine extra Routine zum lesen von Daten Quelltext für die
* Quelltextansicht, siehe auch @ref createRequest
*/
void NetworkAccessManager::openLocalFile ( const QUrl &url )
{
  if ( url.path().isEmpty() )
    return;

  LocalSource src = LocalSource::localSource ( url );
  if ( ! src.source().isEmpty() )
    emit localReplySource ( url, src.source() );
}

/**
* Eine Cache Anforderung wurde mit @ref triggerCache Angefordert und
* mit @ref NetworkCache::readyRead bestätigt.
*/
void NetworkAccessManager::cacheReadyRead ( const QUrl &url )
{
  if ( url.path().isEmpty() )
    return;

  QIODevice* dev = cache()->data ( url );
  if ( dev )
  {
    QByteArray dc = dev->readAll();
    QTextCodec* codec = QTextCodec::codecForLocale ();
    emit localReplySource ( url, codec->toUnicode ( dc ) );
    dev->deleteLater();
  }
}

/**
* An dieser Stelle werden die POST Antworten abgefangen.
* Weil QWebKit keinen Originalen Quelltext zurück gibt muss dies
* an dieser erfolgen. Dabei ist es wichtig das die Daten vom Device
* abgegriffen und nicht ausgelesen werden. Weil sonst die IO Device
* den Speicher freigeben würde und nichts mehr beim WebKit ankommt.
* Siehe auch @ref peekDeviceData
*/
void NetworkAccessManager::peekReplyProcess()
{
  /**
  * FIXME 2011/01/27 Crash with invalid page Size
  * Wenn eine Seitengröße sehr groß ist produziert WebKit
  * zwichendurch einen unzulässigen leeren Header,
  * das wird jetzt an dieser Stelle abgefangen. */
  if ( ( ! m_networkReply ) || ( m_networkReply->size() < 1 ) )
    return;

  QVariant contentTypeHeader = m_networkReply->header ( QNetworkRequest::ContentTypeHeader );
  if ( ! contentTypeHeader.isValid() )
    return;

  QString mimeType = contentTypeHeader.toString();
  if ( ! mimeType.contains ( "text/html" ) )
    return;

  if ( m_networkReply->hasRawHeader ( QByteArray ( "location" ) ) )
  {
    emit netNotify ( i18n ( "Multiple Content-Location header from POST Request received. Note - Many Webservers are free to ignore this. (300)" ) );
    peekPostData.clear();
  }

  QIODevice* copyDevice = m_networkReply;
  if ( ! copyDevice || ! m_networkReply->isOpen() )
    return;

  peekPostData.append ( peekDeviceData ( copyDevice ) );

  // FIXME I know this is a very difficult hack :-/
  if ( peekPostData.contains ( QByteArray ( "</html>" ) ) )
  {
    QTextCodec* codec = QTextCodec::codecForHtml ( peekPostData, fetchHeaderEncoding ( m_networkReply ) );
    // qDebug() << codec->toUnicode ( peekPostData );
    emit postReplySource ( m_networkReply->url(), codec->toUnicode ( peekPostData ) );
  }
}

/**
* Wenn der Benutzer ein POST Formular absendet.
* Werden die Header/Post Daten an dieser Stelle abgefangen.
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
    if ( reply->url() == requestUrl )
      emit urlLoadFinished ( reply->url() );
  }
}

/**
* Setzt die Virtuelle @ref url Variable
* Wird von Page verwendet um den Zeiger auf den Cache zu definieren.
* @see Siehe auch replyFinished
*/
void NetworkAccessManager::setUrl ( const QUrl &u )
{
  requestUrl = u;
}

/**
* Gibt die Variable @ref url der letzten Anfrage zurück.
*/
const QUrl NetworkAccessManager::getUrl()
{
  return requestUrl;
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
  // Keine Veränderung vornehmen wenn das Standard Speicherverhalten deaktiviert ist!
  if ( m_networkSettings->defaultCachingBehavior() )
  {
    // Damit die Validierung funktioniert muss der cache immer Leer sein!
    cache()->remove ( req.url() );

    // Bei einer POST Operation machen wir alles LEER!
    if ( op == QNetworkAccessManager::PostOperation )
      cache()->clear();
  }

  // Lokale Dateien werden nicht von Page::acceptNavigationRequest behandelt!
  // Deshalb sende die Daten für die Quelltextansicht seperat.
  if ( req.url().scheme().contains ( "file" ) || req.url().isRelative() )
    openLocalFile ( req.url() );

  // FTP wird im Moment nicht unterstützt
  if ( req.url().scheme().contains ( "ftp" ) )
  {
    replyErrors ( QNetworkReply::ProtocolUnknownError );
    return new NetworkErrorReply ( req, this );
  }

  QNetworkRequest request = m_networkSettings->requestOptions ( req );
  m_networkReply = QNetworkAccessManager::createRequest ( op, request, data );
  if ( ! m_networkReply )
    return new NetworkErrorReply ( req, this );

  m_networkReply->setSslConfiguration ( sslConfig );
  connect ( m_networkReply, SIGNAL ( error ( QNetworkReply::NetworkError ) ),
            this, SLOT ( replyErrors ( QNetworkReply::NetworkError ) ) );

  // POST wird nicht von Page::acceptNavigationRequest behandelt!
  if ( m_networkReply && op == QNetworkAccessManager::PostOperation )
  {
    peekPostData.clear();
    fetchPostedData ( m_networkReply->request(), data );
    connect ( m_networkReply, SIGNAL ( readyRead() ), this, SLOT ( peekReplyProcess() ) );
  }

  return m_networkReply;
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
  QNetworkReply* reply = createRequest ( QNetworkAccessManager::PostOperation, req, data );
  data->setParent ( reply );
  return reply;
}

/**
* Startet eine Cache Anfrage für diese URL
*/
void NetworkAccessManager::triggerCache ( const QUrl &url )
{
  m_networkCache->initCache ( url );
}

NetworkAccessManager::~NetworkAccessManager()
{
  cache()->clear();
  pendingCerts.clear();
  m_networkCookie->deleteLater();
  m_errorsDialog->deleteLater();
}


