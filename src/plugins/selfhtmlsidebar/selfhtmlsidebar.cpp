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

#include "selfhtmlsidebar.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtDBus */
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

SelfHtmlSidebar::SelfHtmlSidebar ( QWidget * parent )
    : QDockWidget ( parent )
    , service ( "de.hjcms.xhtmldbg" )
    , p_dbus ( QDBusConnection::connectToBus ( QDBusConnection::SessionBus, "de.hjcms.xhtmldbg" ) )
{
  setObjectName ( QLatin1String ( "selfhtmlsidebar" ) );
  setWindowTitle ( QLatin1String ( "SELFTHML" ) );

  m_webView = new QWebView ( this );

  QWebSettings* wcfg = m_webView->settings();
  wcfg->setDefaultTextEncoding ( QLatin1String ( "utf-8" ) );
  wcfg->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, false );
  wcfg->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, false );
  wcfg->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
  wcfg->setAttribute ( QWebSettings::AutoLoadImages, true );
  wcfg->setAttribute ( QWebSettings::JavascriptEnabled, true );
  wcfg->setAttribute ( QWebSettings::PluginsEnabled, false );
  wcfg->setAttribute ( QWebSettings::JavaEnabled, false );
  wcfg->setAttribute ( QWebSettings::PrivateBrowsingEnabled, false );

  m_webView->page()->setLinkDelegationPolicy ( QWebPage::DelegateExternalLinks );
  m_webView->page()->setForwardUnsupportedContent ( false );

  setWidget ( m_webView );

  connect ( m_webView, SIGNAL ( linkClicked ( const QUrl & ) ),
            this, SLOT ( openLinkClicked ( const QUrl & ) ) );

  lastChanged = sideBarUrl();
  m_webView->load ( lastChanged );
}

const QUrl SelfHtmlSidebar::sideBarUrl() const
{
  QUrl fallback ( "http://de.selfhtml.org/navigation/sidebars/html.htm" );
  QString sitebarurl ( getenv ( "SELFHTML_SIDEBAR_URL" ) );
  if ( sitebarurl.isEmpty() )
    return fallback;

  QUrl url ( sitebarurl, QUrl::StrictMode );
  if ( url.isValid() )
    return url;
  else
    return fallback;
}

void SelfHtmlSidebar::openLinkClicked ( const QUrl &url )
{
  // BUG Qt4 and fragment!
  QUrl stripped ( url.toString ( ( QUrl::RemoveQuery | QUrl::RemoveFragment ) ) );
  if ( stripped.host() == sideBarUrl().host() )
  {
    m_webView->setUrl ( stripped );
    if ( url.hasFragment() )
      qDebug() << Q_FUNC_INFO << url.fragment();
  }
  else if ( p_dbus.isConnected() )
  {
    QList<QVariant> args;
    args << lastChanged.toString() << stripped.toString();
    QDBusMessage meth = QDBusMessage::createMethodCall ( service, "/xhtmldbg", service, "setUrl" );
    meth.setArguments ( args );
    if ( p_dbus.send ( meth ) )
    {
      p_dbus.disconnectFromBus ( service );
      lastChanged = url;
    }
  }
  else
  {
    qWarning ( "Cannot connect to the \"xhtmldbg\" D-Bus session bus." );
    m_webView->setUrl ( url );
  }
}

SelfHtmlSidebar::~SelfHtmlSidebar()
{}
