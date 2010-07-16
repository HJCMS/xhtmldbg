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
#include "cookiesdatabase.h"

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

NetworkCookie::NetworkCookie ( NetworkSettings * settings, QObject * parent )
    : QNetworkCookieJar ( parent )
    , m_netcfg ( settings )
    , m_autoSaver ( new AutoSaver ( this ) )
    , m_cookiesDatabase ( new CookiesDatabase ( this ) )
    , cookiesBlocked ( 0 )
    , cookiesAllowed ( 0 )
    , cookiesSession ( 0 )
{
  if ( ! m_netcfg )
    m_netcfg = new NetworkSettings ( this );

  inProgress.clear();
  load();
}

/**
* Entfernt cName(www) vom Hostnamen und gibt die Domäne
* ohne führenden Punkt zurück.
*/
const QString NetworkCookie::cookieHostnameFromUrl ( const QUrl &url ) const
{
  QString domain = url.host().replace ( QRegExp ( "^www\\." ), "" );
  if ( domain.contains ( QRegExp ( "^\\." ) ) )
    return domain.replace ( QRegExp ( "^\\." ), "" );
  else
    return domain;
}

/**
* Entfernt cName(www) vom Hostnamen und gibt die Domäne
* mit führenden Punkt zurück.
*/
const QString NetworkCookie::cookieDomainFromUrl ( const QUrl &url ) const
{
  QString domain = url.host().replace ( QRegExp ( "^www\\." ), "." );
  if ( domain.contains ( QRegExp ( "^\\." ) ) )
    return domain;
  else
    return "." + domain;
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

  QString host1 = cookieDomainFromUrl ( url );
  QString host2 = cookieHostnameFromUrl ( url );
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

  setAllCookies ( m_cookiesDatabase->loadCookies() );
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
  QList<QNetworkCookie> cookies = allCookies();
  // Speicher die Cookies in die Datenbank
  m_cookiesDatabase->saveCookies ( cookies );
#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "(XHTMLDBG) Cookies Saved";
#endif
  // Die Liste wieder leeren
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

  // Befindet sich der Hostname in Bearbeitung dann hier aussteigen aussteigen!
  QString uniqueHostRequest = url.host ();
  if ( inProgress.contains ( uniqueHostRequest ) )
    return false;

  // Setze den Hostnamen dieser Url in die Berarbeitenliste!
  inProgress << uniqueHostRequest;

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

  // Nachsehen ob dieser Host immer erlaubt oder nur alls Session genehmigt ist.
  yes = ( cookiesAllowed.indexOf ( cookieHost ) != -1 ) ? true : false;
  tmp = ( cookiesSession.indexOf ( cookieHost ) != -1 ) ? true : false;

#ifdef XHTMLDBG_DEBUG_VERBOSE
  qDebug() << "(XHTMLDBG) Cookie Request - Host:" << cookieHost
  << " Unique:" << uniqueHostRequest << " Acceppted:" << yes << " Session Cookie:" << tmp
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
        cookie.setDomain ( cookieDomainFromUrl ( cookieUrl ) );

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
  else
  {
    // Anfrage an Page Senden
    emit cookieRequest ( cookieUrl );
    return false;
  }

  if ( isInSecure )
    emit cookieNotice ( trUtf8 ( "Missing Optional Cookie/Secure attribute for HTTPS Scheme" ) );

  // Wenn neu erzeugt dann später speichern
  if ( add && yes )
    save();
  else if ( add )
    m_autoSaver->saveIfNeccessary();

  return add;
}

NetworkCookie::~NetworkCookie()
{
  m_autoSaver->saveIfNeccessary();
  inProgress.clear();
}
