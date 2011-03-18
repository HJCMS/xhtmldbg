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

#include "resizeportbuttons.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ResizePortButtons::ResizePortButtons ( QWidget * parent )
    : QWidget ( parent )
    , icon ( KIcon ( "measure" ) ) //select-rectangular
    , m_desktopWidget ( qApp->desktop() )
{
  setObjectName ( QLatin1String ( "ResizePortButtons" ) );
  setWindowTitle ( i18n ( "Viewer Width" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  m_signalMapper =  new QSignalMapper ( this );
  m_signalMapper->setObjectName ( QLatin1String ( "resizeportbuttons.mapper" ) );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "resizeportbuttons.layout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  m_ToolButton = new QToolButton ( this );
  m_ToolButton->setObjectName ( QLatin1String ( "resizeportbuttons.layout.toolbutton" ) );
  m_ToolButton->setStatusTip ( i18n ( "Resize Browser to fixed width" ) );
  m_ToolButton->setCheckable ( false );
  m_ToolButton->setAutoRaise ( true );
  m_ToolButton->setPopupMode ( QToolButton::InstantPopup );
  m_ToolButton->setIcon ( icon );
  layout->addWidget ( m_ToolButton );

  m_menu = new QMenu ( m_ToolButton );
  m_menu->setObjectName ( QLatin1String ( "resizeportbuttons.layout.toolbutton.menu" ) );
  m_ToolButton->setMenu ( m_menu );

  setLayout ( layout );

  connect ( m_signalMapper, SIGNAL ( mapped ( int ) ),
            this, SIGNAL ( itemClicked ( int ) ) );

  connect ( m_desktopWidget, SIGNAL ( resized ( int ) ),
            this, SLOT ( createSelections ( int ) ) );

  connect ( m_desktopWidget, SIGNAL ( workAreaResized ( int ) ),
            this, SLOT ( createSelections ( int ) ) );

  createSelections ( m_desktopWidget->primaryScreen() );
}

/**
* Nehme eine Standard Liste von Desktop Auflösungsbreiten und füge diese.
* Wenn \b kleiner als die aktuelle Browser Auflösung in die Auswahl ein!
*/
void ResizePortButtons::createSelections ( int screen )
{
  QList<int> list;
  int maxWidth = m_desktopWidget->screenGeometry ( screen ).width();
  list << 600 << 800 << 1024 << 1152 << 1280 << 1360 << 1600 << 1920;

  m_menu->clear();

  // Falls der Benutzer die Bildschirm auflösung ändert, erst mal alles zurück setzen.
  emit itemClicked ( 0 );

  QAction* ac = m_menu->addAction ( icon, i18n ( "Restore" ) );
  connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
  m_signalMapper->setMapping ( ac, 0 );

  for ( int i = 0; i < list.size(); ++i )
  {
    int value = list.at ( i );
    if ( value < maxWidth )
    {
      QAction* ac = m_menu->addAction ( icon, QString::number ( value ) );
      connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
      m_signalMapper->setMapping ( ac, value );
    }
  }
}

ResizePortButtons::~ResizePortButtons()
{
  m_menu->clear();
}
