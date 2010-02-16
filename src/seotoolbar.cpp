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

#include "seotoolbar.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>

SeoToolBar::SeoToolBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "seotoolbar" ) );
  setWindowTitle ( trUtf8 ( "Keywords" ) );
  setStatusTip ( trUtf8 ( "Show Keywords for Search Engine Optimisation (SEO)" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );
  layout()->setSpacing ( 5 );

  QLabel* m_label = new QLabel ( trUtf8 ( "SEO:" ), this );
  m_label->setToolTip ( trUtf8 ( "Search Engine Optimisation (SEO)" ) );
  m_label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( m_label );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setMinimumWidth ( 100 );
  addWidget ( m_lineEdit );

  QAction *cb = addAction ( trUtf8 ( "Clear" ) );

  actionFind = addAction ( trUtf8 ( "Keywords" ) );

  connect ( m_lineEdit, SIGNAL ( returnPressed() ), this, SLOT ( setSignal() ) );
  connect ( cb, SIGNAL ( triggered() ), m_lineEdit, SLOT ( clear() ) );
  connect ( actionFind, SIGNAL ( triggered() ), this, SLOT ( setSignal() ) );
}

void SeoToolBar::setSignal()
{
  QString keyword = m_lineEdit->text();
  if ( keyword.isEmpty() )
    return;

  if ( keyword.contains ( QRegExp ( "[ \\s\\t]+" ) ) )
    emit changed ( keyword.split ( QRegExp ( "[ \\s\\t]+" ) ) );
  else
    emit changed ( keyword );
}

SeoToolBar::~SeoToolBar()
{
}
