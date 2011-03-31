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

#include "liststylesheet.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QColor>
#include <QtGui/QListWidgetItem>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

ListStyleSheet::ListStyleSheet ( QWidget * parent, Settings * settings )
    : QListWidget ( parent )
    , cfg ( settings )
    , cascadedStyle ( QIcon::fromTheme ( QLatin1String ( "code-class" ) ) )
    , inlineStyle ( QIcon::fromTheme ( QLatin1String ( "code-block" ) ) )
{
  setObjectName ( QLatin1String ( "liststylesheet" ) );

  setSortingEnabled ( true );
  setWordWrap ( false );
  sortItems ( Qt::AscendingOrder );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
  setBackgroundRole ( QPalette::AlternateBase );
  setAlternatingRowColors ( true );
  setContextMenuPolicy ( Qt::ActionsContextMenu );
  setMinimumHeight ( 80 );

  ac_copyPredicates = new QAction ( KIcon ( "edit-copy" ), i18n ( "Copy Predicate" ), this );
  ac_copyPredicates->setStatusTip ( i18n ( "copy predicate to clipboard" ) );

  QString strColor = cfg->value ( QLatin1String ( "highlightColor" ), QLatin1String ( "yellow" ) ).toString();
  QColor color ( strColor );
  color.toRgb();
  excludeRgb = QString ( "rgb(%1,%2,%3)" ).arg (
                   QString::number ( color.red() ),
                   QString::number ( color.green() ),
                   QString::number ( color.blue() )
               );

  QFile fp ( QString::fromUtf8 ( ":/css/css.list" ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QString attributesList ( fp.readAll() );
    fp.close();
    foreach ( QString p, attributesList.split ( "\n" ) )
    {
      if ( ! p.isEmpty() )
        cssAttributes.append ( p );
    }
  }

  connect ( ac_copyPredicates, SIGNAL ( triggered () ),
            this, SLOT ( copyPredicate() ) );

  insertAction ( 0, ac_copyPredicates );
}

/** Inhalt in das Clipboard Kopieren */
void ListStyleSheet::copyPredicate ()
{
  if ( currentItem() )
    qApp->clipboard()->setText ( currentItem()->text () );
}

/**
* Wenn das Prädikat ein background* enhält dann dieses mit der
* Hervorhebungs RGB Farbe vergleichen und bei bedarf ausschließen!
*/
bool ListStyleSheet::exclude ( const QString &attr, const QString &value )
{
  if ( attr.contains ( "background" ) )
  {
    QString item = value.trimmed();
    item.remove ( QRegExp ( "\\s" ) );
    return ( item.toLower() == excludeRgb ) ? true : false;
  }
  return false;
}

/**
* Erstelle Eintrag und setze Grafiken, Text und ToolTip zum Inhalts type.
*/
void ListStyleSheet::addStyleItem ( const QString &attr, const QString &value, bool cascaded )
{
  QString display ( attr + ": " + value + ";" );
  QString tip;
  if ( cascaded )
    tip = i18n ( "Rendered from Stylesheet Document" );
  else
    tip = i18n ( "Contained \"style\" Attribute" );

  QListWidgetItem* item = new QListWidgetItem ( this );
  item->setData ( Qt::DisplayRole, display );
  item->setData ( Qt::UserRole, value );
  item->setData ( Qt::ToolTipRole, tip );
  item->setData ( Qt::DecorationRole, ( cascaded ? cascadedStyle : inlineStyle ) );
}

/**
* Suche mit den aus :css.list enthaltenen Prädikaten nach StyleSheet Attributen.
* Dabei werden die im WebElement definierten Attribute vor den StyleSheet gesetzt!
* Also das gleiche Verhalten wir bei Browsern, inline vor cascade.
**/
void ListStyleSheet::setStyleSheetList ( const QWebElement &element )
{
  bool reserved = false;
  clear();

  foreach ( QString attr, cssAttributes )
  {
    if ( element.hasAttribute ( QLatin1String ( "style" ) ) )
    {
      QString val = element.styleProperty ( attr, QWebElement::InlineStyle );
      if ( ! val.isEmpty() )
      {
        reserved = true;
        // exclude Selection background Color from List
        if ( ! exclude ( attr, val ) )
          addStyleItem ( attr, val, false );
      }
    }
    // CSS class Reference
    QString cascaded = element.styleProperty ( attr, QWebElement::CascadedStyle );
    if ( ! cascaded.isEmpty() && ! reserved )
      addStyleItem ( attr, cascaded );

    reserved = false;
  }
}

ListStyleSheet::~ListStyleSheet()
{}
