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

#include "settings.h"
#include "xhtmldbgmain.h"
#include "viewer.h"
#include "webviewer.h"
#include "page.h"
#include "window.h"
#include "networkaccessmanager.h"
#include "networkcookie.h"
#include "cookieacceptdialog.h"
#include "useragentmenu.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QHelpEvent>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QToolTip>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

Viewer::Viewer ( QWidget * parent )
    : QWebView ( parent )
{
  if ( objectName().isEmpty() )
    setObjectName ( "viewer" );

  setContentsMargins ( 0, 0, 0, 0 );
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  NetworkAccessManager* netManager = xhtmldbgmain::instance()->networkAccessManager();
  NetworkCookie* networkCookieManager = xhtmldbgmain::instance()->cookieManager();

  m_page = new Page ( netManager, this );
  setPage ( m_page );

  /* signal cookie nachfrage dialog */
  connect ( networkCookieManager, SIGNAL ( cookieRequest ( const QUrl & ) ),
            this, SLOT ( cookiesRequest ( const QUrl & ) ) );
  connect ( networkCookieManager, SIGNAL ( cookieRejected ( const QString & ) ),
            this, SLOT ( errorMessage ( const QString & ) ) );
  connect ( networkCookieManager, SIGNAL ( cookieNotice ( const QString & ) ),
            this, SLOT ( errorMessage ( const QString & ) ) );

  /* Bei jeder Verknüpfungs Anfrage dem Netzwerk Manager als auch
  * dem Cookie Manager Mitteilen welches die Orignalanfrage Adresse */
  connect ( this, SIGNAL ( linkClicked ( const QUrl & ) ),
            netManager, SLOT ( setUrl ( const QUrl & ) ) );

  connect ( this, SIGNAL ( linkClicked ( const QUrl & ) ),
            networkCookieManager, SLOT ( setUrl ( const QUrl & ) ) );

  /* start maus-sanduhr */
  connect ( this, SIGNAL ( loadStarted () ), this, SLOT ( cursorwait () ) );

  /* stop maus-sanduhr */
  connect ( this, SIGNAL ( loadFinished ( bool ) ), this, SLOT ( cursorFinished ( bool ) ) );

  /* bei überfahren mir der maus informationen der verknüpfung ausgeben. */
  connect ( m_page, SIGNAL ( linkHovered ( const QString &, const QString &, const QString & ) ),
            this, SLOT ( linkInfos ( const QString &, const QString &, const QString & ) ) );
}

/**
* Öffnet den Keks Dialog mit den gesammelten Cookies
* Anfragen aus @ref pendingCookieRequests und arbeitet
* sie nach und nach ab.
* Wenn Cookies akzeptiert wurden wird das Signal
* @ref CookieManager::reload abgegeben.
*/
void Viewer::openCookiesRequestDialog ()
{
  // Wenn nicht vorhanden aussteigen
  if ( pendingCookieRequests.size() < 1 )
    return;

  /** WARNING Die initalisierung von CookieAcceptDialog muss,
  * Wegen @ref CookiesEditorTable nach @class NetworkCookie Initialisiert werden.
  * Grund hier für ist das öffnen der SQL Datenbank!
  */
  CookieAcceptDialog* m_cookiesDialog = new CookieAcceptDialog ( this );
  // Die gesammelten Anfragen in den Dialog einfügen
  foreach ( QUrl url, pendingCookieRequests )
  {
    m_cookiesDialog->setCookieUrl ( url );
  }

  if ( m_cookiesDialog->exec() == QDialog::Accepted )
  {
    pendingCookieRequests.clear(); // Aufräumen
#ifdef HTMLDBG_DEBUG_VERBOSE
    qDebug ( "(XHTMLDBG) Cookie for \"%s\" Accepted", url().host() );
#endif
  }
  delete m_cookiesDialog;
}

/**
* Mit dieser Methode wird das Signal für die
* Klasse @class DomTree aufgearbeitet.
* Mit hitTestContent wird vom aktuellen Rahmen
* die Position des WebElementes abgegriffen.
*/
void Viewer::findChildNode ( const QPoint &p )
{
  if ( ! p.isNull() )
  {
    QWebHitTestResult result = page()->currentFrame()->hitTestContent ( p );
    emit hitTestResult ( result.element() );

    if ( ! result.linkElement().isNull() )
      prepareLinkInfo ( result );
  }
}

/**
* An dieser Stelle werden A Elemente übergeben die mit
* hilfe von @ref findChildNode übermittelt werden.
* Diese Methode Versucht die Kompletten Link Informationen
* von @ref QWebHitTestResult zu ermitteln und sendet das
* Signal @ref linkTriggered.
*/
void Viewer::prepareLinkInfo ( const QWebHitTestResult &link )
{
  if ( link.linkUrl().isValid() )
    emit linkTriggered ( link.linkUrl(), link.linkText(), link.linkTitle().toString() );
}

