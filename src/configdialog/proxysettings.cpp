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

#include "proxysettings.h"

/* QtCore */
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

ProxySettings::ProxySettings ( QWidget * parent )
    : QWidget ( parent )
    , HostName ( QString::null )
    , User ( QString::null )
    , Password ( QString::null )
    , Port ( 3128 )
{
  setObjectName ( QLatin1String ( "proxysettings" ) );

  Qt::Alignment defalignment = ( Qt::AlignRight | Qt::AlignVCenter );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  enableProxy = new QGroupBox ( this );
  enableProxy->setObjectName ( QLatin1String ( "enableProxy" ) );
  enableProxy->setTitle ( trUtf8 ( "Configuring Network Proxy" ) );
  enableProxy->setCheckable ( true );
  enableProxy->setChecked ( false );

  QGridLayout* gLayout = new QGridLayout ( enableProxy );

  // Proxy Type
  QLabel* txtProxyType = new QLabel ( trUtf8 ( "The Network Proxy Type:" ), this );
  txtProxyType->setAlignment ( defalignment );
  txtProxyType->setIndent ( 2 );
  txtProxyType->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyType->setWordWrap ( false );
  gLayout->addWidget ( txtProxyType, 0, 0 );
  // QNetworkProxy::DefaultProxy QNetworkProxy::NoProxy not used
  proxyType = new QComboBox ( this );
  proxyType->setObjectName ( QLatin1String ( "proxyType" ) );
  proxyType->insertItem ( 0, trUtf8 ( "Socks 5 (Based on RFC 1928 and RFC 1929)" ), QNetworkProxy::Socks5Proxy );
  proxyType->setItemData ( 0, QNetworkProxy::Socks5Proxy, Qt::UserRole );
  proxyType->insertItem ( 1, trUtf8 ( "HTTP Transparent Proxying (Default)" ), QNetworkProxy::HttpProxy );
  proxyType->setItemData ( 1, QNetworkProxy::HttpProxy, Qt::UserRole );
  proxyType->insertItem ( 2, trUtf8 ( "Caching-only HTTP (Only for HTTP Requests)" ), QNetworkProxy::HttpCachingProxy );
  proxyType->setItemData ( 2, QNetworkProxy::HttpCachingProxy, Qt::UserRole );
  proxyType->insertItem ( 3, trUtf8 ( "Caching FTP (Only for FTP Requests)" ), QNetworkProxy::FtpCachingProxy );
  proxyType->setItemData ( 3, QNetworkProxy::FtpCachingProxy, Qt::UserRole );
  proxyType->setCurrentIndex ( 1 );
  gLayout->addWidget ( proxyType, 0, 1 );

  // Proxy Hostname
  QLabel* txtProxyHost = new QLabel ( trUtf8 ( "The Hostname of the Proxy:" ), this );
  txtProxyHost->setAlignment ( defalignment );
  txtProxyHost->setIndent ( 2 );
  txtProxyHost->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyHost->setWordWrap ( false );
  gLayout->addWidget ( txtProxyHost, 1, 0 );

  proxyHostName = new QLineEdit ( this );
  proxyHostName->setObjectName ( QLatin1String ( "proxyHostName" ) );
  proxyHostName->setText ( HostName );
  gLayout->addWidget ( proxyHostName, 1, 1 );

  // Proxy Port
  QLabel* txtProxyPort = new QLabel ( trUtf8 ( "The Proxy Port Number:" ), this );
  txtProxyPort->setAlignment ( defalignment );
  txtProxyPort->setIndent ( 2 );
  txtProxyPort->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyPort->setWordWrap ( false );
  gLayout->addWidget ( txtProxyPort, 2, 0 );

  proxyPort = new QSpinBox ( this );
  proxyPort->setObjectName ( QLatin1String ( "proxyPort" ) );
  proxyPort->setMinimum ( 80 );
  proxyPort->setMaximum ( 655351 );
  proxyPort->setValue ( Port );
  gLayout->addWidget ( proxyPort, 2, 1 );

  // Proxy User
  QLabel* txtProxyUser = new QLabel ( trUtf8 ( "The Authentication username:" ), this );
  txtProxyUser->setAlignment ( defalignment );
  txtProxyUser->setIndent ( 2 );
  txtProxyUser->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyUser->setWordWrap ( false );
  gLayout->addWidget ( txtProxyUser, 3, 0 );

  proxyUser = new QLineEdit ( this );
  proxyUser->setObjectName ( QLatin1String ( "proxyUser" ) );
  proxyUser->setText ( User );
  gLayout->addWidget ( proxyUser, 3, 1 );

  // Proxy Password
  QLabel* txtProxyPassword = new QLabel ( trUtf8 ( "The Authentication password:" ), this );
  txtProxyPassword->setAlignment ( defalignment );
  txtProxyPassword->setIndent ( 2 );
  txtProxyPassword->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyPassword->setWordWrap ( false );
  gLayout->addWidget ( txtProxyPassword, 4, 0 );

  proxyPassword = new QLineEdit ( this );
  proxyPassword->setObjectName ( QLatin1String ( "qt_proxy_password" ) );
  proxyPassword->setEchoMode ( QLineEdit::Password );
  proxyPassword->setText ( Password );
  gLayout->addWidget ( proxyPassword, 4, 1 );

  // add Layouts
  enableProxy->setLayout ( gLayout );
  vLayout->addWidget ( enableProxy );
  vLayout->addStretch ();
  vLayout->addWidget ( new QLabel ( trUtf8( "Application restart required." ), this ) );

  setLayout ( vLayout );

  connect ( enableProxy, SIGNAL ( toggled ( bool ) ), this, SIGNAL ( modified() ) );
  connect ( proxyType, SIGNAL ( currentIndexChanged ( int ) ), this, SIGNAL ( modified() ) );
  connect ( proxyHostName, SIGNAL ( editingFinished() ), this, SIGNAL ( modified() ) );
  connect ( proxyPort, SIGNAL ( valueChanged ( int ) ), this, SIGNAL ( modified() ) );
  connect ( proxyUser, SIGNAL ( editingFinished() ), this, SIGNAL ( modified() ) );
  connect ( proxyPassword, SIGNAL ( editingFinished() ), this, SIGNAL ( modified() ) );
}

