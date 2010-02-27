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

#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>

/* QtWebKit */
#include <QtWebKit/QWebHistoryInterface>

class HistoryItem;

class HistoryManager : public QWebHistoryInterface
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    int maxHistoryItems;
    QList<HistoryItem> m_history;

  protected:
    void addHistoryItem ( const HistoryItem & );

  Q_SIGNALS:
    void updateHistoryMenu ( const QList<HistoryItem> & );

  public Q_SLOTS:
    void clear();

  public:
    HistoryManager ( QObject * parent = 0 );
    void addHistoryEntry ( const QString & );
    bool historyContains ( const QString & ) const;
    ~HistoryManager();
};

#endif