/**
* Setze die Maus Sanduhr
* Wird immer dann aufgerufen wenn ein
* Signal loadStarted abgegeben wird.
*/
void Viewer::cursorwait ()
{
  setCursor ( Qt::WaitCursor );

  // Bie neuer Seitenanfrage den Cookie Speicher freigeben
//   if ( pendingCookieRequests.size() > 0 )
//     pendingCookieRequests.clear();
}

/**
* Wird immer dann aufgerufen wenn ein signal @ref QWebView::loadFinished abgegeben wird.
* @li Entferne die Maus Sanduhr
* @li Sende die Seitengröße mit @ref totalBytes
* @li Rufe @ref openCookiesRequestDialog für die Cookies anfragen Verarbeitung auf.
*/
void Viewer::cursorFinished ( bool )
{
  if ( page()->bytesReceived() >= 1000 )
  {
    setCursor ( Qt::ArrowCursor );
    emit totalBytes ( page()->bytesReceived() );
    // Seite geladen - dann Cookie Speicher verarbeiten
    openCookiesRequestDialog();
  }
}

/**
* Konvertiere Lesezeichen Signal @ref addBookmark
* von @ref contextMenuEvent Menu
*/
void Viewer::bookmark()
{
  emit addBookmark ( url(), title() );
}

/**
* Die Tastenkürzel haben im contextMenuEvent() keinen Effekt
* weil das Menü erst später Initialisiert wird und dienen rein zur
* Visuellen Information an den Benutzer. Das Eigentlich abfangen
* der Tastenkürzel geschieht hier.
*/
void Viewer::keyPressEvent ( QKeyEvent * e )
{
  // WARNING Es Funktionieren nur StandardKey's
  if ( e->matches ( QKeySequence::Bold ) )
    bookmark();
  else if ( e->matches ( QKeySequence::Italic ) )
    checkingStyleSheet();
  else if ( e->matches ( QKeySequence::Underline ) )
    showPageSource();

  QWebView::keyPressEvent ( e );
}

