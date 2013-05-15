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

#ifndef HOSTINFO_H
#define HOSTINFO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QHostInfo>

class HostInfo : public QProcess
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private Q_SLOTS:
    void readStandardOutput ();
    void readErrorOutput();
    void exited ( int, QProcess::ExitStatus );

  public Q_SLOTS:
    void dig ( const QHostInfo &host );

  Q_SIGNALS:
    void labelData ( const QString & );
    void listData ( const QStringList & );
    void closedNormal ();

  public:
    HostInfo ( QWidget * parent = 0 );
    ~HostInfo();
};

#endif
