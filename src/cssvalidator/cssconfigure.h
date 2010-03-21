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

#ifndef CSSCONFIGURE_H
#define CSSCONFIGURE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include "ui_cssconfiguremain.h"

class CSSConfigure : public QDialog , protected Ui::CSSConfigureMain
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSettings* cfg;
    QStringList profileList;
    QStringList langList;
    QStringList warningList;
    QStringList mediumList;
    void setComboBoxItem ( const QString &, const QString & );
    bool loadSettings();
    bool saveSettings();

  private Q_SLOTS:
    void saveAndExit();

  public:
    CSSConfigure ( QWidget * parent = 0, QSettings * settings = 0 );
    ~CSSConfigure();
};

#endif
