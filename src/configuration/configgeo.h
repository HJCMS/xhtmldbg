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

#ifndef CONFIGGEO_H
#define CONFIGGEO_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

#include "pagewidget.h"

class ConfigGeo : public PageWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QString databasePath;
    QLineEdit* dbEdit;
    const QString absoluteDatabasePath();

  private Q_SLOTS:
    void openDatabaseDialog();
    void dataChanged ( const QString & );

  public Q_SLOTS:
    void setDatabasePath ( const QString & );

  public:
    ConfigGeo ( QWidget * parent = 0 );
    void load ( QSettings * );
    void save ( QSettings * );
    void defaults();
    ~ConfigGeo();
};

#endif