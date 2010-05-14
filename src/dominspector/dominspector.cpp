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

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebPage>

DomInspector::DomInspector ( QWidget * parent, QSettings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
{
  Q_INIT_RESOURCE ( dominspector );
  setObjectName ( QLatin1String ( "domviewerdock" ) );
  setWindowTitle ( trUtf8 ( "DomTree" ) );
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

  m_domToolBar = new DomToolBar ( layer );
  vLayout->addWidget ( m_domToolBar );

  layer->setLayout ( vLayout );
  m_domSplitter->insertWidget ( 0, layer );

  m_listStyleSheet = new ListStyleSheet ( m_domSplitter, cfg );
  m_domSplitter->insertWidget ( 1, m_listStyleSheet );

  m_domSplitter->setStretchFactor ( 0, 1 );
  m_domSplitter->setCollapsible ( 0, false );
  m_domSplitter->setCollapsible ( 1, true );

  setWidget ( m_domSplitter );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            this, SLOT ( setElementVisible ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            m_listStyleSheet, SLOT ( setStyleSheetList ( const QWebElement & ) ) );

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
* In dieser Methode wird die Hervorhebung für den Browser erstellt.
* Mit QWebElement::setStyleProperty wird je nach Möglichkeit der Stylesheet verändert.
* @li background-color @code  background-color: yellow !important; @endcode
* @li border           @code  border: 1px solid red !important; @endcode
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
  QString highlightBorder = cfg->value ( QLatin1String ( "highlightBorder" ), QLatin1String ( "red" ) ).toString();
  QString borderStyle = QString ( "1px solid %1 !important" ).arg ( highlightBorder );

  if ( lastSelections.size() != 0 )
  {
    if ( lastSelections.first().background )
      lastSelections.first().element.setStyleProperty ( QString::fromUtf8 ( "background-color" ), "" );

    if ( lastSelections.first().border )
      lastSelections.first().element.setStyleProperty ( QString::fromUtf8 ( "border" ), "none !important" );

    lastSelections.removeFirst();
  }

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

  if ( border && ! hasBorderStyleSheet ( element ) )
  {
    selection.border = true;
    ele.setStyleProperty ( QString::fromUtf8 ( "border" ), borderStyle );
  }
  else
    selection.border = false;

  lastSelections << selection;
}

/**
* Dieser slot ruft die Methode @ref DomTree::setDomTree auf.
* Gleichzeitig werden Inhalte aus @class ListStyleSheet entfernt.
*/
void DomInspector::setDomTree ( const QWebElement &element )
{
  if ( ! element.webFrame() )
    return;

  m_domTree->setDomTree ( element );
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
}

