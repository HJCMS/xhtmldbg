/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "xdebugprojecttabledelegation.h"
#include "xdebugspinbox.h"
#include "xdebuglineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/**
* @class XDebugProjectTableDelegation
* 
* Diese Klasse stellt die Editieren funktionen zu verfügung
*/
XDebugProjectTableDelegation::XDebugProjectTableDelegation ( QTableView * parent )
    : QItemDelegate ( parent )
    , m_tableView ( parent )
{}

/**
* virtuelle Speicherbelegung für die Tabellenzelle
*/
QWidget* XDebugProjectTableDelegation::createEditor ( QWidget * editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  // QVariant data = index.model()->data ( index );
  if ( index.column() == 1 )
  {
    return new XDebugSpinBox ( editor );
  }
  else
  {
    return new XDebugLineEdit ( editor );
  }
}

/**
* Bei einem onEnterEdit das passende Widget zu verfügung stellen!
*/
void XDebugProjectTableDelegation::setEditorData ( QWidget * editor, const QModelIndex &index ) const
{
  QVariant data = index.model()->data ( index );
  if ( index.column() == 1 )
  {
    XDebugSpinBox* sw = static_cast<XDebugSpinBox*> ( editor );
    sw->setText ( data.toString() );
  }
  else
  {
    XDebugLineEdit* sw = static_cast<XDebugLineEdit*> ( editor );
    sw->setText ( data.toString() );
  }
}

/**
* Beim verlassen des Editieren den Datensatz aufs Model übertragen.
* @see XDebugProjectTableModel::setData
*/
void XDebugProjectTableDelegation::setModelData ( QWidget * editor,
        QAbstractItemModel* model, const QModelIndex &index ) const
{
  QVariant data = index.model()->data ( index );
  if ( index.column() == 1 )
  {
    XDebugSpinBox* sw = static_cast<XDebugSpinBox*> ( editor );
    model->setData ( index, sw->text(), Qt::DisplayRole );
  }
  else
  {
    XDebugLineEdit* sw = static_cast<XDebugLineEdit*> ( editor );
    model->setData ( index, sw->text(), Qt::DisplayRole );
  }
}

/**
* Diese Virtuelle Methode wird aufgerufen wenn ein Doppelklick
* auf einen Datensatz erfolgt und das Editieren gestartet wird.
*/
void XDebugProjectTableDelegation::updateEditorGeometry ( QWidget* editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( index );
  editor->setGeometry ( option.rect );
  editor->setContentsMargins ( 0, 0, 0, 0 );
}

XDebugProjectTableDelegation::~XDebugProjectTableDelegation()
{}
