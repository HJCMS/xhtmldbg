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

#include "nppfailurewidget.h"

/* QtGui */
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KLocale>

NPPFailureWidget::NPPFailureWidget ( const QString &message )
    : QWidget ()
{
  setObjectName ( QLatin1String ( "nppfailurewidget" ) );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "nppfailurewidget/layout" ) );

  QString text ( message );
  text.append ( "\n" );
  text.append ( i18n ( "Plugin initialization aborted." ) );

  m_label = new QLabel ( this );
  m_label->setObjectName ( QLatin1String ( "nppfailurewidget/layout/label" ) );
  m_label->setWordWrap ( true );
  m_label->setText ( text );
  layout->addWidget ( m_label );

  setLayout ( layout );
}

NPPFailureWidget::~NPPFailureWidget()
{
  m_label->clear();
}
