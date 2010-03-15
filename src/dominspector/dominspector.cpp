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

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

/* QtWebKit */
#include <QtWebKit/QWebPage>

DomInspector::DomInspector ( const QString &highlightColor, QWidget * parent )
    : QDockWidget ( parent )
    , highlight ( highlightColor )
{
  setObjectName ( QLatin1String ( "domviewerdock" ) );
  setWindowTitle ( trUtf8 ( "DOM" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  m_domTree = new DomTree ( this );

  setWidget ( m_domTree );

  connect ( m_domTree, SIGNAL ( itemClicked ( const QWebElement & ) ),
            this, SLOT ( setVisible ( const QWebElement & ) ) );
}

const QString DomInspector::javascript ( const QString &filename )
{
  QString js ( QLatin1String ( "(alert('Script Engine Failure');)" ) );
  QFile file ( QString ( ":/scripts/%1.js" ).arg ( filename ) );
  if ( file.open ( QFile::ReadOnly ) )
  {
    js = file.readAll();
    file.close();
    return js;
  }
  return js;
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
}

DomInspector::~DomInspector()
{
}