/**
* Füge Aktionen für (Lesezeichen|Stylesheet|User-Agent) mit ein.
*/
void Viewer::contextMenuEvent ( QContextMenuEvent * e )
{
  // Settings
  Settings* cfg = new Settings ( this );

  QMenu* menu = m_page->createStandardContextMenu();
  // Lesezeichen
  QAction* add = menu->addAction ( trUtf8 ( "Bookmark" ) );
  add->setObjectName ( QLatin1String ( "ac_context_addbookmarkaction" ) );
  add->setIcon ( QIcon::fromTheme ( QLatin1String ( "bookmark-new" ) ) );
  add->setShortcut ( QKeySequence::Bold );
  connect ( add, SIGNAL ( triggered() ), this, SLOT ( bookmark() ) );

  // Stylesheet Überprüfung
  QAction* style = menu->addAction ( trUtf8 ( "StyleSheet" ) );
  style->setObjectName ( QLatin1String ( "ac_context_stylesheet" ) );
  style->setIcon ( QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-stylesheets" ) ) );
  style->setToolTip ( trUtf8 ( "Start CSS Validation for this Site." ) );
  style->setShortcut ( QKeySequence::Italic );
  connect ( style, SIGNAL ( triggered() ), this, SLOT ( checkingStyleSheet() ) );

  // Stylesheet Überprüfung
  QAction* source = menu->addAction ( trUtf8 ( "Source" ) );
  source->setObjectName ( QLatin1String ( "ac_context_source" ) );
  source->setIcon ( QIcon::fromTheme ( QLatin1String ( "text-html" ) ) );
  source->setToolTip ( trUtf8 ( "Show Document Source" ) );
  source->setShortcut ( QKeySequence::Underline );
  connect ( source, SIGNAL ( triggered() ), this, SLOT ( showPageSource() ) );

  // User-Agent
  menu->addMenu ( new UserAgentMenu ( menu, cfg ) );

  menu->exec ( e->globalPos() );
  delete menu;
}

/**
* Event für @ref findChildNode und dem Auffinden
* von WebElementen für @class DomTree
*/
void Viewer::mousePressEvent ( QMouseEvent * ev )
{
  findChildNode ( ev->pos() );
  QWebView::mousePressEvent ( ev );
}

/**
* Alle Fenster anfragen werden in einem neuen
* Tab gesetzt und nicht ein zweites Fenster geöffnet !
*/
Viewer* Viewer::createWindow ( QWebPage::WebWindowType t )
{
  if ( QWebPage::WebModalDialog == t )
    return this;

  QObject* w = this->parent();
  while ( w )
  {
    if ( WebViewer* mw = qobject_cast<WebViewer*> ( w ) )
    {
      Viewer *view = new Viewer ( mw );
      mw->addViewerTab ( view );
      return view;
    }
    w = w->parent();
  }
  return this;
}

/**
* Dieser Slot wird vom Signal cookiesRequest aufgerufen.
* Es werden hierbei alle Cookies Host in die Liste von
* @ref pendingCookieRequests geschrieben und beim Signal
* @ref QWebView::loadFinished wird die Methode
* @ref openCookiesRequestDialog aufgerufen.
*/
void Viewer::cookiesRequest ( const QUrl &url )
{
  QUrl cookieUrl;
  cookieUrl.setScheme ( url.scheme() );
  cookieUrl.setHost ( url.host() );
  // Wenn nocht nicht vorhanden in die Liste einfügen.
  if ( ! pendingCookieRequests.contains ( cookieUrl ) )
    pendingCookieRequests << cookieUrl;
}

/**
* Zeige den Quelltext an
*/
void Viewer::showPageSource()
{
  xhtmldbgmain::instance()->mainWindow()->setCentralTabWidget ( QLatin1String ( "source" ) );
}

/**
* Sende eine Stylesheet Validieren Anfrage!
* Ausgelöst durch den Eintrag im Kontext Menü.
*/
void Viewer::checkingStyleSheet()
{
  xhtmldbgmain::instance()->mainWindow()->checkStyleSheet ( url() );
}

/**
* Dieser SLOT wird von @class KeywordsToolBar benötigt
* um die gesuchten Schlüsselwörter hervor zu heben.
*/
void Viewer::findKeyword ( const QString &word )
{
  // Reset anfrage
  if ( word.isEmpty() )
  {
    findText ( QString::null, QWebPage::HighlightAllOccurrences );
    return;
  }

  int summary = 0;
  QString message = trUtf8 ( "SEO Result for \"%1\" Url: " ).arg ( word );
  message.append ( url().toString ( ( QUrl::RemovePassword | QUrl::RemoveFragment ) ) );
  message.append ( "\n" );

  findText ( word, QWebPage::HighlightAllOccurrences );

  QString body = bodyContent();
  // An Wortgrenzen:
  message.append ( trUtf8 ( "Word boundary: " ) );
  message.append ( QString::number ( body.count ( QRegExp ( "\\b"+word+"\\b", Qt::CaseInsensitive ) ) ) );
  message.append ( " " );

  // Groß- Kleinschreibung:
  message.append ( trUtf8 ( "Case Sensitive: " ) );
  message.append ( QString::number ( body.count ( word, Qt::CaseSensitive ) ) );
  message.append ( " " );

  summary = body.count ( word, Qt::CaseInsensitive );

  QStringList meta = m_page->keywordMetaTagItems();

  // Meta Schlüsselwörter:
  int keywords = meta.at ( 0 ).count ( word, Qt::CaseInsensitive );
  message.append ( trUtf8 ( "Meta Keywords: " ) );
  message.append ( QString::number ( keywords ) );
  message.append ( " " );

  // Meta Beschreibung:
  int description = meta.at ( 1 ).count ( word, Qt::CaseInsensitive );
  message.append ( trUtf8 ( "Meta Description: " ) );
  message.append ( QString::number ( description ) );
  message.append ( " " );

  // Seiten Titel:
  QString site = title().isEmpty() ? "0" : title();
  int inTitle = site.count ( word, Qt::CaseInsensitive );
  message.append ( trUtf8 ( "Page Title: " ) );
  message.append ( QString::number ( inTitle ) );
  message.append ( " " );

  // Insgesamt:
  message.append ( trUtf8 ( "Summary: " ) );
  message.append ( QString::number ( ( summary + keywords + description + inTitle ) ) );

  xhtmldbgmain::instance()->mainWindow()->setApplicationMessage ( message );
}

/**
* Slot auf QWebView::setUrl
*/
void Viewer::openUrl ( const QUrl &url )
{
  // Sende die Seiten Url an den Cookie Manager
  xhtmldbgmain::instance()->cookieManager()->setUrl ( url );

  setUrl ( url );
}

/**
* Such im aktuellen Frame mit QwebElement nach dem BODY
* Tag und gibt dieses als text Zurück.
*/
const QString Viewer::bodyContent()
{
  return m_page->currentFrame()->findFirstElement ( QLatin1String ( "body" ) ).toPlainText();
}

/**
* Gibt den Original von page() Quelltext zurück!
*/
const QString Viewer::source()
{
  return m_page->xhtmlSource();
}

/**
* Erstelle Extras für die Anzeige erweiterte Verknüpfungs Informationen.
*/
void Viewer::linkInfos ( const QString &link, const QString &title, const QString & )
{
  if ( ! link.isEmpty() )
    emit statusBarMessage ( link );

  if ( title.isEmpty() )
    return;

  QPoint point = cursor ().pos();
  QToolTip::showText ( point, title, this, QRect ( point, QSize ( 20, 20 ) ) );
}

/** Nachricht weiter an IDE geben ! */
void Viewer::errorMessage ( const QString &error )
{
  xhtmldbgmain::instance()->mainWindow()->setApplicationMessage ( error, true );
}

Viewer::~Viewer()
{}
