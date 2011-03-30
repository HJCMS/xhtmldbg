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

#include "formtabledelegation.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QStyleOptionViewItem>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

FormTableDelegation::FormTableDelegation ( QTableView * parent )
    : QItemDelegate ( parent )
{
  setObjectName ( QLatin1String ( "FormTableDelegation" ) );
}

QWidget* FormTableDelegation::createEditor ( QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  if ( index.column() == 1 )
    return new QLabel ( editor );
  else
    return new QLineEdit ( editor );
}

void FormTableDelegation::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
  if ( index.column() > 0 ) // TODO Element TagName ist Gesperrt!
  {
    QLineEdit* sw = static_cast<QLineEdit*> ( editor );
    sw->setText ( index.model()->data ( index ).toString() );
  }
}

void FormTableDelegation::setModelData ( QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index ) const
{
  if ( index.column() > 0 ) // TODO Element TagName ist Gesperrt!
  {
    QLineEdit* sw = static_cast<QLineEdit*> ( editor );
    model->setData ( index, QVariant ( sw->text() ), Qt::EditRole );
  }
}

void FormTableDelegation::updateEditorGeometry ( QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index ) const
{
  Q_UNUSED ( index );
  editor->setGeometry ( option.rect );
  editor->setContentsMargins ( 0, 0, 0, 0 );
}

FormTableDelegation::~FormTableDelegation()
{}
