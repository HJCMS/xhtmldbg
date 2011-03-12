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

#include "securityorigin.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSettings>

/*

#ifdef DEBUG_VERBOSE
    qDebug() << Q_FUNC_INFO << mainFrame()->securityOrigin().port() << mainFrame()->securityOrigin().host();
#endif

    foreach ( QWebDatabase db, mainFrame()->securityOrigin().databases () )
    {
#ifdef DEBUG_VERBOSE
      qDebug() << Q_FUNC_INFO << db.fileName() << db.name();
#endif
      QStringList message ( QString::fromUtf8 ( "HTML 5 DB" ) );
      message << db.origin().host() << db.fileName() << db.name();
      xhtmldbgmain::instance()->mainWindow()->setApplicationMessage ( Qt::escape ( message.join ( " " ) ) );
    }
  }
*/

SecurityOrigin::SecurityOrigin ( const QWebSecurityOrigin &parent )
    : QWebSecurityOrigin ( parent )
{
  QSettings settings;
  setDatabaseQuota ( settings.value ( "WebDatabase/DefaultQuota", ( 3*1024 ) ).toUInt() );
}

const QVariant SecurityOrigin::getParam ( const QString &key, const QVariant &def ) const
{
  QVariant value = def;
  QSettings settings;
  int size = settings.beginReadArray ( "HTML5SecurityOptions" );
  for ( int r = 0; r < size; ++r )
  {
    settings.setArrayIndex ( r );
    QString h = settings.value ( "host" ).toString();
    if ( host() == h )
    {
      value = settings.value ( key );
      break;
    }
  }
  settings.endArray();
  return value;
}

/** FIXME Kann zur zeit NICHT Überladen werden :-/ */
int SecurityOrigin::port() const
{
  return getParam ( "port", 0 ).toUInt();
}

QWebSecurityOrigin SecurityOrigin::origin ( const QWebSecurityOrigin &parent )
{
  SecurityOrigin orig ( parent );
  return orig;
}
