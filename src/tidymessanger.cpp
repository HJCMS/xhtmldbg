/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "tidymessanger.h"

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

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

TidyMessanger::TidyMessanger ( QWidget * parent )
    : QDockWidget ( parent )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconCritical ( QString::fromUtf8 ( ":/icons/critical.png" ) )
{
  setObjectName ( "tidymessanger" );
  setWindowTitle ( i18n ( "HTML/XHTML Impartations" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( true );
  m_listWidget->sortItems ( Qt::AscendingOrder );
  m_listWidget->setSelectionMode ( QAbstractItemView::SingleSelection );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  m_listWidget->setBackgroundRole ( QPalette::AlternateBase );
  m_listWidget->setAlternatingRowColors ( true );

  setWidget ( m_listWidget );

  connect ( m_listWidget, SIGNAL ( itemClicked ( QListWidgetItem * ) ),
            this, SLOT ( pretended ( QListWidgetItem * ) ) );
}

void TidyMessanger::pretended ( QListWidgetItem * item )
{
  QList<QVariant> data = item->data ( Qt::UserRole ).toList();
  if ( data.size() <= 0 )
    return;

  int row = ( data.at ( 0 ).toInt() - 1 );
  int col = data.at ( 1 ).toInt();
  emit marking ( ( ( row >= 0 ) ? row : 0 ), col );
  emit itemSelected();
}

void TidyMessanger::sortAscending()
{
  m_listWidget->sortItems ( Qt::AscendingOrder );
}

void TidyMessanger::sortDescending()
{
  m_listWidget->sortItems ( Qt::DescendingOrder );
}

void TidyMessanger::contextMenuEvent ( QContextMenuEvent *e )
{
  QMenu* menu = new QMenu ( this );

  QAction* ac_Ascending = menu->addAction ( KIcon ( QLatin1String ( "view-sort-ascending" ) ),
                          i18n ( "Ascending" ) );
  connect ( ac_Ascending, SIGNAL ( triggered() ), this, SLOT ( sortAscending() ) );

  QAction* ac_Descending = menu->addAction ( KIcon ( QLatin1String ( "view-sort-descending" ) ),
                           i18n ( "Descending" ) );
  connect ( ac_Descending, SIGNAL ( triggered() ), this, SLOT ( sortDescending() ) );

  QAction* ac_clear = menu->addAction ( KIcon ( QLatin1String ( "edit-clear" ) ),
                                        i18n ( "Clear" ) );
  connect ( ac_clear, SIGNAL ( triggered() ), this, SLOT ( clearItems() ) );

  menu->exec ( e->globalPos() );
}

void TidyMessanger::messages ( const QTidy::QTidyDiagnosis &d )
{
  QString IconName;
  QList<QVariant> pair;
  pair << d.line << d.column;

  switch ( d.level )
  {
    case TidyInfo:
      IconName = "notice";
      break;

    case TidyWarning:
      IconName = "warning";
      break;

    case TidyConfig:
      IconName = "warning";
      break;

    case TidyAccess:
      IconName = "notice";
      break;

    case TidyError:
      IconName = "critical";
      break;

    case TidyBadDocument:
      IconName = "critical";
      break;

    case TidyFatal:
      IconName = "critical";
      break;

    default:
      IconName = "notice";
      break;
  }

  QIcon qIcon ( QString::fromUtf8 ( ":/icons/%1.png" ).arg ( IconName ) );
  QListWidgetItem* item = new QListWidgetItem ( qIcon, d.message, m_listWidget );
  item->setData ( Qt::UserRole, pair );
  m_listWidget->addItem ( item );
  emit itemsChanged ( true );
}

void TidyMessanger::messages ( int l, const QString &m )
{
  QList<QVariant> pair;
  pair << l << 0;

  QIcon qIcon ( QString::fromUtf8 ( ":/icons/warning.png" ) );
  QListWidgetItem* item = new QListWidgetItem ( qIcon, m, m_listWidget );
  item->setData ( Qt::UserRole, pair );
  m_listWidget->addItem ( item );
  emit itemsChanged ( true );
}

void TidyMessanger::clearItems()
{
  m_listWidget->clear();
  emit itemsChanged ( false );
}

TidyMessanger::~TidyMessanger()
{
}
