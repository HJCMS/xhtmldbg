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

#ifndef CONFIGHEADERDEFINITIONS_H
#define CONFIGHEADERDEFINITIONS_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

/* xhtmldbg */
#include "settings.h"

class ConfigHeaderDefinitions : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QTableWidget* headersTable;

  private Q_SLOTS:
    void itemChanged ( QTableWidgetItem * );
    void addHeaderItem();
    void removeHeaderItem();

  Q_SIGNALS:
    void modified ( bool );

  public:
    ConfigHeaderDefinitions ( QWidget * parent = 0 );
    void loadHeaderDefinitions ( Settings * );
    void saveHeaderDefinitions ( Settings * );
    ~ConfigHeaderDefinitions();
};

#endif
