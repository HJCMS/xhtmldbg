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
#include <QtCore/QChar>
#include <QtCore/QDebug>
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

  inProgress.clear();
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
* Wenn diese nicht Identisch sind wird false zurück gegeben und das
* Signal @ref cookieRejected abgestoßen.
* Verwende hierbei die RFC 2109 vorgaben:
* @li A Set-Cookie from request-host x.foo.com for Domain=.foo.com would be accepted.
* @li A Set-Cookie with Domain=.com or Domain=.com., will always be rejected, because
*     there is no embedded dot.
* @li A Set-Cookie with Domain=ajax.com will be rejected because the value for Domain
*     does not begin with a dot.
*/
bool NetworkCookie::validateDomainAndHost ( const QString &domain, const QUrl &url )
{
  if ( domain.isEmpty() )
    return false;

  QString host1 = cookieDomainFromHost ( url );
  QString host2 = "." + cookieDomainFromHost ( url );
  QString rejectMessage = trUtf8 ( "Impermissible Cookie format for \"%1\" and Cookie Domain \"%2\" rejected by RFC 2109." ).arg ( url.host(), domain );
  rejectMessage.append ( QLatin1String ( " " ) );

  if ( ! domain.contains ( QRegExp ( "^\\." ) ) )
  {
    rejectMessage.append ( trUtf8 ( "A Set-Cookie with Domain=%1 will be rejected because the value for Domain does not begin with a dot." ).arg ( domain ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( domain.contains ( QRegExp ( "\\.$" ) ) )
  {
    rejectMessage.append ( trUtf8 ( "A Set-Cookie with attached dot Domain=%1, will always be rejected." ).arg ( domain ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( domain.count ( QChar ( '.' ) ) < 2 )
  {
    rejectMessage.append ( trUtf8 ( "A Set-Cookie with missing Hostname Domain=.tld, will always be rejected." ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( host1.compare ( domain, Qt::CaseInsensitive ) == 0 )
  {
    // Accepted: .host.tld == .domain.tld
    return true;
  }
  else if ( host2.compare ( domain, Qt::CaseInsensitive ) == 0 )
  {
    // Accepted: host.tld == .domain.tld
    return true;
  }

  emit cookieNotice ( trUtf8 ( "Different Cookie/Domain for host %1. (Rejected)" ).arg ( url.host() ) );
  return false;
}

/**
* Setze die Lebenszeit mit den Einstellungen aus
* "CookieLifeTime" der Konfiguration.
*/
const QDateTime NetworkCookie::cookieLifeTime()
{
  QDateTime now = QDateTime::currentDateTime();
  return now.addDays ( m_netcfg->value ( QLatin1String ( "CookieLifeTime" ), 15 ).toUInt() );
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
#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "(XHTMLDBG) Cookies Saved";
#endif
  inProgress.clear();
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
    return QNetworkCookieJar::cookiesForUrl ( url );
  else
    return empty;
}

/**
* Alles Cookie anfragen werden in dieser Methode Verarbeitet bevor sie
* an @ref QNetworkCookieJar::setCookiesFromUrl gesendet werden.
* 1. Suche zuerst in der Blockliste und steige bei bedarf aus.
* 2. Nachsehen ob es sich um ein erlaubtes Setzen oder eine Session Regelung handelt!
*    Hierfür werden folgende Verhaltens Regelungen festgelegt:
*    @li Ist es kein Session Cookie und wird bei den Einstellungen das Cookie
*     nur als Session akzeptiert dann dieses Cookie modifizieren in dem die
*     Laufzeit dieses Cookies entfernt wird.
*    @li Wenn es sich um ein erlaubtes Cookie handelt dann die Laufzeit neu setzen.
*    @li Überprüfen ob im Cookie der Wert cookie.domain nicht Leer ist.
*     Wenn doch! Dann aus Anfrage Url die Domäne ermitteln und Cookie:Domain setzen.
*    @li Validiere die cookie.domain mit @ref validateDomainAndHost nach RFC 2109 bei
*     fehlern wird dieses Cookie \b NICHT Akzeptiert!
*    @li Überprüfe ob es sich um eine https Verbindung handelt und das Secure Flag gesetzt
*     ist, wenn nicht sende das Signal @ref cookieNotice mit einem Hinweis.
*    @li Jetzt sende an @ref QNetworkCookieJar::setCookiesFromUrl die CookieUrl und Cookies.
*     Bei erfolg sende das Signal @ref cookieChanged und setze den Rückgabewert auf true.
*    @li Bei einem true an @ref AutoSaver die Mitteilung zum speichern geben.
* 3. Wenn keine Cookie-Regelung vorhanden ist, dann das Signal @ref cookieRequest für
*   den WebViewer abstoßen und mit false beenden.
*/
bool NetworkCookie::setCookiesFromUrl ( const QList<QNetworkCookie> &list, const QUrl &url )
{
  // Fehlerhafte Url's erst gar nicht akzeptieren!
  if ( ! url.isValid() )
  {
    emit cookieNotice ( trUtf8 ( "Invalid Url for Cookie Request - rejected!" ) );
    return false;
  }

  bool add = false;
  bool yes = false;
  bool tmp = false;
  bool isInSecure = false;

  QString cookieHost = url.host().remove ( QRegExp ( "\\bwww\\." ) );
  // Wenn dieser Host in der Blockliste steht sofort aussteigen.
  if ( cookiesBlocked.indexOf ( cookieHost ) != -1 )
  {
    emit cookieNotice ( trUtf8 ( "Cookie for Host \"%1\" rejected by blocked list!" ).arg ( cookieHost ) );
    return false;
  }

  // befinded sich ein Teil dieser URL in der Bearbeitung dann aussteigen!
  QString uniqueUrlString = url.toString ( ( QUrl::RemoveFragment | QUrl::RemoveQuery ) );
  if ( inProgress.contains ( uniqueUrlString ) )
    return false;

  // Nachsehen ob dieser Host immer erlaubt oder nur alls Session genehmigt ist.
  yes = ( cookiesAllowed.indexOf ( cookieHost ) != -1 ) ? true : false;
  tmp = ( cookiesSession.indexOf ( cookieHost ) != -1 ) ? true : false;

#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "(XHTMLDBG) Cookie Request - Host:" << cookieHost
  << " Unique:" << uniqueUrlString << " Acceppted:" << yes << " Session Cookie:" << tmp
  << " Full Request:" << url.toString();
#endif

  // Neue Cookie Laufzeit
  QDateTime lifeTime = cookieLifeTime();

  // CookieUrl bereinigen
  QUrl cookieUrl;
  cookieUrl.setScheme ( url.scheme() );
  cookieUrl.setHost ( url.host() );
  cookieUrl.setPath ( url.path() );

  if ( tmp || yes )
  {
    foreach ( QNetworkCookie cookie, list )
    {
      QList<QNetworkCookie> cookies;
      if ( tmp && ! cookie.isSessionCookie() )
        cookie.setExpirationDate ( QDateTime() );
      else if ( ! cookie.isSessionCookie() && ( cookie.expirationDate() > lifeTime ) )
        cookie.setExpirationDate ( lifeTime );

      if ( cookie.domain().isEmpty() )
        cookie.setDomain ( cookieDomainFromHost ( cookieUrl ) );

      if ( ! validateDomainAndHost ( cookie.domain(), cookieUrl ) )
        continue;

      if ( url.scheme().contains ( "https" ) && ! cookie.isSecure() )
        isInSecure = true; // Setze Warnung für diese Anfrage.

      // Cookies von dieser Url zusammen packen
      cookies += cookie;

      // Jetzt die Cookies an QNetworkCookieJar übergeben
      if ( QNetworkCookieJar::setCookiesFromUrl ( cookies, url ) )
      {
        emit cookieChanged ();
        add = true;
      }
    } // end foreach
  }

  if ( isInSecure )
    emit cookieNotice ( trUtf8 ( "Missing Optional Cookie/Secure attribute for HTTPS Scheme" ) );

  inProgress << uniqueUrlString;

  // Wenn neu erzeugt dann später speichern
  if ( add )
    m_autoSaver->saveIfNeccessary();
  else if ( ( ! add ) && ( ! tmp ) && ( ! yes ) )
    emit cookieRequest ( cookieUrl ); // Anfrage an Page Senden

  return add;
}

NetworkCookie::~NetworkCookie()
{
  m_autoSaver->saveIfNeccessary();
  inProgress.clear();
}
