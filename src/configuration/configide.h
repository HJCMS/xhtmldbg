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

#ifndef CONFIGIDE_H
#define CONFIGIDE_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

#include "pagewidget.h"

class IconThemeSelecter;
class IconThemesList;

class ConfigIDE : public PageWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QLineEdit* m_editStartUpUrl;
    QCheckBox* m_hideDownloadWidget;
    QLineEdit* m_javaApplication;
    QLineEdit* m_w3cJarFile;
    QLineEdit* m_w3cClasspath;
    QLabel* m_missingW3CMissing;
    IconThemeSelecter* m_iconThemeSelecter;
    IconThemesList* m_iconThemesList;
    void checkCSSValidator();

  private Q_SLOTS:
    void editingFinished ( const QString &txt = QString() );
    void openJavaApplicationDialog ();
    void openCSSValidatorDialog ();
    void openCSSValidatorClassPathDialog ();
    void openThemePathDialog ();
    void clearingThemePaths ( bool );

  public:
    ConfigIDE ( QWidget * parent = 0 );
    void load ( QSettings * );
    void save ( QSettings * );
    ~ConfigIDE();
};

#endif
