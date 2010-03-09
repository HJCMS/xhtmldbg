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

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

NetworkAccessManager::NetworkAccessManager ( QObject * parent )
    : QNetworkAccessManager ( parent )
{
  m_networkSettings = new  NetworkSettings ( this );

  if ( m_networkSettings->value ( QLatin1String ( "enableProxy" ), false ).toBool() )
    setProxy ( m_networkSettings->getProxy() );

  connect ( this, SIGNAL ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ),
            this, SLOT ( authenticationRequired ( QNetworkReply *, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ),
            this, SLOT ( proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator * ) ) );

  connect ( this, SIGNAL ( finished ( QNetworkReply * ) ),
            this, SLOT ( replyFinished ( QNetworkReply * ) ) );
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

void NetworkAccessManager::replyFinished ( QNetworkReply *reply )
{
  return;
  if ( reply )
  {
    QByteArray retval ( reply->readAll() );
    if ( retval.isEmpty() )
      return;

    QTextCodec *codec = QTextCodec::codecForHtml ( retval, fetchHeaderEncoding ( reply ) );
    QString out = codec->toUnicode ( retval );
    qDebug() << Q_FUNC_INFO << out << reply->url();
  }
}

QNetworkReply* NetworkAccessManager::createRequest ( QNetworkAccessManager::Operation op,
        const QNetworkRequest &req,
        QIODevice *data )
{
  QNetworkRequest request = m_networkSettings->requestOptions ( req );
  return QNetworkAccessManager::createRequest ( op, request, data );
}

NetworkAccessManager::~NetworkAccessManager()
{}
