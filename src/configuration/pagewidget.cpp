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
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

PageWidget::PageWidget ( const QString &title, QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "config_page_widget" ) );
  setContentsMargins ( 2, 5, 2, 5 );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "config_page_widget_layout" ) );
  verticalLayout->setContentsMargins ( 0, 0, 0, 0 );
  verticalLayout->setSpacing ( 0 );

  m_groupBox = new QGroupBox ( title, this );
  m_groupBox->setObjectName ( QLatin1String ( "config_page_widget_group_box" ) );
  m_groupBox->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  m_groupBox->setContentsMargins ( 0, 0, 0, 0 );
  m_groupBox->setFlat ( true );
  verticalLayout->addWidget ( m_groupBox );

  QVBoxLayout* verticalBoxLayout = new QVBoxLayout ( m_groupBox );
  verticalBoxLayout->setObjectName ( QLatin1String ( "config_page_widget_box_layout" ) );
  verticalBoxLayout->setContentsMargins ( 5, 5, 5, 5 );
  verticalBoxLayout->setSpacing ( 5 );

  centralWidget = new QWidget ( m_groupBox );
  centralWidget->setObjectName ( QLatin1String ( "config_page_widget_central_widget" ) );
  verticalBoxLayout->addWidget ( centralWidget );

  QSpacerItem* vSpacer = new QSpacerItem ( 2, 2, QSizePolicy::Preferred, QSizePolicy::Expanding );
  verticalLayout->addSpacerItem ( vSpacer );

  m_labelBottom = new QLabel ( this );
  m_labelBottom->setObjectName ( QLatin1String ( "config_page_widget_label_bottom" ) );
  verticalLayout->addWidget ( m_labelBottom );

  QSpacerItem* hSpacer = new QSpacerItem ( 2, 2, QSizePolicy::Expanding, QSizePolicy::Preferred );
  verticalLayout->addSpacerItem ( hSpacer );

  setLayout ( verticalLayout );
}

void PageWidget::setNotice ( bool b )
{
  if ( b )
    m_labelBottom->setText ( trUtf8 ( "Application restart required." ) );
}

void PageWidget::setCheckable ( bool b )
{
  m_groupBox->setCheckable ( b );
}

bool PageWidget::isChecked ()
{
  if ( m_groupBox->isCheckable() )
    return m_groupBox->isChecked();
  else
    return false;
}

void PageWidget::setChecked ( bool b )
{
  if ( m_groupBox->isCheckable() )
    m_groupBox->setChecked ( b );
}

PageWidget::~PageWidget()
{}
