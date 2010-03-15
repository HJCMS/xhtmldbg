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

DomInspector::DomInspector ( const QString &highlightColor, QWidget * parent )
    : QDockWidget ( parent )
    , highlight ( highlightColor )
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

  setWidget ( m_splitter );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            this, SLOT ( setVisible ( const QWebElement & ) ) );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            m_listStyleSheet, SLOT ( setStyleSheetList ( const QWebElement & ) ) );
}

void DomInspector::setVisible ( const QWebElement &element )
{
  QString color = QString ( "%1 !important" ).arg ( highlight );
  if ( lastSelections.size() != 0 )
  {
    lastSelections.first().setStyleProperty ( QString::fromUtf8 ( "background-color" ), "" );
    lastSelections.removeFirst();
  }
  QWebElement ele ( element );
  ele.setStyleProperty ( QString::fromUtf8 ( "background-color" ), color );
  ele.setFocus();
  lastSelections << ele;
}

void DomInspector::setDomTree ( const QWebElement &element )
{
  if ( ! element.webFrame() )
    return;

  m_domTree->setDomTree ( element );
  m_listStyleSheet->clear();
}

DomInspector::~DomInspector()
{
}

