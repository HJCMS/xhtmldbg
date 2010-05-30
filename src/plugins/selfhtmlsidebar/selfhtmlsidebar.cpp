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

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QSizePolicy>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

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
    , cfg ( new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "xhtmldbg", this ) )
{
  setObjectName ( QLatin1String ( "selfhtmlsidebar" ) );
  setWindowTitle ( QLatin1String ( "SELFTHML" ) );

  QWidget* layer = new QWidget ( this );
  layer->setContentsMargins ( 1, 2, 1, 2 );

  QVBoxLayout* vLayout = new QVBoxLayout ( layer );
  vLayout->setObjectName ( QLatin1String ( "vLayout" ) );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  QSizePolicy policy ( QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::DefaultType );

  m_webView = new QWebView ( layer );
  m_webView->setObjectName ( QLatin1String ( "slelfhtmlviewer" ) );
  m_webView->setSizePolicy ( policy );
  vLayout->addWidget ( m_webView );

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
  openIndex();

  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->setObjectName ( QLatin1String ( "hLayout" ) );
  hLayout->setContentsMargins ( 0, 0, 0, 0 );
  hLayout->setSpacing ( 5 );
  vLayout->addLayout ( hLayout );

  QToolButton* startPage = new QToolButton ( layer );
  startPage->setObjectName ( QLatin1String ( "startpagebutton" ) );
  startPage->setStatusTip ( trUtf8 ( "Index" ) );
  startPage->setToolTip ( trUtf8 ( "Index" ) );
  startPage->setIcon ( QIcon::fromTheme ( QLatin1String ( "user-home" ) ) );
  hLayout->addWidget ( startPage );

  m_label = new QLabel ( layer );
  m_label->setObjectName ( QLatin1String ( "infolabel" ) );
  m_label->setScaledContents ( false );
  m_label->setOpenExternalLinks ( false );
  m_label->setMargin ( 1 );
  hLayout->addWidget ( m_label );

  QToolButton* configPage = new QToolButton ( layer );
  configPage->setObjectName ( QLatin1String ( "configurebutton" ) );
  configPage->setStatusTip ( trUtf8 ( "Configure Startpage" ) );
  configPage->setToolTip ( trUtf8 ( "Configure Startpage" ) );
  configPage->setIcon ( QIcon::fromTheme ( QLatin1String ( "configure" ) ) );
  hLayout->addWidget ( configPage );

  // Layout abschliessen
  layer->setLayout ( vLayout );
  setWidget ( layer );

  connect ( m_webView, SIGNAL ( linkClicked ( const QUrl & ) ),
            this, SLOT ( openLinkClicked ( const QUrl & ) ) );

  connect ( startPage, SIGNAL ( clicked () ), this, SLOT ( openIndex () ) );
  connect ( configPage, SIGNAL ( clicked () ), this, SLOT ( openConfig () ) );
}

/**
* Sehe nach ob die Globale Variable SELFHTML_SIDEBAR_URL gesetzt ist.
* Wenn ja diese als Hauptseite setzen.
*/
const QUrl SelfHtmlSidebar::sideBarUrl() const
{
  QString selfhtml ( "http://de.selfhtml.org/navigation/sidebars/html.htm" );
  QString sUrl = cfg->value ( QLatin1String ( "Plugins/SelfHtmlSidebarUrl" ), selfhtml ).toString();
  QUrl url ( sUrl, QUrl::StrictMode );
  return url;
}

/**
* Konfigurations Dialog Erstellen
*/
void SelfHtmlSidebar::openConfigDialog ()
{
  QString selfhtml ( "http://de.selfhtml.org/navigation/sidebars/html.htm" );
  QString sUrl = cfg->value ( QLatin1String ( "Plugins/SelfHtmlSidebarUrl" ), selfhtml ).toString();
  QDialog* dialog = new QDialog ( this );
  dialog->setObjectName ( QLatin1String ( "configselfhtmldialog" ) );
  dialog->setMinimumWidth ( 350 );
  dialog->setSizeGripEnabled ( true );

  QVBoxLayout* layout = new QVBoxLayout ( dialog );

  layout->addWidget ( new QLabel ( trUtf8( "Configure the SELFHTML Index HTML" ), dialog ) );

  QLineEdit* setUrl = new QLineEdit ( sUrl, dialog );
  layout->addWidget ( setUrl );

  QDialogButtonBox* box = new QDialogButtonBox ( ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ),
          Qt::Horizontal, dialog );
  connect ( box, SIGNAL ( accepted() ), dialog, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected() ), dialog, SLOT ( reject() ) );

  layout->addWidget ( box );

  if ( dialog->exec() == QDialog::Accepted )
    cfg->setValue ( QLatin1String ( "Plugins/SelfHtmlSidebarUrl" ), setUrl->text() );

  delete dialog;
}

/**
* Startseite Öffnen
*/
void SelfHtmlSidebar::openIndex ()
{
  lastChanged = sideBarUrl();
  m_webView->load ( lastChanged );
}

/**
* Konfiguration Öffnen
*/
void SelfHtmlSidebar::openConfig ()
{
  openConfigDialog();
}

void SelfHtmlSidebar::openLinkClicked ( const QUrl &url )
{
  // BUG Qt4 and fragment!
  QUrl stripped ( url.toString ( ( QUrl::RemoveQuery | QUrl::RemoveFragment ) ) );
  if ( stripped.host() == sideBarUrl().host() )
  {
    m_webView->load ( url );
    // if ( url.hasFragment() )
    //  m_webView->setUrl ( QString ( "#%1" ).arg ( url.fragment() ) );
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
  m_label->setText ( url.toString() );
}

SelfHtmlSidebar::~SelfHtmlSidebar()
{}
