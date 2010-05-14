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

#include "keywordstoolbar.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QIcon>

KeywordsToolBar::KeywordsToolBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "keywordstoolbar" ) );
  setWindowTitle ( trUtf8 ( "Keywords" ) );
  setStatusTip ( trUtf8 ( "Show Keywords for Search Engine Optimisation (SEO)" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );
  layout()->setSpacing ( 5 );

  QIcon icon;

  QLabel* m_label = new QLabel ( trUtf8 ( "SEO:" ), this );
  m_label->setToolTip ( trUtf8 ( "Search Engine Optimisation (SEO)" ) );
  m_label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( m_label );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setMinimumWidth ( 100 );
  addWidget ( m_lineEdit );

  QAction *cb = addAction ( trUtf8 ( "Clear" ) );
  cb->setIcon ( icon.fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );

  actionFind = addAction ( trUtf8 ( "Keywords" ) );
  actionFind->setIcon ( icon.fromTheme ( QLatin1String ( "edit-find" ) ) );

  connect ( m_lineEdit, SIGNAL ( returnPressed() ), this, SLOT ( treating() ) );
  connect ( cb, SIGNAL ( triggered() ), m_lineEdit, SLOT ( clear() ) );
  connect ( actionFind, SIGNAL ( triggered() ), this, SLOT ( treating() ) );
}

void KeywordsToolBar::treating()
{
  QString words = m_lineEdit->text();
  QStringList keywords;
  if ( words.isEmpty() )
    return;

  if ( words.contains ( QRegExp ( "[ \\s\\t]+" ) ) )
    keywords = words.split ( QRegExp ( "[ \\s\\t]+" ) );
  else
    keywords << words;

  emit changed ( keywords );
}

KeywordsToolBar::~KeywordsToolBar()
{
}
