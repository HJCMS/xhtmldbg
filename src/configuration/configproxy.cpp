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

#include "configproxy.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

ConfigProxy::ConfigProxy ( QWidget * parent )
    : PageWidget ( trUtf8 ( "Proxy" ), parent )
    , HostName ( QString::null )
    , User ( QString::null )
    , Password ( QString::null )
    , Port ( 3128 )
{
  setObjectName ( QLatin1String ( "config_page_proxy" ) );
  setNotice ( false );
  setCheckable ( true );
  setChecked ( false );
  Qt::Alignment defalignment = ( Qt::AlignRight | Qt::AlignVCenter );

  QGridLayout* gridLayout = new QGridLayout ( centralWidget );
  gridLayout->setObjectName ( QLatin1String ( "config_page_proxy_layout" ) );

  // Proxy Type
  QLabel* txtProxyType = new QLabel ( trUtf8 ( "The Network Proxy Type:" ), centralWidget );
  txtProxyType->setAlignment ( defalignment );
  txtProxyType->setIndent ( 2 );
  txtProxyType->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyType->setWordWrap ( false );
  gridLayout->addWidget ( txtProxyType, 0, 0, 1, 1 );

  // QNetworkProxy::DefaultProxy QNetworkProxy::NoProxy not used
  proxyType = new QComboBox ( centralWidget );
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
  gridLayout->addWidget ( proxyType, 0, 1, 1, 1 );

  // Proxy Hostname
  QLabel* txtProxyHost = new QLabel ( trUtf8 ( "Hostname of the Proxy:" ), centralWidget );
  txtProxyHost->setAlignment ( defalignment );
  txtProxyHost->setIndent ( 2 );
  txtProxyHost->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyHost->setWordWrap ( false );
  gridLayout->addWidget ( txtProxyHost, 1, 0, 1, 1 );

  proxyHostName = new QLineEdit ( centralWidget );
  proxyHostName->setObjectName ( QLatin1String ( "proxyHostName" ) );
  proxyHostName->setText ( HostName );
  gridLayout->addWidget ( proxyHostName, 1, 1, 1, 1 );

  // Proxy Port
  QLabel* txtProxyPort = new QLabel ( trUtf8 ( "Proxy Port Number:" ), centralWidget );
  txtProxyPort->setAlignment ( defalignment );
  txtProxyPort->setIndent ( 2 );
  txtProxyPort->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyPort->setWordWrap ( false );
  gridLayout->addWidget ( txtProxyPort, 2, 0, 1, 1 );

  proxyPort = new QSpinBox ( centralWidget );
  proxyPort->setObjectName ( QLatin1String ( "proxyPort" ) );
  proxyPort->setMinimum ( 80 );
  proxyPort->setMaximum ( 655351 );
  proxyPort->setValue ( Port );
  gridLayout->addWidget ( proxyPort, 2, 1, 1, 1 );

  // Proxy User
  QLabel* txtProxyUser = new QLabel ( trUtf8 ( "Authentication Username:" ), centralWidget );
  txtProxyUser->setAlignment ( defalignment );
  txtProxyUser->setIndent ( 2 );
  txtProxyUser->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyUser->setWordWrap ( false );
  gridLayout->addWidget ( txtProxyUser, 3, 0, 1, 1 );

  proxyUser = new QLineEdit ( centralWidget );
  proxyUser->setObjectName ( QLatin1String ( "proxyUser" ) );
  proxyUser->setText ( User );
  gridLayout->addWidget ( proxyUser, 3, 1, 1, 1 );

  // Proxy Password
  QLabel* txtProxyPassword = new QLabel ( trUtf8 ( "Authentication Password:" ), centralWidget );
  txtProxyPassword->setAlignment ( defalignment );
  txtProxyPassword->setIndent ( 2 );
  txtProxyPassword->setTextInteractionFlags ( Qt::NoTextInteraction );
  txtProxyPassword->setWordWrap ( false );
  gridLayout->addWidget ( txtProxyPassword, 4, 0, 1, 1 );

  proxyPassword = new QLineEdit ( centralWidget );
  proxyPassword->setObjectName ( QLatin1String ( "qt_proxy_password" ) );
  proxyPassword->setEchoMode ( QLineEdit::Password );
  proxyPassword->setText ( Password );
  gridLayout->addWidget ( proxyPassword, 4, 1, 1, 1 );

  centralWidget->setLayout ( gridLayout );

  connect ( proxyType, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( itemClicked () ) );

  connect ( proxyHostName, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( itemClicked() ) );

  connect ( proxyPort, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( itemClicked () ) );

  connect ( proxyUser, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( itemClicked() ) );

  connect ( proxyPassword, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( itemClicked() ) );
}

void ConfigProxy::itemClicked ( const QVariant & )
{
  emit modified ( true );
}

void ConfigProxy::setType ( QNetworkProxy::ProxyType type )
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

void ConfigProxy::load ( QSettings * cfg )
{
  int index = proxyType->findData ( cfg->value ( QLatin1String ( "proxyType" ), QNetworkProxy::NoProxy ), Qt::UserRole );
  if ( index >= 0 )
    proxyType->setCurrentIndex ( index );

  proxyHostName->setText ( cfg->value ( QLatin1String ( "proxyHostName" ) ).toString() );
  proxyUser->setText ( cfg->value ( QLatin1String ( "proxyUser" ) ).toString() );

  QString p ( QByteArray::fromBase64 ( cfg->value ( QLatin1String ( "proxyPassword" ) ).toByteArray() ) );
  proxyPassword->setText ( p );

  setChecked ( cfg->value ( QLatin1String ( "enableProxy" ), false ).toBool() );
}

void ConfigProxy::save ( QSettings * cfg )
{
  cfg->setValue ( QLatin1String ( "proxyType" ), getType() );
  cfg->setValue ( QLatin1String ( "enableProxy" ), isChecked() );
  cfg->setValue ( QLatin1String ( "proxyHostName" ), proxyHostName->text() );
  cfg->setValue ( QLatin1String ( "proxyUser" ), proxyUser->text() );
  QByteArray p = proxyPassword->text().toAscii();
  cfg->setValue ( QLatin1String ( "proxyPassword" ), p.toBase64() );
}

void ConfigProxy::defaults()
{
  setChecked ( false );
  proxyHostName->clear();
  proxyUser->clear();
  proxyPassword->clear();
}

void ConfigProxy::setHostName ( const QString &str )
{
  proxyHostName->setText ( str );
}

void ConfigProxy::setPort ( int p )
{
  proxyPort->setValue ( p );
}

void ConfigProxy::setUser ( const QString &str )
{
  proxyUser->setText ( str );
}

void ConfigProxy::setPassword ( const QString &str )
{
  proxyPassword->setText ( str );
}

QNetworkProxy::ProxyType ConfigProxy::getType ()
{
  if ( ! isChecked() )
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

const QString ConfigProxy::getHostName ()
{
  return proxyHostName->text();
}

int ConfigProxy::getPort ()
{
  return proxyPort->value ();
}

const QString ConfigProxy::getUser ()
{
  return proxyUser->text();
}

const QString ConfigProxy::getPassword ()
{
  return proxyPassword->text();
}

ConfigProxy::~ConfigProxy()
{}
