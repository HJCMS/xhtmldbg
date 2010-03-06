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

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

/*
* @begin CODE FROM Nokia Corporation (qt-info@nokia.com)
*/
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

/*
* @end CODE FROM Nokia Corporation (qt-info@nokia.com)
*/

NetworkCookie::NetworkCookie ( QObject * parent )
    : QNetworkCookieJar ( parent )
    , m_netcfg ( 0 )
    , cookiesBlocked ( 0 )
    , cookiesAllowed ( 0 )
    , cookiesSession ( 0 )
{
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
  QSettings qs ( storedir + QLatin1String ( "/cookie.jar" ), QSettings::IniFormat );
  setAllCookies ( qvariant_cast<QList<QNetworkCookie> > ( qs.value ( QLatin1String ( "cookies" ) ) ) );
}

void NetworkCookie::save()
{
  QString storedir = m_netcfg->storageDirectory();
  if ( storedir.isEmpty() )
    return;

  QSettings qs ( storedir + QLatin1String ( "/cookie.jar" ), QSettings::IniFormat );
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
    return add;

  yes = ( cookiesAllowed.indexOf ( h ) != -1 ) ? true : false;
  tmp = ( cookiesSession.indexOf ( h ) != -1 ) ? true : false;

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

  emit cookiesAdd( add );
  return add;
}

NetworkCookie::~NetworkCookie()
{
  save();
}
