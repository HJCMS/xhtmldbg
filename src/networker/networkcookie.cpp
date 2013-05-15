/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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
#include "cookiemanager.h"

/* QtCore */
#include <QtCore/QChar>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QVariant>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

/* QtNetwork */
#include <QtNetwork/QHostInfo>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KLocalizedString>

NetworkCookie::NetworkCookie ( NetworkSettings * settings, QObject * parent )
    : KIO::Integration::CookieJar ( parent )
    , m_netcfg ( settings )
    , m_autoSaver ( new AutoSaver ( this ) )
    , m_cookieManager ( new CookieManager ( this ) )
    , currentUrl ( "http://localhost" )
{
  setDisableCookieStorage ( false );

  if ( ! m_netcfg )
    m_netcfg = new NetworkSettings ( this );

  inProgress.clear();

  if ( m_cookieManager->isOpen() )
    load();

  m_autoSaver->changeOccurred();
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
bool NetworkCookie::validateDomainAndHost ( const QString &domain, const QUrl &url, bool rfc )
{
  if ( domain.isEmpty() )
    return false;

  QString host1 = cookieDomainFromUrl ( url );
  QString host2 = cookieHostnameFromUrl ( url );
  QString rejectMessage = i18n ( "Impermissible Cookie format for \"%1\" and Cookie Domain \"%2\" rejected by RFC 2109." )
                          .arg ( url.host(), domain );
  rejectMessage.append ( QLatin1String ( " " ) );

  if ( rfc && ! domain.startsWith ( QLatin1String ( "." ) ) )
  {
#ifdef DEBUG_VERBOSE
    qDebug() << "(XHTMLDBG) RFC2109 REJECT:" << domain;
#endif
    rejectMessage.append ( i18n ( "A Set-Cookie with Domain=%1 will be rejected because the value for Domain does not begin with a dot." ).arg ( domain ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( domain.endsWith ( QLatin1String ( "." ) ) )
  {
    rejectMessage.append ( i18n ( "A Set-Cookie with attached dot Domain=%1, will always be rejected." ).arg ( domain ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( ! domain.contains ( QRegExp ( "([\\S]+)\\.(\\w{2,})$" ) ) )
  {
    rejectMessage.append ( i18n ( "A Set-Cookie with missing Hostname Domain=.tld, will always be rejected." ) );
    emit cookieRejected ( rejectMessage );
    return false;
  }
  else if ( host1.compare ( domain, Qt::CaseInsensitive ) == 0 )
  {
#ifdef DEBUG_VERBOSE
    qDebug() << "(XHTMLDBG) Accepted:" << domain;
#endif
    // Accepted: .host.tld == .domain.tld
    return true;
  }
  else if ( host2.compare ( domain, Qt::CaseInsensitive ) == 0 )
  {
#ifdef DEBUG_VERBOSE
    qDebug() << "(XHTMLDBG) Accepted:" << domain;
#endif
    // Accepted: host.tld == .domain.tld
    return true;
  }
  else if ( ! rfc )
  {
#ifdef DEBUG_VERBOSE
    qDebug() << "(XHTMLDBG) Accepted NONE RFC2109:" << domain;
#endif
    // Accepted: NONE RFC2109
    return true;
  }

  emit cookieNotice ( i18n ( "Different Cookie/Domain %1 for host %2. (Rejected)" ).arg ( domain, url.host() ) );
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
  setAllCookies ( m_cookieManager->getCookies() );
}

/**
* Aktuelle Cookies, bis auf die Session Cookies speichern!
*/
void NetworkCookie::save()
{
  QList<QNetworkCookie> cookies = allCookies();
  // Speichere die Cookies in die Datenbank
  m_cookieManager->saveCookiesList ( cookies );

  inProgress.clear();
  load();
}

/**
* Diese Methode wird nicht mehr benötigt!
*/
void NetworkCookie::setUrl ( const QUrl &url )
{
  currentUrl = url;
}

/**
* Versucht an hand der aktuellen Anfrage URL und
* der Seiten Url zu ermitteln, ob es sich um eine
* Drittanbieter Domäne handelt!
*
* @li Wenn dies der fall ist wird true zurück gegeben!
* @li Wenn es sich um eine "localhost" Adresse handelt
*   oder die Adresse nicht die bestanteile von DOMAIN.TLD
*   enthält wird ebenfalls true zurück gegeben!
*   Eine Reguläre Adresse muss mindestens ein Root
*   Zeichen und Hostname vor der TLD enthalten.
*   Andernfalls ist die Adresse nicht RFC Konform!
*
* @param hostname der vom CookieManager übergeben wurde
* @param url  die url der Anfrage
* @see setCookiesFromUrl
*/
bool NetworkCookie::isThirdPartyDomain ( const QString &hostname, const QUrl &url ) const
{
  // Wenn Leer dann true zurück geben ;-/
  if ( hostname.isEmpty() )
    return true;

  // Ermittle "Domain" und "Top Level Domain"
  // Manche Werbebanner... setzen SubDomain mit den Hostnamen der Original Seite!
  QHostInfo info = QHostInfo::fromName ( url.host() );
  if ( info.error() == QHostInfo::NoError )
  {
    QString domain = info.hostName();
    // Nachsehen ob die Domain aus <host>.<tld> besteht! -- Wenn nicht abbruch mit true!
    if ( ! domain.contains ( QRegExp ( "\\w+\\.\\w{2,}$" ) ) )
      return true;

    domain.prepend ( "." ); // mache RFC Conform
    // Prüfen ob der Hostname in der domain enthalten ist!
    // Wenn ja auf false kein 3.Anbieter Cookie setzen!
    bool status = ( domain.contains ( hostname ) ? false : true );
#ifdef DEBUG_VERBOSE
    if ( status )
      qDebug() << "(XHTMLDBG) Cookie isThirdParty Domain:" << domain << " Hostname: "  << hostname;
#endif
    // Wenn hostname vorhanden sollte es kein 3. Anbieter
    return status;
  }
  // letzter versuch wenn Hostname vorhanden dann false
  return ( url.host().contains ( hostname ) ? false : true );
}

/**
* Nehme alle Cookies von dieser URL
* Dabei wird eine Neue Url mit Scheme/Host/Path erstellt!
* Der Rest geht in die Mülltonne!
*/
QList<QNetworkCookie> NetworkCookie::cookiesForUrl ( const QUrl &url ) const
{
  QList<QNetworkCookie> list;
  if ( url.isValid() )
    list = QNetworkCookieJar::cookiesForUrl ( url );

  // Wenn leer - versuche es nur mit der Domäne und Pfad beim CookieManager
  if ( ( list.size() < 1 ) && url.scheme().contains ( "http" ) )
  {
    QString domain ( url.host().remove ( QRegExp ( "^www[\\.]?\\b" ) ) );
    QString path = url.path();
    if ( ! path.contains ( QRegExp ( "^\\/" ) ) )
      path = QLatin1String ( "/" );

    list = m_cookieManager->getCookiesForDomain ( domain, path );
  }
  return list;
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
*    @li Überprüfen ob im Cookie der Wert cookie:.domain.tld nicht leer ist.
*     Wenn doch! => An dieser Stelle aussteigen!
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
  // XHTMLDBG Prodiziert einen loop wenn es sich nicht um eine Cookie URL handelt!
  if ( ! url.scheme().contains ( "http" ) )
    return false;

  // Fehlerhafte Url's erst gar nicht akzeptieren!
  if ( ! url.isValid() )
  {
    emit cookieNotice ( i18n ( "Invalid Url for Cookie Request - rejected!" ) );
    return false;
  }

  // Befindet sich der Hostname in Bearbeitung dann hier aussteigen aussteigen!
  QString cookieHost = url.host().remove ( QRegExp ( "^www[\\.]?" ) );
  if ( inProgress.contains ( cookieHost ) )
    return false;

  // Setze den Hostnamen dieser Url in die Bearbeitenliste!
  inProgress << cookieHost;

  if ( ! m_cookieManager->isOpen() )
    return false;

  CookieManager::CookiesAccessItem cookieAcces = m_cookieManager->getCookieAccess ( cookieHost );

  bool add = false;
  bool yes = false;
  bool tmp = false;

  if ( cookieAcces.Access == CookieManager::BLOCKED )
  {
    // Wenn dieser Host in der Blockliste steht sofort aussteigen.
    emit cookieNotice ( i18n ( "Cookie for Host \"%1\" rejected by blocked list!" ).arg ( cookieHost ) );
    return false;
  }
  else if ( ! cookieAcces.AllowThirdParty && isThirdPartyDomain ( cookieAcces.Hostname, url ) )
  {
    // Drittanbieter keks Anfrage verwerfen.
    emit cookieNotice ( i18n ( "third-party vendor cookie \"%1\" for host \"%2\" rejected!" )
                        .arg ( url.host(), cookieAcces.Hostname ) );
    return false;
  }

  // Nachsehen ob dieser Host immer erlaubt oder nur alls Session genehmigt ist.
  yes = ( cookieAcces.Access == CookieManager::ALLOWED ) ? true : false;
  tmp = ( cookieAcces.Access == CookieManager::SESSION ) ? true : false;

  // Eine Kopie von "url" erstellen und bereinigen
  QUrl cookieUrl;
  cookieUrl.setScheme ( url.scheme() );
  cookieUrl.setHost ( url.host() );
  cookieUrl.setPath ( url.path() );

  // Neue Cookie Laufzeit
  QDateTime lifeTime = cookieLifeTime();

  if ( tmp || yes )
  {
    QList<QNetworkCookie> cookies;
    foreach ( QNetworkCookie cookie, list )
    {
      // @modified 2011/07/14
      if ( cookie.domain().isEmpty() )
      {
        emit cookieNotice ( i18n ( "empty cookies domain parameter!" ) );
        yes = false;
        break;
      }

      // Session Cookie behandlung
      if ( tmp && ! cookie.isSessionCookie() )
      {
        // Wenn kein SessionCookie aber soll als Session erzwingen dann ...
        cookie.setExpirationDate ( QDateTime() );
      }
      else if ( cookie.isSessionCookie() && ( cookie.expirationDate() > lifeTime ) )
      {
        // Wenn SessionCookie aber die Laufzeit zu hoch ist dann ...
        cookie.setExpirationDate ( lifeTime );
      }

      if ( ! validateDomainAndHost ( cookie.domain(), cookieUrl, cookieAcces.RFC2109 ) )
        continue;

      if ( url.scheme().contains ( "https" ) && ! cookie.isSecure() )
        emit cookieNotice ( i18n ( "Missing Optional Cookie/Secure attribute for HTTPS Scheme" ) );

      // Cookies von dieser Url zusammen packen
      cookies += cookie;
    } // end foreach
    // Jetzt die Cookies an QNetworkCookieJar übergeben
    if ( cookies.size() >=1 )
    {
      if ( QNetworkCookieJar::setCookiesFromUrl ( cookies, url ) )
      {
        emit cookieChanged ();
        if ( ! tmp )
          m_autoSaver->saveIfNeccessary();

#ifdef DEBUG_VERBOSE
        qDebug() << "(XHTMLDBG) Cookie saved for Hostname:" << cookieHost << " Url:"  << url << " Size:" << cookies.size();
#endif

        return true;
      }
    }
  }
  else
  {
    // Anfrage an Page Senden
    emit cookieRequest ( cookieUrl );
    return false;
  }

  // Wenn neu erzeugt dann später speichern
  if ( cookieAcces.Access == CookieManager::ALLOWED )
    save();
  else if ( add )
    save();
  else
    m_autoSaver->saveIfNeccessary();

  return add;
}

NetworkCookie::~NetworkCookie()
{
  m_autoSaver->saveIfNeccessary();
  inProgress.clear();
}
