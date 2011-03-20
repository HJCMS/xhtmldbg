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

#include "stylesheeted.h"
#include "colorchooserwidget.h"
#include "actiontoolbar.h"
#include "predicates.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

StyleSheeted::StyleSheeted ( const QWebElement &element, QWidget * parent )
    : KDialog ( parent )
    , origElement ( element.clone() )
    , htmlElement ( element )
{
  setObjectName ( QLatin1String ( "StyleSheeted" ) );
  setCaption ( i18n ( "Edit Element StyleSheet for %1" ).arg ( element.localName() ), false );
  setButtons ( ( KDialog::Ok | KDialog::Cancel ) );

  m_colorChooserWidget = new ColorChooserWidget ( this );
  setMainWidget ( m_colorChooserWidget );

  QWidget* layer = new QWidget ( this );
  QVBoxLayout* verticalLayout = new QVBoxLayout ( layer );

  m_actionToolBar = new ActionToolBar ( layer );
  verticalLayout->addWidget ( m_actionToolBar );

  m_predicates = new Predicates ( layer );
  m_predicates->setElement ( element );
  verticalLayout->addWidget ( m_predicates );

  layer->setLayout ( verticalLayout );

  setDetailsWidget ( layer );
  setDetailsWidgetVisible ( true );

  connect ( m_colorChooserWidget, SIGNAL ( colorSelected ( const QColor & ) ),
            this, SLOT ( colorChanged ( const QColor & ) ) );

  connect ( this, SIGNAL ( rejected () ), this, SLOT ( reset () ) );
}

void StyleSheeted::colorChanged ( const QColor &color )
{
  QString param ( "background-color" );
  QString value ( " !important" );
  value.prepend ( color.name() );

  switch ( m_actionToolBar->whichColor() )
  {
    case ActionToolBar::FOREGROUND:
      param = QLatin1String ( "color" );
      break;

    case ActionToolBar::BORDER:
      param = QLatin1String ( "border-color" );
      break;

    default:
      param = QLatin1String ( "background-color" );
      break;
  };
  htmlElement.setStyleProperty ( param, value );
}

void StyleSheeted::reset()
{
  htmlElement.replace ( origElement );
}

StyleSheeted::~StyleSheeted()
{}
