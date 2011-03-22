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
#include "borderstyle.h"
#include "fontstyle.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QFontInfo>
// #include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

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

  m_tabWidget = new KTabWidget ( this );
  m_tabWidget->setAutomaticResizeTabs ( true );
  m_tabWidget->setCloseButtonEnabled ( false );
  m_tabWidget->setTabPosition ( KTabWidget::South );

  // ColorWidget {
  QWidget* colorWidget = new QWidget ( m_tabWidget );
  colorWidget->setObjectName ( QLatin1String ( "ColorLayerWidget" ) );

  QVBoxLayout* verticalColorLayout = new QVBoxLayout ( colorWidget );

  m_colorChooserWidget = new ColorChooserWidget ( colorWidget );
  verticalColorLayout->addWidget ( m_colorChooserWidget );

  m_actionToolBar = new ActionToolBar ( colorWidget );
  verticalColorLayout->addWidget ( m_actionToolBar, Qt::AlignLeft );

  colorWidget->setLayout ( verticalColorLayout );
  m_tabWidget->insertTab ( 0, colorWidget, i18n ( "Colors" ) );
  m_tabWidget->setTabIcon ( 0, KIcon ( "preferences-desktop-color" ) );
  // } ColorWidget

  // BorderStyle {
  m_borderStyle = new  BorderStyle ( m_tabWidget );
  m_tabWidget->insertTab ( 1, m_borderStyle, i18n ( "Border" ) );
  m_tabWidget->setTabIcon ( 1, KIcon ( "edit-text-frame-update" ) );
  // m_borderStyle->();
  // } BorderStyle

  // FontStyle {
  m_fontStyle = new  FontStyle ( m_tabWidget );
  m_fontStyle->readFontAttributes ( htmlElement );
  m_tabWidget->insertTab ( 2, m_fontStyle, i18n ( "Font" ) );
  m_tabWidget->setTabIcon ( 2, KIcon ( "preferences-desktop-font" ) );
  // } FontStyle

  // Predicates {
  m_predicates = new Predicates ( this );
  m_predicates->setElement ( element );
  setDetailsWidget ( m_predicates );
  setDetailsWidgetVisible ( true );
  // } Predicates

  // add Central Widget
  setMainWidget ( m_tabWidget );

  connect ( m_colorChooserWidget, SIGNAL ( colorSelected ( const QColor & ) ),
            this, SLOT ( colorChanged ( const QColor & ) ) );

  connect ( m_fontStyle, SIGNAL ( fontChanged ( const QFont & ) ),
            this, SLOT ( fontChanged ( const QFont & ) ) );

  connect ( m_fontStyle, SIGNAL ( fontAdjust ( double ) ),
            this, SLOT ( fontSizeAdjust ( double ) ) );

  connect ( m_fontStyle, SIGNAL ( fontWeight ( const QVariant & ) ),
            this, SLOT ( fontWeight ( const QVariant & ) ) );

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

void StyleSheeted::fontChanged ( const QFont &font )
{
  QFontInfo info ( font );
  htmlElement.setStyleProperty ( "font-family", info.family() );
  QString size = QString::fromUtf8 ( "%1px" ).arg ( QString::number ( info.pixelSize() ) );
  htmlElement.setStyleProperty ( "font-size", size );
  if ( info.bold() )
    htmlElement.setStyleProperty ( "font-weight", "bold" );

  htmlElement.setStyleProperty ( "font-style", ( info.italic() ? "italic" : "normal" ) );
}

void StyleSheeted::fontSizeAdjust ( double d )
{
  htmlElement.setStyleProperty ( "font-size-adjust", QString::number ( d ) );
}

void StyleSheeted::fontWeight ( const QVariant &w )
{
  htmlElement.setStyleProperty ( "font-weight", w.toString() );
}

void StyleSheeted::reset()
{
  htmlElement.replace ( origElement );
}

StyleSheeted::~StyleSheeted()
{}
