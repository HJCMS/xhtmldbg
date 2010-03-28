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

#include "networkcookie.h"
#include "networksettings.h"
#include "autosaver.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

/****************************************************************************
** @begin Codes from demonstration applications of the Qt Toolkit.
*****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This Code is part of the demonstration applications of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
*****************************************************************************/

static const unsigned int JAR_VERSION = 23;

QDataStream &operator<< ( QDataStream &stream, const QList<QNetworkCookie> &list )
{
  stream << JAR_VERSION;
  stream << quint32 ( list.size() );
  for ( int i = 0; i < list.size(); ++i )
    stream << list.at ( i ).toRawForm();
  return stream;
}

QDataStream &operator>> ( QDataStream &stream, QList<QNetworkCookie> &list )
{
  list.clear();

  quint32 version;
  stream >> version;

  if ( version != JAR_VERSION )
    return stream;

  quint32 count;
  stream >> count;
  for ( quint32 i = 0; i < count; ++i )
  {
    QByteArray value;
    stream >> value;
    QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies ( value );
    if ( newCookies.count() == 0 && value.length() != 0 )
    {
      qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
    }
    for ( int j = 0; j < newCookies.count(); ++j )
      list.append ( newCookies.at ( j ) );
    if ( stream.atEnd() )
      break;
  }
  return stream;
}

/****************************************************************************
* @end Codes from demonstration applications of the Qt Toolkit.
****************************************************************************/

NetworkCookie::NetworkCookie ( NetworkSettings * settings, QObject * parent )
    : QNetworkCookieJar ( parent )
    , m_netcfg ( settings )
    , m_autoSaver ( new AutoSaver ( this ) )
    , cookiesBlocked ( 0 )
    , cookiesAllowed ( 0 )
    , cookiesSession ( 0 )
{
  if ( ! m_netcfg )
    m_netcfg = new NetworkSettings ( this );

  load();
}

void NetworkCookie::load()
{
  m_netcfg->beginGroup ( QLatin1String ( "CookieArrangement" ) );
  QStringList keys = m_netcfg->allKeys();
  if ( keys.size() >= 1 )
  {
    foreach ( QString key, keys )
    {
      switch ( m_netcfg->value ( key ).toUInt() )
      {
        case Session:
          cookiesSession << key;
          break;

        case Allowed:
          cookiesAllowed << key;
          break;

        default:
          cookiesBlocked << key;
          break;
      };
    }
  }
  m_netcfg->endGroup();

  QString storedir = m_netcfg->storageDirectory();
  if ( storedir.isEmpty() )
    return;

  qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> > ( "QList<QNetworkCookie>" );
  QSettings qs ( storedir + QLatin1String ( "/cookiejar.ini" ), QSettings::IniFormat );
  setAllCookies ( qvariant_cast<QList<QNetworkCookie> > ( qs.value ( QLatin1String ( "cookies" ) ) ) );
}

void NetworkCookie::reload()
{
  cookiesSession.clear();
  cookiesAllowed.clear();
  cookiesBlocked.clear();
  load();
}

void NetworkCookie::save()
{
  QString storedir = m_netcfg->storageDirectory();
  if ( storedir.isEmpty() )
    return;

  QSettings qs ( storedir + QLatin1String ( "/cookiejar.ini" ), QSettings::IniFormat );
  QList<QNetworkCookie> cookies = allCookies();
  for ( int i = cookies.count() - 1; i >= 0; --i )
  {
    if ( cookies.at ( i ).isSessionCookie() )
      cookies.removeAt ( i );
  }
  qs.setValue ( QLatin1String ( "cookies" ), qVariantFromValue<QList<QNetworkCookie> > ( cookies ) );
}

QList<QNetworkCookie> NetworkCookie::cookiesForUrl ( const QUrl &url ) const
{
  QList<QNetworkCookie> empty;
  QUrl host;
  host.setScheme ( QLatin1String ( "http" ) );
  host.setHost ( url.host().remove ( QRegExp ( "\\bwww\\." ) ) );
  if ( host.isValid() )
    return QNetworkCookieJar::cookiesForUrl ( host );
  else
    return empty;
}

bool NetworkCookie::setCookiesFromUrl ( const QList<QNetworkCookie> &list, const QUrl &url )
{
  bool add = false;
  bool yes = false;
  bool tmp = false;
  QString h = url.host().remove ( QRegExp ( "\\bwww\\." ) );

  if ( cookiesBlocked.indexOf ( h ) != -1 )
    return false;

  yes = ( cookiesAllowed.indexOf ( h ) != -1 ) ? true : false;
  tmp = ( cookiesSession.indexOf ( h ) != -1 ) ? true : false;

  if ( tmp || yes )
  {
    QDateTime now = QDateTime::currentDateTime();
    now = now.addDays ( m_netcfg->value ( QLatin1String ( "CookieLifeTime" ), 15 ).toUInt() );

    foreach ( QNetworkCookie cookie, list )
    {
      QList<QNetworkCookie> cookies;
      if ( !cookie.isSessionCookie() && cookie.expirationDate() > now )
        cookie.setExpirationDate ( now );

      cookies += cookie;

      if ( QNetworkCookieJar::setCookiesFromUrl ( cookies, url ) )
        add = true;
    }
  }

  if ( add )
  {
    m_autoSaver->saveIfNeccessary();
    emit cookiesChanged ();
  }
  else
    emit cookiesRequest ( url );

  return add;
}

NetworkCookie::~NetworkCookie()
{
  m_autoSaver->saveIfNeccessary();
}
