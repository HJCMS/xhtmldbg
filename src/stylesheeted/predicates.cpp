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

#include "predicates.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAbstractItemView>

/* KDE */
#include <KDE/KLocale>

Predicates::Predicates ( QWidget * parent )
    : QListWidget ( parent )
    , cascadedStyle ( KIcon ( QLatin1String ( "code-class" ) ) )
    , inlineStyle ( KIcon ( QLatin1String ( "code-block" ) ) )
{
  setObjectName ( QLatin1String ( "PredicatesList" ) );
  setContextMenuPolicy ( Qt::NoContextMenu );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setEditTriggers ( QAbstractItemView::NoEditTriggers );
}

/** Standard QListWidgetItem Konstruktor */
const QListWidgetItem Predicates::constructItem ()
{
  QListWidgetItem item ( this, QListWidgetItem::UserType );
  item.setData ( Qt::DecorationRole, cascadedStyle );
  item.setFlags ( ( item.flags() & ~Qt::ItemIsEditable ) );
  return item;
}

/** TODO Neue Einträge einfügen! */
void Predicates::predicateChanged ( const QString &pa, const QString &va )
{
  Q_UNUSED (pa)
  Q_UNUSED (va)
  // qDebug() << Q_FUNC_INFO << pa << va;
}

/** Alle Stylesheet Daten in die Liste einfügen!
* @note Es werden keine geerbten Attribute eingelesen!
*/
void Predicates::setElement ( const QWebElement &element )
{
  if ( element.hasAttributes() )
  {
    // Wenn class Prädikat vorhanden dann dies mitteilen!
    foreach ( QString cl, element.classes() )
    {
      emit className ( cl );
    }
    // Alle Style Attribute einlesen
    QFile fp ( QString::fromUtf8 ( ":/css/css.list" ) );
    if ( fp.open ( QIODevice::ReadOnly ) )
    {
      QString attributesList ( fp.readAll() );
      fp.close();
      foreach ( QString p, attributesList.split ( "\n" ) )
      {
        if ( p.isEmpty() )
          continue;

        QString value = element.styleProperty ( p, QWebElement::CascadedStyle );
        if ( value.isEmpty() )
          continue;

        QString text ( p );
        text.append ( ":" );
        text.append ( value );
        text.append ( ";" );

        QListWidgetItem* item = new QListWidgetItem ( constructItem () );
        item->setText ( text );
        item->setData ( Qt::UserRole, value );
        addItem ( item );
      }
    }
  }
}

Predicates::~Predicates()
{}
