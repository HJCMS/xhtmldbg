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

#include "dominspector.h"
#include "domtoolbar.h"
#include "domtree.h"
#include "liststylesheet.h"
#include "paintelement.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRect>
#include <QtCore/QPoint>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElementCollection>

DomInspector::DomInspector ( Settings * settings, QWidget * parent )
    : QDockWidget ( parent )
    , cfg ( settings )
    , m_paintElement ( 0 )
{
  Q_INIT_RESOURCE ( dominspector );
  setObjectName ( QLatin1String ( "domviewerdock" ) );
  setWindowTitle ( i18n ( "DomTree" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  m_domSplitter = new QSplitter ( Qt::Vertical, this );

  QWidget* layer = new QWidget ( m_domSplitter );
  layer->setContentsMargins ( 0, 0, 0, 0 );
  layer->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );

  QVBoxLayout* vLayout = new QVBoxLayout ( layer );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  m_domTree = new DomTree ( layer );
  vLayout->addWidget ( m_domTree );

  QHBoxLayout* hLayout = new QHBoxLayout;
  m_domToolBar = new DomToolBar ( layer );
  hLayout->addWidget ( m_domToolBar, 0, Qt::AlignLeft );

  sizeInfo = new QLabel ( layer );
  hLayout->addWidget ( sizeInfo );

  vLayout->addLayout ( hLayout );

  layer->setLayout ( vLayout );
  m_domSplitter->insertWidget ( 0, layer );

  m_listStyleSheet = new ListStyleSheet ( m_domSplitter, cfg );
  m_domSplitter->insertWidget ( 1, m_listStyleSheet );

  m_domSplitter->setStretchFactor ( 0, 1 );
  m_domSplitter->setCollapsible ( 0, false );
  m_domSplitter->setCollapsible ( 1, true );

  setWidget ( m_domSplitter );

  connect ( m_domTree, SIGNAL ( elementRect ( const QString &, const QRect & ) ),
            this, SLOT ( setSizeInfo ( const QString &, const QRect & ) ) );

  connect ( m_domTree, SIGNAL ( itemHighlight ( const QWebElement & ) ),
            this, SLOT ( setElementVisible ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( initStyleSheet ( const QWebElement & ) ),
            m_listStyleSheet, SLOT ( setStyleSheetList ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( errorMessage ( const QString & ) ),
            this, SIGNAL ( errorMessage ( const QString & ) ) );

  connect ( m_domToolBar, SIGNAL ( prune() ), m_domTree, SLOT ( setPrune() ) );
  connect ( m_domToolBar, SIGNAL ( expand() ), m_domTree, SLOT ( expandAll() ) );
  connect ( m_domToolBar, SIGNAL ( unselect() ), m_domTree, SLOT ( setUnselect() ) );
}

/**
* Versuche hier bei dem WebElement heraus zu finden ob nicht
* schon (z.B: fieldset) ein Rahmen enthalten ist.
*/
bool DomInspector::hasBorderStyleSheet ( const QWebElement &element ) const
{
  if ( element.tagName().contains ( "fieldset", Qt::CaseInsensitive ) )
    return true;

  QStringList styles ( "solid" );
  styles << "dashed" << "dotted" << "double" << "groove" << "ridge" << "inset" << "outset";
  QString pattern = QString ( "\\b(%1)\\b" ).arg ( styles.join ( "|" ) );

  QStringList cssAttributes ( "border-color" );
  cssAttributes << "border-style" << "border";
  foreach ( QString attr, cssAttributes )
  {
    QString value = element.styleProperty ( attr, QWebElement::CascadedStyle );
    if ( ! value.isEmpty() )
      return value.contains ( QRegExp ( pattern, Qt::CaseInsensitive ) );
  }
  return false;
}

/**
* Suche im aktuellen Elementenblock HEAD nach Links zu externen StyleSheet
* Definitionen. Wenn gefunden, dann das Prädikat "href" auflösen und
* in die Stringliste schreiben. Die Sortierung bei mehrfachen
* verweisen liegt bei der Klasse die alle Daten entgegen nimmt.
*/
const QStringList DomInspector::foundStylesheetReferences ( const QWebElement &head ) const
{
  QStringList list;
  foreach ( QWebElement link, head.findAll ( "LINK[rel~=stylesheet]" ) )
  {
    if ( link.hasAttribute ( QLatin1String ( "href" ) ) )
    {
      QString cssPath = link.attribute ( "href", QString::null );
      if ( cssPath.isEmpty() )
        continue;

      // qDebug() << Q_FUNC_INFO << cssPath;
      list << cssPath;
    }
  }
  return list;
}

void DomInspector::setSizeInfo ( const QString &name, const QRect &rect )
{
  sizeInfo->clear();
  QSize size = rect.size();
  if ( size.width() < 1 || size.height() < 1 )
    return;

  QString info ( "Element : <b>&lt;" );
  info.append ( name );
  info.append ( "&gt;</b> " );
  info.append ( i18n ( "Position" ) );
  info.append ( " : " );
  info.append ( QString::number ( rect.x() ) );
  info.append ( "," );
  info.append ( QString::number ( rect.y() ) );

  info.append ( " " );
  info.append ( i18n ( "Size" ) );
  info.append ( " : " );
  info.append ( QString::number ( size.width() ) );
  info.append ( "x" );
  info.append ( QString::number ( size.height() ) );

  sizeInfo->setText ( info );
}

/**
* In dieser Methode wird die Hervorhebung für den Browser erstellt.
* Mit QWebElement::setStyleProperty wird je nach Möglichkeit der Stylesheet verändert.
* @li background-color @code  background-color: yellow !important; @endcode
* @note Es ist Wichtig das "!important" gesetzt ist sonst Ignoriert WebKit die angaben.
*
* @note Hier gibt es einige Probleme was das Überschreiben betrifft!
* 1) Hat ein Element schon einen Rahmen und kann QWebElement diesen finden!
*    Bestes Beispiel ist das "fieldset" Element.
* 2) Wenn ich hier hingehe und ein WebElement Neu Schreibe dann werden dieses
*    in @class DomTree wegen der neuen Speicher Adresse nicht mehr gefunden.
*    Es kann auch keine Kopie eines WebElementes z.B. in einem struct zwischen
*    gelagert werden weil sonst der Pointer auf das Element verloren geht.
*    Klar - Ich könnte jetzt auch ein Vector mit den Pointern befüllen
*    aber mal ehrlich, das ist in meinen Augen sinnloses Speicherfressen.
*
* Aus diesem Grund habe ich mich nun so entschieden.
* Bei Änderungen kopiere die Informationen in das QList @ref lastSelections
* und halte immer das zuletzt verwendete Element in diesem struct fest.
* Wenn ein erneuter Aufruf dieser Methode erfolgt erst nachsehen ob in
* @ref lastSelections Inhalte enthalten sind, welche noch bereinigt werden müssen.
*/
void DomInspector::setElementVisible ( const QWebElement &element )
{
  bool background = cfg->value ( QLatin1String ( "enableHighlightBackground" ), true ).toBool();
  bool border = cfg->value ( QLatin1String ( "enableHighlightBorder" ), true ).toBool();
  QString highlightColor = cfg->value ( QLatin1String ( "highlightColor" ), QLatin1String ( "yellow" ) ).toString();
  QString backgroundStyle = QString ( "%1 !important" ).arg ( highlightColor );

  if ( lastSelections.size() != 0 )
  {
    if ( lastSelections.first().background )
      lastSelections.first().element.setStyleProperty ( QString::fromUtf8 ( "background-color" ), "" );

    lastSelections.removeFirst();
  }

  /* Hole für das Element den dazu gehörigen QWebFrame.
  * Wenn gefunden und bei einem anklicken das Element nicht sichtbar ist.
  * Springe mit dem slot setScrollPosition an den Seiten Kopf. */
  QRect elementRect = element.geometry();

  // BorderFrame anzeigen
  if ( m_paintElement && border )
  {
    m_paintElement->setGeometry ( elementRect );
    m_paintElement->show();
  }

  if ( elementRect.isValid() && element.webFrame() )
  {
    QWebFrame* currentFrame = element.webFrame();
    QRect topRect = currentFrame->geometry();
    topRect.setTop ( currentFrame->scrollPosition().y() );
    if ( ! topRect.contains ( elementRect ) )
      currentFrame->setScrollPosition ( elementRect.topLeft() );
  }

  QWebElement ele ( element );
  SelectedItem selection;
  selection.element = ele;
  if ( background )
  {
    selection.background = true;
    ele.setStyleProperty ( QString::fromUtf8 ( "background-color" ), backgroundStyle );
  }
  else
    selection.background = false;

  lastSelections << selection;
}

/**
* Dieser slot ruft die Methode @ref DomTree::setDomTree auf.
* Gleichzeitig werden Inhalte aus @class ListStyleSheet entfernt.
* Des weiteren wird hier nach LINK Elementen gesucht welche auf
* externe CSS Referenzen verweisen, bei erfolg werden diese mit
* dem Signal @ref cascadedStylesHref weiter gereicht.
*/
void DomInspector::setDomTree ( const QUrl &url, const QWebElement &element )
{
  if ( ! element.webFrame() )
    return;

  if ( m_paintElement )
    delete m_paintElement;

  m_paintElement = new PaintElement ( element.webFrame()->page()->view() );
  m_paintElement->setObjectName ( url.toString ( QUrl::RemoveScheme ) );

  m_domTree->setToolTip ( url.toString ( QUrl::RemoveQuery ) );
  m_domTree->setDomTree ( element );

  QStringList css = foundStylesheetReferences ( element.findFirst ( "HEAD" ) );
  if ( css.size() >= 1 )
    emit cascadedStylesHref ( css );

  m_listStyleSheet->clear();
}

/**
* Dieser slot ruft die Methode @ref DomTree::findItem auf.
* Wenn diese true zurück gibt dann Zeige den CSS StyleSheet
* mit @ref ListStyleSheet::setStyleSheetList an.
*/
void DomInspector::findItem ( const QWebElement &element )
{
  if ( m_domTree->findItem ( element ) )
    m_listStyleSheet->setStyleSheetList ( element );
}

DomInspector::~DomInspector()
{
  if ( m_paintElement )
    delete m_paintElement;

  lastSelections.clear();
}
