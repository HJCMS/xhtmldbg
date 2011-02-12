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

#include "pagewidget.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QStyle>

PageWidget::PageWidget ( const QString &title, QWidget * parent )
    : QScrollArea ( parent )
{
  setObjectName ( QLatin1String ( "config_page_widget" ) );
  setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  setFrameStyle ( QFrame::NoFrame );
  setWidgetResizable ( true );
  setContentsMargins ( 0, 0, 0, 0 );
  setStyleSheet ( QLatin1String ( "QLayout{padding: 0px 5px 0px 5px;}" ) );

  QWidget* layerWidget = new QWidget ( this );
  layerWidget->setObjectName ( QLatin1String ( "config_page_scroll_layer_widget" ) );
  layerWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  layerWidget->setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layerWidget );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_widget_layout" ) );
  verticalLayout->setContentsMargins ( 0, 5, 0, 5 );
  verticalLayout->setSpacing ( 5 );

  m_groupBox = new QGroupBox ( title, layerWidget );
  m_groupBox->setObjectName ( QLatin1String ( "config_page_widget_group_box" ) );
  m_groupBox->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  m_groupBox->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
  m_groupBox->setFlat ( true );
  verticalLayout->addWidget ( m_groupBox );

  QVBoxLayout* verticalBoxLayout = new QVBoxLayout ( m_groupBox );
  verticalBoxLayout->setObjectName ( QLatin1String ( "config_page_widget_box_layout" ) );

  centralWidget = new QWidget ( m_groupBox );
  centralWidget->setObjectName ( QLatin1String ( "config_page_widget_central_widget" ) );
  centralWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  verticalBoxLayout->addWidget ( centralWidget );

  verticalBottomSpacer = new QSpacerItem ( 2, 2, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
  verticalBoxLayout->addSpacerItem ( verticalBottomSpacer );

  m_labelBottom = new QLabel ( layerWidget );
  m_labelBottom->setObjectName ( QLatin1String ( "config_page_widget_label_bottom" ) );
  verticalLayout->addWidget ( m_labelBottom );

  QSpacerItem* hSpacer = new QSpacerItem ( 2, 2, QSizePolicy::Expanding, QSizePolicy::Preferred );
  verticalLayout->addSpacerItem ( hSpacer );

  layerWidget->setLayout ( verticalLayout );
  setWidget ( layerWidget );
}

/**
* Hinweis für Programm Neustart setzen
*/
void PageWidget::setNotice ( bool b )
{
  if ( b )
    m_labelBottom->setText ( i18n ( "Application restart required." ) );
}

/**
* Die Gruppe ist Auswählbar setzen
*/
void PageWidget::setCheckable ( bool b )
{
  m_groupBox->setCheckable ( b );
}

/**
* Ist die Gruppe Aktiviert?
*/
bool PageWidget::isActiv()
{
  return m_groupBox->isEnabled();
}

/**
* Ist die Gruppe Auswählbar?
*/
bool PageWidget::isChecked ()
{
  if ( m_groupBox->isCheckable() )
    return m_groupBox->isChecked();
  else
    return false;
}

/**
* Die Auswahl der Gruppe aktivieren
*/
void PageWidget::setActiv ( bool b )
{
  m_groupBox->setEnabled ( b );
}

/**
* Die Auswahl der Gruppe setzen
*/
void PageWidget::setChecked ( bool b )
{
  if ( m_groupBox->isCheckable() )
    m_groupBox->setChecked ( b );
}

/**
* Die Vertikale ausrichtung des letzen Platzhalters im
* vertikalen Bereich neu setzen!
*/
void PageWidget::polishVerticalSpacer ( QSizePolicy::Policy type )
{
  verticalBottomSpacer->changeSize ( 2, 2, QSizePolicy::Preferred, type );
}

PageWidget::~PageWidget()
{}
