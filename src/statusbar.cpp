/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "statusbar.h"

#include <climits>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

StatusBar::StatusBar ( KStatusBar * parent )
    : KStatusBar ( parent )
{
  setObjectName ( QLatin1String ( "statusbar" ) );
  setContentsMargins ( 2, 5, 2, 2 );

  m_refreshLabel = new QLabel ( this );
  m_refreshLabel->setObjectName ( QLatin1String ( "refreshlabel" ) );
  m_refreshLabel->setFrameShape ( QFrame::NoFrame );
  m_refreshLabel->setContentsMargins ( 5, 2, 5, 2 );
  m_refreshLabel->setToolTip ( i18n ( "Reload Status" ) );
  m_refreshLabel->setMinimumWidth ( 26 );
  m_refreshLabel->setAlignment ( Qt::AlignCenter );
  m_refreshLabel->setStyleSheet ( "background-image:url(':/icons/clock.png');background-position:center;background-repeat:no-repeat;" );
  m_refreshLabel->setEnabled ( false );
  insertPermanentWidget ( 0, m_refreshLabel );

  // Display Notifications
  m_noticeLabel = new QLabel ( this );
  m_noticeLabel->setObjectName ( QLatin1String ( "noticelabel" ) );
  m_noticeLabel->setFrameShape ( QFrame::NoFrame );
  m_noticeLabel->setContentsMargins ( 5, 2, 5, 2 );
  QIcon noticeIcon ( QString::fromUtf8 ( ":/icons/notice.png" ) );
  m_noticeLabel->setPixmap ( noticeIcon.pixmap ( 16, QIcon::Normal, QIcon::On ) );
  m_noticeLabel->setToolTip ( i18n ( "QTidy Messanger contains Impartations" ) );
  m_noticeLabel->setEnabled ( false );
  insertPermanentWidget ( 1, m_noticeLabel );

  // Display Current Pagse Size
  m_viewPageSize = new QLabel ( this );
  m_viewPageSize->setObjectName ( QLatin1String ( "viewpagesizelabel" ) );
  m_viewPageSize->setFrameShape ( QFrame::NoFrame );
  m_viewPageSize->setContentsMargins ( 5, 2, 5, 2 );
  m_viewPageSize->setText ( QLatin1String ( "Bytes" ) );
  m_viewPageSize->setToolTip ( i18n ( "the rendered page size" ) );
  insertPermanentWidget ( 2, m_viewPageSize );

  // Display Browser ViewPort Width
  m_viewPortInfo = new QLabel ( this );
  m_viewPortInfo->setObjectName ( QLatin1String ( "viewportinfolabel" ) );
  m_viewPortInfo->setFrameShape ( QFrame::NoFrame );
  m_viewPortInfo->setContentsMargins ( 5, 2, 5, 2 );
  m_viewPortInfo->setToolTip ( i18n ( "Display Browser Dimension Width x Height with Pixel." ) );
  insertPermanentWidget ( 3, m_viewPortInfo );

  reformat();
}

/**
* Zeigt Informationen über den Aktuellen Status der
* Seiten Aktualisierung an.
*/
void StatusBar::timerStatus ( int max, int sek )
{
  if ( max == 0 )
  {
    m_refreshLabel->setText ( QString::null );
    return;
  }

  QString status;
  status.sprintf ( "%02d", ( max - sek ) );
  m_refreshLabel->setText ( status );
}

/**
* Wenn mit @ref TidyMessanger::itemsChanged Nachrichten eingehen
* diese hier hervorheben.
*/
void StatusBar::notice ( bool notice )
{
  m_noticeLabel->setEnabled ( notice );
}

/**
* Wird von @ref WebViewer::bytesLoaded aufgerufen und
* Übermittelt die Anzahl der geladenen Bytes der
* aktuellen Seite.
*/
void StatusBar::setLoadedPageSize ( qint64 bytes )
{
  QString out;
  if ( bytes > 1024 )
  {
    out = QString::number ( ( bytes / 1024 ) );
    out.append ( " kB" );
    m_viewPageSize->setText ( out );
  }
  else
  {
    out = QString::number ( ( bytes ) );
    out.append ( " Bytes" );
    m_viewPageSize->setText ( out );
  }
}

/**
* Wird von @ref Window::paintEvent aufgerufen und
* übermittelt die aktuelle Fenstergröße des Browsers.
*/
void StatusBar::displayBrowserWidth ( const QSize &s )
{
  QString w = QString::number ( s.width() );
  QString h = QString::number ( s.height() );
  QString txt = i18n ( "Browser %1x%2 px" ).arg ( w, h );
  m_viewPortInfo->setText ( txt );
}

StatusBar::~StatusBar()
{}
