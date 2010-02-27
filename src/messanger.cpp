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

#include "messanger.h"

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QListWidgetItem>

Messanger::Messanger ( QWidget * parent )
    : QDockWidget ( parent )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconCritical ( QString::fromUtf8 ( ":/icons/critical.png" ) )
{
  setObjectName ( "messanger" );
  setWindowTitle ( trUtf8 ( "Messages" ) );
  setStatusTip ( trUtf8 ( "Display XHTML/JavaScript and CSS Debugging Messages" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( false );

  QString startupMessage = trUtf8 ( "XHTMLDBG started Normaly. Waiting for Actions..." );
  m_listWidget->addItem ( new QListWidgetItem ( iconNotice, startupMessage ) );

  setWidget ( m_listWidget );

  connect ( m_listWidget, SIGNAL ( itemClicked ( QListWidgetItem * ) ),
            this, SLOT ( pretended ( QListWidgetItem * ) ) );
}

void Messanger::pretended ( QListWidgetItem * item )
{
  QList<QVariant> data = item->data ( Qt::UserRole ).toList();
  if ( data.size() <= 0 )
    return;

  int row = data.at ( 0 ).toInt();
  int col = data.at ( 1 ).toInt();
  int l = ( row - 1 );
  if ( l >= 0 )
    emit marking ( l, col );
}

void Messanger::messages ( const QTidy::QTidyDiagnosis &d )
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
}

Messanger::~Messanger()
{
}
