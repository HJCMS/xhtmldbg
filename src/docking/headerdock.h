/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef HEADERDOCK_H
#define HEADERDOCK_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>

#include "docking.h"

class NetworkCookie;

class HeaderDock : public Docking
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    DockTreeWidget* m_treePostVars;
    DockTreeWidget* m_treeCookies;
    NetworkCookie* m_networkCookie;
    int minColumnWidth;
    void setCookieData ( const QNetworkCookie &, QTreeWidgetItem* );
    QString unserialize ( const QByteArray & ) const;

  protected:
    void setTreeHeaderLabels ( const QStringList &, int index = 0 );

  Q_SIGNALS:
    void isXdebugCookie ( const QString & );

  public Q_SLOTS:
    void clearAllData ();
    void setHeaderData ( const QUrl &, const QMap<QString,QString> & );
    void setPostedData ( const QUrl &, const QStringList & );
    void setCookieData ( const QUrl & );

  public:
    HeaderDock ( QWidget * parent = 0 );
    ~HeaderDock();
};

#endif
