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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslConfiguration>

#include "ui_configdialogui.h"

class UserAgentEditor;

class ConfigDialog : public QDialog, protected Ui::ConfigDialogUi
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QSettings* cfg;
    UserAgentEditor* m_userAgentEditor;
    QSslConfiguration ssl;
    QString highlightBackgroundColor;
    QString defaultBackgroundStyle;
    QString highlightBorderColor;
    QString defaultBorderStyle;
    QPushButton* m_buttonCancel;
    QPushButton* m_buttonClose;
    QPushButton* m_buttonReset;
    QPushButton* m_buttonRestore;
    QPushButton* m_buttonSave;

    void setCaCertIssuerTable();

    // load
    void loadHeaderDefinitions();
    void loadUntrustedHostsWhiteList();

    // save
    void saveHeaderDefinitions();
    void saveUntrustedHostsWhiteList();

  private Q_SLOTS:
    void addCookieAccess();
    void addTrustedHost();
    void delTrustedHost();
    void addHeaderItem();
    void removeHeaderItem();
    void setModified();
    void loadSettings();
    void saveSettings();
    void restoreSettings();
    void setCaCertDatabase ( const QString &p = QString() );
    void setDomTreeBackgroundColor();
    void setDomTreeBorderColor();
    void getPrivKeyDialog();
    void getPupKeyDialog();
    void getCaCertDatabaseDialog();
    void getJavaDialog();
    void getCSSValidatorDialog();
    void getClassPathDialog();
    void quit();

  public:
    ConfigDialog ( QWidget * parent = 0, QSettings * settings = 0 );
    virtual ~ConfigDialog();
};

#endif
