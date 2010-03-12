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

#include "editcookiestable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QItemSelectionModel>

ArrangementItem::ArrangementItem ( const QString &name, int type, QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( name );

  QStringList items;
  items << trUtf8 ( "Session" ) << trUtf8 ( "Blocked" ) << trUtf8 ( "Allowed" );
  addItems ( items );
  setCurrentIndex ( type );

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SIGNAL ( itemChanged() ) );
}

EditCookiesTable::EditCookiesTable ( QWidget * parent )
    : QTableWidget ( parent )
{
  setObjectName ( QLatin1String ( "editcookiestable" ) );
  if ( columnCount () != 2 )
  {
    setColumnCount ( 2 );
    QStringList lb ( trUtf8 ( "Domain" ) );
    lb << trUtf8 ( "Arrangement" );
    setHorizontalHeaderLabels ( lb );
  }
  connect ( this, SIGNAL ( doubleClicked ( const QModelIndex & ) ),
            this, SLOT ( cellChanged ( const QModelIndex & ) ) );
}

void EditCookiesTable::cellChanged ( const QModelIndex &m )
{
  Q_UNUSED ( m )
  emit modified();
}

void EditCookiesTable::loadCookieArrangements ( QSettings * cfg )
{
  Q_ASSERT ( cfg );

  cfg->beginGroup ( QLatin1String ( "CookieArrangement" ) );
  QStringList keys = cfg->allKeys();
  if ( keys.size() >= 1 )
  {
    setRowCount ( keys.size() );
    int r = 0;
    foreach ( QString key, keys )
    {
      int val = cfg->value ( key ).toUInt();
      if ( val < 0 )
        continue;

      // qDebug() << Q_FUNC_INFO << key << val;
      setItem ( r, 0, new QTableWidgetItem ( key ) );
      ArrangementItem* item = new ArrangementItem ( key, val, this );
      connect ( item, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
      setCellWidget ( r, 1, item );
      r++;
    }
    horizontalHeader()->setResizeMode ( 0, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode ( 1, QHeaderView::ResizeToContents );
  }
  cfg->endGroup();
}

void EditCookiesTable::saveCookieArrangements ( QSettings * cfg )
{
  Q_ASSERT ( cfg );

  int rows = rowCount();
  cfg->remove ( QLatin1String ( "CookieArrangement" ) );
  if ( rows >= 1 )
  {
    cfg->beginGroup ( QLatin1String ( "CookieArrangement" ) );
    for ( int r = 0; r < rows; r++ )
    {
      QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
      ArrangementItem* cb = qobject_cast<ArrangementItem*> ( cellWidget ( r, 1 ) );
      cfg->setValue ( key, cb->currentIndex() );
    }
    cfg->endGroup();
  }
}

void EditCookiesTable::removeItem()
{
  QList<QTableWidgetItem*> items = selectedItems();
  if ( items.size() < 1 )
    return;

  int r = row ( items.at ( 0 ) );
  if ( r >= 0 )
  {
    removeCellWidget ( r, 1 );
    removeRow ( r );
    emit modified();
  }
}

void EditCookiesTable::removeAll()
{
  clearContents();
  setRowCount ( 0 );
  emit modified();
}

bool EditCookiesTable::addCookie ( int t, const QString &h )
{
  if ( h.isEmpty() || t < 0 )
    return false;

  if ( rowCount() >= 1 )
  {
    for ( int r = 0; r < rowCount(); r++ )
    {
      QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
      if ( key == h )
      {
        setCurrentCell ( r, 0 );
        return false;
      }
    }
  }
  setRowCount ( rowCount() + 1 );
  int row = ( rowCount() - 1 );
  setItem ( row, 0, new QTableWidgetItem ( h ) );
  setCellWidget ( row, 1, new ArrangementItem ( h, t, this ) );
  emit modified();
  return true;
}

EditCookiesTable::~EditCookiesTable()
{}
