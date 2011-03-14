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

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRect>
#include <QtCore/QPoint>
#include <QtCore/QStringList>

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

  connect ( m_domTree, SIGNAL ( elementHovered ( const QWebElement & ) ),
            this, SLOT ( setElementInfo ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( itemHighlight ( const QWebElement & ) ),
            this, SLOT ( setElementVisible ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( initStyleSheet ( const QWebElement & ) ),
            m_listStyleSheet, SLOT ( setStyleSheetList ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( errorMessage ( const QString & ) ),
            this, SIGNAL ( errorMessage ( const QString & ) ) );

  connect ( m_domTree, SIGNAL ( unselect () ), this, SLOT ( hideLayer () ) );
  connect ( m_domToolBar, SIGNAL ( prune() ), m_domTree, SLOT ( setPrune() ) );
  connect ( m_domToolBar, SIGNAL ( expand() ), m_domTree, SLOT ( expandAll() ) );
  connect ( m_domToolBar, SIGNAL ( unselect() ), this, SLOT ( hideLayer() ) );
}

/**
* Dies ist der Hover Layer zum anzeigen der WebElemente
* Siehe @ref moveLayer für die Manipulation des Layers
* @li Erstelle mit JabaScript Code eine div Layer
* @li Hänge ihn mit QWebElement::evaluateJavaScript an den BODY an!
* @note Im Moment ist es \b NICHT Möglich mit QWebElement neue Element zu
*       erzeugen und diese in das Dokument einzufügen :-/
*/
void DomInspector::appendLayer ( const QWebElement &element )
{
  QWebElement body = element.findFirst ( "BODY" );
  if ( body.isNull() )
    return;

  QString color = cfg->value ( QLatin1String ( "highlightColor" ), QLatin1String ( "yellow" ) ).toString();
  QString border = cfg->value ( QLatin1String ( "highlightBorder" ), QLatin1String ( "red" ) ).toString();
  QStringList html ( "var div = document.createElement('div');" );
  html << "div.id='xhtmldbg_inspector_layer';";
  html << "div.style.position='absolute';";
  html << "div.style.left='1px';";
  html << "div.style.top='1px';";
  html << "div.style.width='1px';";
  html << "div.style.height='1px';";
  html << QString::fromUtf8 ( "div.style.border='1px solid %1';" ).arg ( border );
  html << QString::fromUtf8 ( "div.style.backgroundColor='%1';" ).arg ( color );
  html << "div.style.zIndex=6;";
  html << "div.style.display='none';";
  html << "this.appendChild(div);";
  /* Leider Verursacht evaluateJavaScript eine Initialisierung der
  * Plugins weil wohl ein evaluateJavaScript bereits in @ref Viewer::displayPlugins
  * vorhanden ist wird dieses dann Anscheinnet auch Initialisiert. ???? */
  body.evaluateJavaScript ( html.join ( "" ) );
}

/**
* Den Layer auf einem Element abbilden
*/
void DomInspector::moveLayer ( const QWebElement &element, bool visible ) const
{
  QRect rect = element.geometry();
  QWebElement layer = element.document().findAll ( "DIV[id~=xhtmldbg_inspector_layer]" ).first();
  if ( !visible || !rect.isValid() )
  {
    layer.setStyleProperty ( "display", "none" );
    layer.setStyleProperty ( "left", "1px" );
    layer.setStyleProperty ( "top", "1px" );
    layer.setStyleProperty ( "width", "1px" );
    layer.setStyleProperty ( "height", "1px" );
    return;
  }
  layer.setStyleProperty ( "display", "inline" );
  layer.setStyleProperty ( "left", QString::number ( rect.x() ) + "px" );
  layer.setStyleProperty ( "top", QString::number ( rect.y() ) + "px" );
  layer.setStyleProperty ( "width", QString::number ( rect.width() - 2 ) + "px" );
  layer.setStyleProperty ( "height", QString::number ( rect.height() - 2 ) + "px" );
  layer.setStyleProperty ( "border-radius", "5px" );
  layer.setStyleProperty ( "opacity", "0.5" );
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

/** Slot zum verbergen des Layers */
void DomInspector::hideLayer ()
{
  if ( currentElement.size() < 1 )
    return;

  moveLayer ( currentElement.last(), false );
}

/** Anzeige der Dimensionen im Label */
void DomInspector::setElementInfo ( const QWebElement &element )
{
  sizeInfo->clear();
  if ( element.isNull() )
    return;

  QString info ( "Element : <b>&lt;" );
  info.append ( element.localName() );
  info.append ( "&gt;</b> " );
  QRect rect = element.geometry();
  if ( rect.isValid() )
  {
    QSize size = rect.size();
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
  }
  else
    info.append ( i18n ( "no visible geometry" ) );

  sizeInfo->setText ( info );
  if ( m_domToolBar->onHoverEnabled() )
    moveLayer ( element );
}

/**
* In dieser Methode wird die Hervorhebung für den Browser erstellt.
* Wenn das Element nicht Sichtbar ist Springe mit setScrollPosition
* Zum Sichtbaren teil des WebElementes. Siehe auch @ref moveLayer
*/
void DomInspector::setElementVisible ( const QWebElement &element )
{
  /* Hole für das Element den dazu gehörigen QWebFrame.
  * Wenn gefunden und bei einem anklicken das Element nicht sichtbar ist.
  * Springe mit dem slot setScrollPosition an den Seiten Kopf. */
  QRect elementRect = element.geometry();
  if ( elementRect.isValid() && element.webFrame() )
  {
    QWebFrame* currentFrame = element.webFrame();
    QRect topRect = currentFrame->geometry();
    topRect.setTop ( currentFrame->scrollPosition().y() );
    if ( ! topRect.contains ( elementRect ) )
      currentFrame->setScrollPosition ( elementRect.topLeft() );
  }
  moveLayer ( element );
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

  currentElement.clear();
  m_domTree->setToolTip ( url.toString ( QUrl::RemoveQuery ) );
  m_domTree->setDomTree ( element );

  QStringList css = foundStylesheetReferences ( element.findFirst ( "HEAD" ) );
  if ( css.size() >= 1 )
    emit cascadedStylesHref ( css );

  appendLayer ( element );
  m_listStyleSheet->clear();
  currentElement.append ( element );
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
  currentElement.clear();
}
