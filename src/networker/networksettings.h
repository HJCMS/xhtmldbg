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

#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslConfiguration>

/* xhtmldbg */
#include "settings.h"

class NetworkSettings : public Settings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QWebSettings* wcfg;
    const QByteArray userAgentString() const;
    QNetworkProxy::ProxyType proxyType();

  public:
    NetworkSettings ( QObject * parent = 0 );
    const QNetworkRequest requestOptions ( const QNetworkRequest & );
    const QString storageDirectory();
    const QNetworkProxy getProxy();
    const QSslConfiguration sslConfiguration();
    const QList<QString> trustedCertsList();
    ~NetworkSettings();
};

#endif
