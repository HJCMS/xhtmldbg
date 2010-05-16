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

#ifndef XDEBUGPROJECTITEM_H
#define XDEBUGPROJECTITEM_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

class XDebugProjectItem
{
  private:
    QString Port;
    QString Name;
    QString RemoteHost;
    QString SessionId;

  public:
    explicit XDebugProjectItem ();
    XDebugProjectItem ( const XDebugProjectItem &other );
    void setPort ( const QString & );
    void setName ( const QString & );
    void setSessionId ( const QString & );
    void setRemoteHost ( const QString & );
    const QString getPort();
    const QString getName();
    const QString getSessionId();
    const QString getRemoteHost();
    virtual ~XDebugProjectItem();
};

#endif
