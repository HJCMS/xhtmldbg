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
    , maxHistoryItems ( 20 )
{
  // QWebHistoryInterface will delete the history manager
  QWebHistoryInterface::setDefaultInterface ( this );
}

void HistoryManager::save()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void HistoryManager::addHistoryItem ( const HistoryItem &item )
{
  QWebSettings *cfg = QWebSettings::globalSettings();
  if ( cfg->testAttribute ( QWebSettings::PrivateBrowsingEnabled ) )
    return;

  m_history.prepend ( item );
  if ( m_history.count() > maxHistoryItems )
    m_history.removeLast();
}

void HistoryManager::addHistoryEntry ( const QString &url )
{
  qDebug() << Q_FUNC_INFO << "TODO" << url;
}

bool HistoryManager::historyContains ( const QString &url )
{
  foreach ( HistoryItem i, m_history )
  {
    if ( i.url == url )
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
