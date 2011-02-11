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
#include <QtCore/QRegExp>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtCore/QMargins>

/* QtGui */
#include <QtGui/QCursor>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QSizePolicy>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>

/* QtWebKit */
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebSettings>

SelfHtmlSidebar::SelfHtmlSidebar ( QWidget * parent )
    : QDockWidget ( parent )
    , service ( "de.hjcms.xhtmldbg" )
    , p_dbus ( QDBusConnection::sessionBus() )
    , cfg ( new QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "xhtmldbg", this ) )
{
  setObjectName ( QLatin1String ( "SelfHtmlSidebar" ) );
  setWindowTitle ( QLatin1String ( "SELFHTML" ) );

  QWidget* layer = new QWidget ( this );
  layer->setContentsMargins ( 1, 2, 1, 2 );

  QVBoxLayout* vLayout = new QVBoxLayout ( layer );
  vLayout->setObjectName ( QLatin1String ( "vLayout" ) );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  QSizePolicy policy ( QSizePolicy::Expanding, QSizePolicy::Expanding, QSizePolicy::DefaultType );

  m_titleLabel = new QLabel ( layer );
  m_titleLabel->setObjectName ( QLatin1String ( "titlelabel" ) );
  m_titleLabel->setScaledContents ( false );
  m_titleLabel->setWordWrap ( true );
  m_titleLabel->setOpenExternalLinks ( false );
  m_titleLabel->setMargin ( 1 );
  m_titleLabel->setTextFormat ( Qt::PlainText );
  m_titleLabel->setTextInteractionFlags ( Qt::NoTextInteraction );
  vLayout->addWidget ( m_titleLabel );

  m_webView = new QWebView ( layer );
  m_webView->setObjectName ( QLatin1String ( "Viewer" ) );
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

  QToolButton* configPage = new QToolButton ( layer );
  configPage->setObjectName ( QLatin1String ( "configurebutton" ) );
  configPage->setStatusTip ( trUtf8 ( "Configure Startpage" ) );
  configPage->setToolTip ( trUtf8 ( "Configure Startpage" ) );
  configPage->setIcon ( QIcon::fromTheme ( QLatin1String ( "configure" ) ) );
  hLayout->addWidget ( configPage );
  hLayout->addStretch ( 1 );

  QToolButton* startPage = new QToolButton ( layer );
  startPage->setObjectName ( QLatin1String ( "Index" ) );
  startPage->setStatusTip ( trUtf8 ( "Index" ) );
  startPage->setToolTip ( trUtf8 ( "Index" ) );
  startPage->setIcon ( QIcon::fromTheme ( QLatin1String ( "user-home" ) ) );
  hLayout->addWidget ( startPage );

  searchLine = new QLineEdit ( layer );
  searchLine->setObjectName ( QLatin1String ( "Search" ) );
  hLayout->addWidget ( searchLine );

  QToolButton* searchButton = new QToolButton ( layer );
  searchButton->setObjectName ( QLatin1String ( "searchbutton" ) );
  searchButton->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-find" ) ) );
  hLayout->addWidget ( searchButton );

  m_urlLabel = new QLabel ( layer );
  m_urlLabel->setObjectName ( QLatin1String ( "infolabel" ) );
  m_urlLabel->setScaledContents ( false );
  m_urlLabel->setWordWrap ( true );
  m_urlLabel->setOpenExternalLinks ( false );
  m_urlLabel->setMargin ( 1 );
  m_urlLabel->setTextFormat ( Qt::PlainText );
  m_urlLabel->setTextInteractionFlags ( Qt::NoTextInteraction );
  vLayout->addWidget ( m_urlLabel );

  // Layout abschliessen
  layer->setLayout ( vLayout );
  setWidget ( layer );

  connect ( m_webView, SIGNAL ( titleChanged ( const QString & ) ),
            m_titleLabel, SLOT ( setText ( const QString & ) ) );

  connect ( m_webView, SIGNAL ( linkClicked ( const QUrl & ) ),
            this, SLOT ( openLinkClicked ( const QUrl & ) ) );

  connect ( searchLine, SIGNAL ( returnPressed () ), this, SLOT ( findKeyword () ) );
  connect ( searchButton, SIGNAL ( clicked () ), this, SLOT ( findKeyword () ) );
  connect ( startPage, SIGNAL ( clicked () ), this, SLOT ( openIndex () ) );
  connect ( configPage, SIGNAL ( clicked () ), this, SLOT ( openConfig () ) );

  // D-Bus Registrierung
  if ( p_dbus.registerObject ( "/Plugin/SelfHtml", this, QDBusConnection::ExportAllSignals ) )
  {
    p_dbus.registerObject ( "/Plugin/SelfHtml/Viewer", m_webView, QDBusConnection::ExportAllSlots );
    p_dbus.registerObject ( "/Plugin/SelfHtml/Search", searchLine, QDBusConnection::ExportAllSlots );
  }
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

  layout->addWidget ( new QLabel ( trUtf8 ( "Configure the SELFHTML Index HTML" ), dialog ) );

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
* Wenn nicht sichtbar - gehe zu gefundenen Element!
*/
void SelfHtmlSidebar::gotoKeyword ( const QString &w )
{
  QWebElement body = m_webView->page()->currentFrame()->findFirstElement ( "BODY" );
  foreach ( QWebElement we, body.findAll ( "CODE" ) )
  {
    if ( we.toPlainText().contains ( w ) )
    {
      QRect elementRect = we.geometry();
      if ( elementRect.isValid() && we.webFrame() )
      {
        QWebFrame* currentFrame = we.webFrame();
        QRect topRect = currentFrame->geometry();
        topRect.setTop ( currentFrame->scrollPosition().y() );
        if ( ! topRect.contains ( elementRect ) )
          currentFrame->setScrollPosition ( elementRect.topLeft() );
      }
      break;
    }
  }
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

/**
* Wenn ein Link angeklickt wird nachsehen ob es sich um einen Externe
* Url handelt, oder ob es ein Link ist der von dieser Basis Url ausgeht.
* Wenn von der Basis Url dann im selben Viewer anzeigen andernfalls
* via DBus an XHTMLDBG senden.
*/
void SelfHtmlSidebar::openLinkClicked ( const QUrl &url )
{
  // Lokale anfragen hier verarbeiten!
  QUrl stripped ( url.toString ( ( QUrl::RemoveQuery | QUrl::RemoveFragment ) ) );
  if ( stripped.host() == sideBarUrl().host() )
  {
    m_webView->load ( url );
  }
  else if ( p_dbus.isConnected() ) // andernfalls an XHTMLDBG senden
  {
    QList<QVariant> args;
    args << lastChanged.toString() << stripped.toString();
    QDBusMessage meth = QDBusMessage::createMethodCall ( p_dbus.baseService(), "/xhtmldbg/Window",
                                                         service, "setUrl" );
    meth.setArguments ( args );
    if ( p_dbus.send ( meth ) )
    {
      p_dbus.disconnectFromBus ( service );
      lastChanged = url;
    }
    // qDebug() << p_dbus.lastError().message();
  }
  else // fehler ausgeben und lokal verarbeiten
  {
    qWarning ( "(XHTMLDBG:SELFHTML) Cannot connect to the \"xhtmldbg\" D-Bus session bus." );
    m_webView->setUrl ( url );
  }
  m_urlLabel->setText ( url.toString() );
}

/**
* Schlüsselwort Suche
*/
void SelfHtmlSidebar::findKeyword ( const QString &word )
{
  QString w = ( word.isEmpty() ) ? searchLine->text() : word;
  if ( w.isEmpty() )
    return;

  m_webView->findText ( QLatin1String ( "" ), QWebPage::FindBackward );
  if ( m_webView->findText ( w, QWebPage::HighlightAllOccurrences ) )
    gotoKeyword ( w );
}

/**
* Verhindern das sich @ref QDockWidget::SelfHtmlSidebar bei überlangen
* Adressen und Titel in den QLabel's Automatisch anpasst.
* @note Das Hauptfenster darf nicht Automtatisch verändert werden!
*/
void SelfHtmlSidebar::resizeEvent ( QResizeEvent * ev )
{
  QMargins m = contentsMargins();
  int mw = ( ev->size().width() - ( m_urlLabel->margin() * 2 ) - ( m.left() + m.right() ) );
  m_titleLabel->setMaximumWidth ( mw );
  m_urlLabel->setMaximumWidth ( mw );
}

SelfHtmlSidebar::~SelfHtmlSidebar()
{}
