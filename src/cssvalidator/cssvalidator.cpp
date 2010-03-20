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

#include "cssvalidator.h"
#include "validator.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>

CSSValidator::CSSValidator ( QWidget * parent )
    : QDockWidget ( parent )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
{
  setObjectName ( QLatin1String ( "cssvalidator" ) );
  setWindowTitle ( trUtf8 ( "CSS Validation" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( true );
  m_listWidget->setWordWrap ( false );
  m_listWidget->sortItems ( Qt::AscendingOrder );
  m_listWidget->setSelectionMode ( QAbstractItemView::SingleSelection );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  m_listWidget->setBackgroundRole ( QPalette::AlternateBase );
  m_listWidget->setAlternatingRowColors ( true );

  setWidget ( m_listWidget );
}

void CSSValidator::sortAscending()
{
  m_listWidget->sortItems ( Qt::AscendingOrder );
}

void CSSValidator::sortDescending()
{
  m_listWidget->sortItems ( Qt::DescendingOrder );
}

void CSSValidator::contextMenuEvent ( QContextMenuEvent *e )
{
  QMenu* menu = new QMenu ( this );

  QAction* ac_Ascending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-ascending" ) ),
                          trUtf8 ( "Ascending" ) );
  connect ( ac_Ascending, SIGNAL ( triggered() ), this, SLOT ( sortAscending() ) );

  QAction* ac_Descending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-descending" ) ),
                           trUtf8 ( "Descending" ) );
  connect ( ac_Descending, SIGNAL ( triggered() ), this, SLOT ( sortDescending() ) );

  QAction* ac_clear = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "edit-clear" ) ),
                                        trUtf8 ( "Clear" ) );
  connect ( ac_clear, SIGNAL ( triggered() ), m_listWidget, SLOT ( clear() ) );

  menu->exec ( e->globalPos() );
}

void CSSValidator::runCssCheck ( const QUrl &url )
{
  qDebug() << Q_FUNC_INFO << url.toString();
}

void CSSValidator::clearItems()
{
  m_listWidget->clear();
}

CSSValidator::~CSSValidator()
{}
