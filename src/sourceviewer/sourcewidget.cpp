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

#include "sourcewidget.h"
#include "sourceview.h"
#include "listlines.h"

#include <cstdio>

/* QtCore */
// #include <QtCore>
#include <QtCore/QList>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidgetItem>
#include <QtGui/QSizePolicy>
#include <QtGui/QPalette>
#include <QtGui/QFrame>
#include <QtGui/QFont>
#include <QtGui/QScrollBar>

SourceWidget::SourceWidget ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "sourcewidget" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setBackgroundRole ( QPalette::NoRole );
  setContentsMargins ( 0, 0, 0, 0 );
  setWindowTitle ( trUtf8 ( "Source" ) );

  QWidget* layer = new QWidget ( this );

  QFont font ( qApp->font() );
  font.setPointSize ( 12 );
  font.setStyleHint ( QFont::Courier, QFont::PreferDefault );
  font.setFamily ( "Courier" );

  QHBoxLayout* mainLayout = new QHBoxLayout ( layer );
  mainLayout->setContentsMargins ( 0, 0, 0, 0 );
  mainLayout->setSpacing ( 1 );

  m_listLines = new ListLines ( font, layer );
  mainLayout->addWidget ( m_listLines );
  mainLayout->setStretchFactor ( m_listLines, 0 );

  m_sourceView = new SourceView ( font, layer );
  m_sourceView->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
  mainLayout->addWidget ( m_sourceView );
  mainLayout->setStretchFactor ( m_sourceView, 1 );

  setWidget ( layer );

  // Add Line Numbers
  connect ( m_sourceView, SIGNAL ( textChanged ( const QList<QListWidgetItem*> & ) ),
            m_listLines, SLOT ( setItems ( const QList<QListWidgetItem*> & ) ) );

  // a cursor position hase changed
  connect ( m_sourceView, SIGNAL ( lineChanged ( int ) ),
            m_listLines, SLOT ( setCurrentRow ( int ) ) );

  connect ( m_listLines, SIGNAL ( currentRowChanged ( int ) ),
            m_sourceView, SLOT ( setCursorToRow ( int ) ) );

  // Sync ScrollBars
  connect ( m_sourceView->verticalScrollBar(), SIGNAL ( valueChanged ( int ) ),
            m_listLines, SLOT ( setValue ( int ) ) );

  connect ( m_listLines, SIGNAL ( valueChanged ( int ) ),
            m_sourceView->verticalScrollBar(), SLOT ( setValue ( int ) ) );

}

void SourceWidget::setSource ( const QString &source )
{
  m_sourceView->setSource ( source );
}

void SourceWidget::check()
{
  m_sourceView->checkSource();
}

void SourceWidget::format()
{
  m_sourceView->formatSource();
}

SourceWidget::~SourceWidget()
{}
