/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

/* xhtmldbg */
#include "settings.h"

class ConfigurationMenu;
class StackedWidget;

class Configuration : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    Settings* cfg;
    QSplitter* m_splitter;
    // Menues
    ConfigurationMenu* menuWidget;
    // Pages
    StackedWidget* m_stackedWidget;
    // ButtonBox
    QPushButton* m_buttonCancel;
    QPushButton* m_buttonClose;
    QPushButton* m_buttonReset;
    QPushButton* m_buttonRestore;
    QPushButton* m_buttonSave;

  private Q_SLOTS:
    void restoreSettings();
    void quit();

  public:
    Configuration ( QWidget * parent = 0, Settings * settings = 0 );
    ~Configuration();
};

#endif
