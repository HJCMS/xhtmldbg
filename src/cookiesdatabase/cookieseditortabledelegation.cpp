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

#include "cookieseditortabledelegation.h"
#include "cookieaccesscombobox.h"
#include "cookiesboolcombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QStyleOption>

CookiesEditorTableDelegation::CookiesEditorTableDelegation ( QTableView * parent )
    : QItemDelegate ( parent )
    , m_tableView ( parent )
{}

/**
* virtuelle Speicherbelegung für die Tabellenzelle
*/
QWidget* CookiesEditorTableDelegation::createEditor ( QWidget * editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  QVariant data = index.model()->data ( index );
  switch ( index.column() )
  {
    case 0:
      return new CookieAccessComboBox ( editor, data.toInt() );

    case 1:
      return new QLineEdit ( data.toString(), editor );

    default:
      return new CookiesBoolComboBox ( editor, data.toInt() );
  }
}

/**
* Bei einem onEnterEdit das passende Widget zu verfügung stellen!
*/
void CookiesEditorTableDelegation::setEditorData ( QWidget * editor, const QModelIndex &index ) const
{
  QVariant data = index.model()->data ( index );
  switch ( index.column() )
  {
    case 0:
    {
      CookieAccessComboBox* widget = static_cast<CookieAccessComboBox*> ( editor );
      widget->setValue ( data.toUInt() );
    }
    break;

    case 1:
    {
      QLineEdit* widget = static_cast<QLineEdit*> ( editor );
      widget->setText ( data.toString() );
    }
    break;

    default:
    {
      CookiesBoolComboBox* widget = static_cast<CookiesBoolComboBox*> ( editor );
      widget->setValue ( data.toUInt() );
    }
    break;
  }
}

/**
* Beim verlassen des Editieren den Datensatz aufs Model übertragen.
*/
void CookiesEditorTableDelegation::setModelData ( QWidget * editor,
        QAbstractItemModel* model, const QModelIndex &index ) const
{
  QVariant data = index.model()->data ( index );
  switch ( index.column() )
  {
    case 0:
    {
      CookieAccessComboBox* widget = static_cast<CookieAccessComboBox*> ( editor );
      model->setData ( index, widget->value(), Qt::DisplayRole );
    }
    break;

    case 1:
    {
      QLineEdit* widget = static_cast<QLineEdit*> ( editor );
      model->setData ( index, widget->text(), Qt::DisplayRole );
    }
    break;

    default:
    {
      CookiesBoolComboBox* widget = static_cast<CookiesBoolComboBox*> ( editor );
      model->setData ( index, widget->value(), Qt::DisplayRole );
    }
    break;
  }
}

/**
* Diese Virtuelle Methode wird aufgerufen wenn ein Doppelklick
* auf einen Datensatz erfolgt und das Editieren gestartet wird.
*/
void CookiesEditorTableDelegation::updateEditorGeometry ( QWidget* editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( index );
  editor->setGeometry ( option.rect );
  editor->setContentsMargins ( 0, 0, 0, 0 );
}

CookiesEditorTableDelegation::~CookiesEditorTableDelegation()
{}

