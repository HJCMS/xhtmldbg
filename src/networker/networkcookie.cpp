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

#include "networkcookie.h"
#include "networksettings.h"
#include "autosaver.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
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

  QFileInfo cookie_ini ( m_netcfg->storageDirectory() + QLatin1String ( "/cookiejar.ini" ) );
  iniFile = cookie_ini.absoluteFilePath();
  load();
}

/**
* Entfernt cName(www) vom Hostnamen und gibt die Domäne
* mit führenden Punkt zurück.
*/
const QString NetworkCookie::cookieDomainFromHost ( const QUrl &url ) const
{
  return url.host().replace ( QRegExp ( "^www\\." ), "." );
}

/**
* Vergleiche die im Keks enthaltene Domäne mit dem Hostnamen der Url
* Wenn diese nicht Identisch sind wird false zurück gegeben!
*/
bool NetworkCookie::compareDomainAndHost ( const QString &domain, const QUrl &url )
{
  QString host = cookieDomainFromHost ( url );
  if ( host.compare ( domain, Qt::CaseInsensitive ) == 0 )
    return true;
  else if ( host.compare ( "." + domain, Qt::CaseInsensitive ) == 0 )
    return true;
  else
    return false;
}

/**
* Gespeicherte Cookies laden und @ref AutoSaver starten.
*/
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

  qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> > ( "QList<QNetworkCookie>" );
  QSettings qs ( iniFile, QSettings::IniFormat );
  setAllCookies ( qvariant_cast<QList<QNetworkCookie> > ( qs.value ( QLatin1String ( "cookies" ) ) ) );
  m_autoSaver->changeOccurred();
}

/**
* Alle listen Neu laden.
*/
void NetworkCookie::reload()
{
  cookiesSession.clear();
  cookiesAllowed.clear();
  cookiesBlocked.clear();
  load();
}

/**
* Aktuelle Cookies, bis auf die Session Cookies speichern!
*/
void NetworkCookie::save()
{
  QSettings qs ( iniFile, QSettings::IniFormat );
  QList<QNetworkCookie> cookies = allCookies();
  for ( int i = cookies.count() - 1; i >= 0; --i )
  {
    if ( cookies.at ( i ).isSessionCookie() )
      cookies.removeAt ( i );
  }
  qs.setValue ( QLatin1String ( "cookies" ), qVariantFromValue<QList<QNetworkCookie> > ( cookies ) );
}

/**
* Nehme alle Cookies von dieser URL
* Dabei wird eine Neue Url mit Scheme/Host/Path erstellt!
* Der Rest geht in die Mülltonne!
*/
QList<QNetworkCookie> NetworkCookie::cookiesForUrl ( const QUrl &url ) const
{
  QUrl host;
  QList<QNetworkCookie> empty;

  host.setScheme ( url.scheme() );
  host.setHost ( url.host() );
  host.setPath ( url.path() );

  if ( host.isValid() )
    return QNetworkCookieJar::cookiesForUrl ( host );
  else
    return empty;
}

/**
* Anfrage zum setzen eines Cookies.
* @li Sucht zuerst in der Blockliste und bricht bei bedarf ab.
* @li Nachsehen ob es sich um ein erlaubtes Setzen oder eine Session Regelunug handelt!
* @li 
*/
bool NetworkCookie::setCookiesFromUrl ( const QList<QNetworkCookie> &list, const QUrl &url )
{
  bool add = false;
  bool yes = false;
  bool tmp = false;
  QUrl cookieUrl ( url.toString ( QUrl::RemoveQuery | QUrl::RemoveFragment ) );
  QString cookieHost = cookieUrl.host().remove ( QRegExp ( "\\bwww\\." ) );

  // Wenn dieser Host in der Blockliste steht sofort aussteigen.
  if ( cookiesBlocked.indexOf ( cookieHost ) != -1 )
    return false;

  // Nachsehen ob dieser Host immer erlaubt oder nur alls Session genehmigt ist.
  yes = ( cookiesAllowed.indexOf ( cookieHost ) != -1 ) ? true : false;
  tmp = ( cookiesSession.indexOf ( cookieHost ) != -1 ) ? true : false;

  if ( tmp || yes )
  {
    QDateTime now = QDateTime::currentDateTime();
    now = now.addDays ( m_netcfg->value ( QLatin1String ( "CookieLifeTime" ), 15 ).toUInt() );

    foreach ( QNetworkCookie cookie, list )
    {
      QList<QNetworkCookie> cookies;
      // Ist es ein Session Cookie und liegt die
      // Laufzeit höher als jetzt dann auf jetzt setzen!
      if ( ! cookie.isSessionCookie() && cookie.expirationDate() > now )
        cookie.setExpirationDate ( now );

      if ( ! compareDomainAndHost ( cookie.domain(), url ) && cookie.domain().isEmpty() )
        cookie.setDomain ( cookieDomainFromHost ( url ) );

      // Cookies von dieser Url zusammen packen
      cookies += cookie;

      // Jetzt die Cookies an QNetworkCookieJar übergeben
      if ( QNetworkCookieJar::setCookiesFromUrl ( cookies, url ) )
      {
        emit cookiesChanged ();
        add = true;
      }

// #ifdef XHTMLDBG_DEBUG_VERBOSE
//       qDebug() << "(XHTMLDBG) Cookie Accepted:" << add << cookie.domain();
// #endif

    } // end foreach
  }

  // Wenn neu erzeugt dann später speichern
  if ( add )
    m_autoSaver->saveIfNeccessary();
  else if ( ( ! add ) && ( ! tmp ) )
    emit cookiesRequest ( url ); // Anfrage an Page Senden

  return add;
}

NetworkCookie::~NetworkCookie()
{
  m_autoSaver->saveIfNeccessary();
}
