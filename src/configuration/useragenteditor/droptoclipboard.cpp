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

#include "droptoclipboard.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

/* KDE */
#include <KDE/KLocale>

DropToClipBoard::DropToClipBoard ( QWidget * parent, QSize maxSize )
    : QWidget ( parent )
    , baseSize ( maxSize )
{
  setObjectName ( QLatin1String ( "droptoclipboard" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMouseTracking ( true );
  setAcceptDrops ( true );

  QHBoxLayout* layout = new QHBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  QLabel* label = new QLabel ( this );
  label->setToolTip ( i18n ( "Copy to Clipboard Drop Area" ) );
  label->setWhatsThis ( i18n ( "Copy to Clipboard Drop Area" ) );
  QIcon icon = QIcon::fromTheme ( QLatin1String ( "edit-copy" ) );
  label->setPixmap ( icon.pixmap ( baseSize, QIcon::Normal, QIcon::On ) );
  layout->addWidget ( label );
  setLayout ( layout );
}

void DropToClipBoard::dragEnterEvent ( QDragEnterEvent *e )
{
  if ( e->mimeData()->hasFormat ( "text/plain" ) )
    e->acceptProposedAction();
}

void DropToClipBoard::dragMoveEvent ( QDragMoveEvent *e )
{
  if ( e->mimeData()->hasFormat ( "text/plain" ) )
  {
    e->setDropAction ( Qt::MoveAction );
    e->accept();
  }
}

void DropToClipBoard::dropEvent ( QDropEvent *e )
{
  if ( e->mimeData()->text().isEmpty() )
    return;

  QApplication::clipboard()->setText ( e->mimeData()->text() );
  e->acceptProposedAction();
}

DropToClipBoard::~DropToClipBoard()
{}
