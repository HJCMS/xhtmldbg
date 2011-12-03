/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2012
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

#ifndef XHTMLDBGMAIN_H
#define XHTMLDBGMAIN_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>

/* KDE */
#include <KDE/KUniqueApplication>

#include "settings.h"
#include "window.h"

class DownloadManager;
class HistoryManager;
class NetworkCookie;
class NetworkAccessManager;
class DBManager;

class xhtmldbgmain : public KUniqueApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QPointer<Window> window;
    Settings* m_settings;
    void setWindowFocus();
    static HistoryManager* p_historyManager;
    static NetworkAccessManager* p_networkAccessManager;
    static DownloadManager* p_downloadManager;
    static DBManager* p_dbManager;

  protected:
    Window* newWindow();

  public:
    xhtmldbgmain ();
    static DBManager* dbManager( QObject * parent = 0 );
    static DownloadManager* downloadManager();
    static HistoryManager* historyManager();
    static NetworkAccessManager* networkAccessManager();
    static NetworkCookie* cookieManager();
    static xhtmldbgmain* instance();
    Window* mainWindow();
    virtual int newInstance();
    virtual ~xhtmldbgmain();
};

#endif
