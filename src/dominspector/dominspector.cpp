/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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
#include "domtree.h"
#include "liststylesheet.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>

/* QtWebKit */
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

  m_splitter = new QSplitter ( Qt::Vertical, this );
  m_splitter->setObjectName ( QLatin1String ( "domsplitter" ) );

  m_domTree = new DomTree ( m_splitter );
  m_splitter->insertWidget ( 0, m_domTree );

  m_listStyleSheet = new ListStyleSheet ( m_splitter );
  m_splitter->insertWidget ( 1, m_listStyleSheet );

  m_splitter->setStretchFactor ( 0, 1 );
  setWidget ( m_splitter );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            this, SLOT ( setVisible ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            m_listStyleSheet, SLOT ( setStyleSheetList ( const QWebElement & ) ) );
}

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

void DomInspector::setVisible ( const QWebElement &element )
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

void DomInspector::setDomTree ( const QWebElement &element )
{
  if ( ! element.webFrame() )
    return;

  m_domTree->setDomTree ( element );
  m_listStyleSheet->clear();
}

void DomInspector::findItem ( const QWebElement &element )
{
  if ( m_domTree->findItem ( element ) )
    m_listStyleSheet->setStyleSheetList ( element );
}

DomInspector::~DomInspector()
{
}

