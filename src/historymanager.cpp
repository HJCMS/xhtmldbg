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

#include "historymanager.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore>

/* QtGui */
#include <QtGui>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

HistoryManager::HistoryManager ( QObject * parent )
    : QWebHistoryInterface ( parent )
    , maxHistoryItems ( 10 )
{
  QWebHistoryInterface::setDefaultInterface ( this );
}

void HistoryManager::addHistoryItem ( const HistoryItem &item )
{
  QWebSettings *cfg = QWebSettings::globalSettings();
  if ( cfg->testAttribute ( QWebSettings::PrivateBrowsingEnabled ) )
    return;

  m_history.prepend ( item );
  if ( m_history.count() > maxHistoryItems )
    m_history.removeLast();

  emit updateHistoryMenu ( m_history );
}

void HistoryManager::addHistoryEntry ( const QString &url )
{
  QUrl address ( url );
  address.setPassword ( QString::null );
  address.setHost ( address.host().toLower() );
  HistoryItem item ( address.toString(), QDateTime::currentDateTime() );
  addHistoryItem ( item );
}

bool HistoryManager::historyContains ( const QString &url ) const
{
  QString check = url.toLower();
  foreach ( HistoryItem i, m_history )
  {
    if ( i.url == check )
      return true;
  }
  return false;
}

void HistoryManager::clear()
{
  m_history.clear();
}

HistoryManager::~HistoryManager()
{}
