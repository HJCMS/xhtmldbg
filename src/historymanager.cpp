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

#include "historymanager.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

HistoryManager::HistoryManager ( QObject * parent )
    : QWebHistoryInterface ( parent )
    , maxHistoryItems ( 10 )
{
  QWebHistoryInterface::setDefaultInterface ( this );
}

/**
* Konvertiert einen String zurück in eine URL.
* Dabei wird die URL im @ref QUrl::StrictMode erstellt.
*/
const QUrl HistoryManager::toUrl ( const QString &path ) const
{
  QUrl url ( path, QUrl::StrictMode );
  if ( ! url.isValid() )
    return QUrl();

  if ( path.contains ( QRegExp ( "^file:\\/\\/" ) ) )
    url.setScheme ( "file" );
  else if ( path.contains ( QRegExp ( "^https:\\/\\/" ) ) )
    url.setScheme ( "https" );
  else if ( path.contains ( QRegExp ( "^ftp:\\/\\/" ) ) )
    url.setScheme ( "ftp" );
  else
    url.setScheme ( "http" );

  url.setPassword ( QString::null );
  url.setHost ( url.host().toLower() );
  return url;
}

/**
* Einen neuen Eintrag erzeugen in dem dieser an die
* Liste von @ref m_history angehangen wird.
*/
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

/**
* Erzeugt ein Object von @ref HistoryItem und fügt diesen
* mit @ref addHistoryItem in die Liste ein.
*/
void HistoryManager::addHistoryEntry ( const QString &url )
{
  QUrl addr = toUrl ( url );
  if ( addr.host().isEmpty() )
    return;

  HistoryItem item ( addr.toString(), QDateTime::currentDateTime(), addr.host() );
  addHistoryItem ( item );
}

/**
* Durch sucht @ref m_history nach dem @ref HistoryItem passenden
* Eintrag und gibt bei erfolg true zurück.
*/
bool HistoryManager::historyContains ( const QString &url ) const
{
  foreach ( HistoryItem i, m_history )
  {
    if ( url.contains ( i.url, Qt::CaseInsensitive ) )
      return true;
  }
  return false;
}

/**
* Leert die Historien Liste
*/
void HistoryManager::clear()
{
  m_history.clear();
}

HistoryManager::~HistoryManager()
{
  m_history.clear();
}