void ProxySettings::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "proxyType" ), getType() );
  cfg->setValue ( QLatin1String ( "enableProxy" ), enableProxy->isChecked() );
  QByteArray p = proxyPassword->text().toAscii();
  cfg->setValue ( QLatin1String ( "proxyPassword" ), p.toBase64() );
}

void ProxySettings::load ( QSettings * cfg )
{
  int index = proxyType->findData ( cfg->value ( QLatin1String ( "proxyType" ), QNetworkProxy::NoProxy ), Qt::UserRole );
  if ( index >= 0 )
    proxyType->setCurrentIndex ( index );

  QString p ( QByteArray::fromBase64 ( cfg->value ( QLatin1String ( "proxyPassword" ) ).toByteArray() ) );
  proxyPassword->setText ( p );

  enableProxy->setChecked ( cfg->value ( QLatin1String ( "enableProxy" ), false ).toBool() );
}

void ProxySettings::setType ( QNetworkProxy::ProxyType type )
{
  if ( type == QNetworkProxy::NoProxy )
    return;

  switch ( type )
  {
    case QNetworkProxy::Socks5Proxy:
      proxyType->setCurrentIndex ( 0 );
      break;

    case QNetworkProxy::HttpProxy:
      proxyType->setCurrentIndex ( 1 );
      break;

    case QNetworkProxy::HttpCachingProxy:
      proxyType->setCurrentIndex ( 2 );
      break;

    case QNetworkProxy::FtpCachingProxy:
      proxyType->setCurrentIndex ( 3 );
      break;

    default:
      proxyType->setCurrentIndex ( 0 );
      break;
  }
}

void ProxySettings::setHostName ( const QString &str )
{
  proxyHostName->setText ( str );
}

void ProxySettings::setPort ( int p )
{
  proxyPort->setValue ( p );
}

void ProxySettings::setUser ( const QString &str )
{
  proxyUser->setText ( str );
}

void ProxySettings::setPassword ( const QString &str )
{
  proxyPassword->setText ( str );
}

QNetworkProxy::ProxyType ProxySettings::getType ()
{
  if ( ! enableProxy->isChecked () )
    return QNetworkProxy::NoProxy;

  QNetworkProxy::ProxyType type;
  switch ( proxyType->currentIndex () )
  {
    case 0:
      type = QNetworkProxy::Socks5Proxy;
      break;

    case 1:
      type = QNetworkProxy::HttpProxy;
      break;

    case 2:
      type = QNetworkProxy::HttpCachingProxy;
      break;

    case 3:
      type = QNetworkProxy::FtpCachingProxy;
      break;

    default:
      type = QNetworkProxy::NoProxy;
      break;
  }
  return type;
}

const QString ProxySettings::getHostName ()
{
  return proxyHostName->text();
}

int ProxySettings::getPort ()
{
  return proxyPort->value ();
}

const QString ProxySettings::getUser ()
{
  return proxyUser->text();
}

const QString ProxySettings::getPassword ()
{
  return proxyPassword->text();
}

ProxySettings::~ProxySettings()
{}
