/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#ifndef LOGLISTENER_H
#define LOGLISTENER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSettings>
  
/* KDE */
#include <KDE/KDirWatch>

class LogListener : public KDirWatch
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QSettings config;
    void setListener();
    void destroyListener();
    void openLogFileJob ( const QString & );

  private Q_SLOTS:
    void hasChanged ( const QString & );

  Q_SIGNALS:
    void logChanged ( const QString &name, const QString &log );

  public Q_SLOTS:
    void restart();
    void shutdown();

  public:
    LogListener ( QObject * parent = 0 );
    virtual ~LogListener();
};

#endif
