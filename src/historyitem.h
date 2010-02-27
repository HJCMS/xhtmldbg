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

#ifndef HISTORYITEM_H
#define HISTORYITEM_H

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

class HistoryItem
{
  public:
    QString title;
    QString url;
    QDateTime dateTime;
    HistoryItem();
    HistoryItem ( const QString &u, const QDateTime &d = QDateTime(), const QString &t = QString() );
    inline bool operator == ( const HistoryItem & ) const;
    inline bool operator < ( const HistoryItem & ) const;
};

#endif
