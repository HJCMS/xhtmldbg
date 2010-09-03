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

#include "configdomtree.h"
#include "configutils.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

ConfigDomTree::ConfigDomTree ( QWidget * parent )
    : PageWidget ( trUtf8 ( "DOM Inspector" ), parent )
    , highlightBackgroundColor ( QLatin1String ( "yellow" ) )
    , highlightBorderColor ( QLatin1String ( "red" ) )
{
  setObjectName ( QLatin1String ( "config_page_domtree" ) );
  setNotice ( false );
  setCheckable ( false );

  QIcon iconChooser = QIcon::fromTheme ( QLatin1String ( "kcolorchooser" ) );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_domtree_vertical_layout" ) );

  m_backgroundColorGroup = new QGroupBox ( trUtf8 ( "Enable Background Highlight" ), centralWidget );
  m_backgroundColorGroup->setObjectName ( QLatin1String ( "config_page_domtree_group_background" ) );
  m_backgroundColorGroup->setFlat ( true );
  m_backgroundColorGroup->setCheckable ( true );
  verticalLayout->addWidget ( m_backgroundColorGroup );

  QHBoxLayout* hLayoutOne = new QHBoxLayout ( m_backgroundColorGroup );

  previewBackground = new QLabel ( m_backgroundColorGroup );
  previewBackground->setMinimumSize ( QSize ( 30, 5 ) );
  previewBackground->setFrameShape ( QFrame::Panel );
  previewBackground->setAlignment ( Qt::AlignCenter );
  hLayoutOne->addWidget ( previewBackground );

  QToolButton* btnBackground = new QToolButton ( m_backgroundColorGroup );
  btnBackground->setIcon ( iconChooser );
  hLayoutOne->addWidget ( btnBackground );

  m_backgroundColorGroup->setLayout ( hLayoutOne );

  m_borderColorGroup = new QGroupBox ( trUtf8 ( "Enable Border Highlight" ), centralWidget );
  m_borderColorGroup->setObjectName ( QLatin1String ( "config_page_domtree_group_border" ) );
  m_borderColorGroup->setCheckable ( true );
  m_borderColorGroup->setFlat ( true );
  verticalLayout->addWidget ( m_borderColorGroup );

  QHBoxLayout* hLayoutTwo = new QHBoxLayout ( m_borderColorGroup );

  previewBorder = new QLabel ( m_borderColorGroup );
  previewBorder->setMinimumSize ( QSize ( 30, 5 ) );
  previewBorder->setFrameShape ( QFrame::Panel );
  previewBorder->setAlignment ( Qt::AlignCenter );
  hLayoutTwo->addWidget ( previewBorder );

  QToolButton* btnBorder = new QToolButton ( m_borderColorGroup );
  btnBorder->setIcon ( iconChooser );
  hLayoutTwo->addWidget ( btnBorder );

  m_borderColorGroup->setLayout ( hLayoutTwo );

  centralWidget->setLayout ( verticalLayout );

  connect ( btnBackground, SIGNAL ( clicked() ),
            this, SLOT ( setBackgroundColor() ) );

  connect ( btnBorder, SIGNAL ( clicked() ),
            this, SLOT ( setBorderColor() ) );
}

void ConfigDomTree::setBackgroundColor()
{
  highlightBackgroundColor = ConfigUtils::openColorDialog ( highlightBackgroundColor, this );
  previewBackground->setStyleSheet ( QString ( "background-color: %1;" ).arg ( highlightBackgroundColor ) );
}

void ConfigDomTree::setBorderColor()
{
  highlightBorderColor = ConfigUtils::openColorDialog ( highlightBorderColor, this );
  previewBorder->setStyleSheet ( QString ( "background-color: %1;" ).arg ( highlightBorderColor ) );
}

void ConfigDomTree::load ( Settings * cfg )
{
  m_backgroundColorGroup->setChecked ( cfg->value ( QLatin1String ( "enableHighlightBackground" ), true ).toBool() );
  m_borderColorGroup->setChecked ( cfg->value ( QLatin1String ( "enableHighlightBorder" ), false ).toBool() );

  highlightBackgroundColor = cfg->value ( QLatin1String ( "highlightColor" ), highlightBackgroundColor ).toString();
  previewBackground->setStyleSheet ( QString ( "background-color: %1;" ).arg ( highlightBackgroundColor ) );

  highlightBorderColor = cfg->value ( QLatin1String ( "highlightBorder" ), highlightBorderColor ).toString();
  previewBorder->setStyleSheet ( QString ( "background-color: %1;" ).arg ( highlightBorderColor ) );
}

void ConfigDomTree::save ( Settings * cfg )
{
  cfg->setValue ( QLatin1String ( "enableHighlightBackground" ), m_backgroundColorGroup->isChecked() );
  cfg->setValue ( QLatin1String ( "enableHighlightBorder" ), m_borderColorGroup->isChecked() );
  cfg->setValue ( QLatin1String ( "highlightColor" ), highlightBackgroundColor );
  cfg->setValue ( QLatin1String ( "highlightBorder" ), highlightBorderColor );
}

ConfigDomTree::~ConfigDomTree()
{}
