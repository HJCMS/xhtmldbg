/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "websecurityitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

WebSecurityItem::WebSecurityItem ( const QString &origin, qint64 quotadata )
    : p_scheme ( "file" )
    , p_host ( QLatin1String ( "localhost" ) )
    , p_port ( 0 )
    , p_quota ( quotadata )
{
  QStringList buffer = origin.split ( "_" );
  if ( buffer.size() == 3 )
  {
    p_scheme = buffer.at ( 0 );
    p_host = buffer.at ( 1 );
    p_port = buffer.at ( 2 );
  }
}

const QString WebSecurityItem::host()
{
  return p_host.toString();
}

void WebSecurityItem::setHost ( const QVariant &val )
{
  if ( val.isValid() )
    p_host = val;
}

QTableWidgetItem* WebSecurityItem::hostItem()
{
  QTableWidgetItem* item = new QTableWidgetItem ( p_host.toString(), QTableWidgetItem::UserType );
  item->setData ( Qt::UserRole, p_host );
  return item;
}

qint64 WebSecurityItem::port()
{
  return p_port.toUInt();
}

void WebSecurityItem::setPort ( const QVariant &val )
{
  if ( val.isValid() )
    p_port = val;
}

QTableWidgetItem* WebSecurityItem::portItem()
{
  QTableWidgetItem* item = new QTableWidgetItem ( p_port.toString(), QTableWidgetItem::UserType );
  item->setData ( Qt::UserRole, p_port );
  return item;
}

const QString WebSecurityItem::scheme()
{
  return p_scheme.toString();
}

void WebSecurityItem::setScheme ( const QVariant &val )
{
  if ( val.isValid() )
    p_scheme = val;
}

QTableWidgetItem* WebSecurityItem::schemeItem()
{
  QTableWidgetItem* item = new QTableWidgetItem ( p_scheme.toString(), QTableWidgetItem::UserType );
  item->setData ( Qt::UserRole, p_scheme );
  return item;
}

qint64 WebSecurityItem::quota()
{
  return p_quota.toUInt();
}

void WebSecurityItem::setQuota ( const QVariant &val )
{
  if ( val.isValid() )
    p_quota = val;
}

QTableWidgetItem* WebSecurityItem::quotaItem()
{
  QTableWidgetItem* item = new QTableWidgetItem ( p_quota.toString(), QTableWidgetItem::UserType );
  item->setData ( Qt::UserRole, p_quota );
  return item;
}
