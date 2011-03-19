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

#ifndef CONFIGWEBSECURITY_H
#define CONFIGWEBSECURITY_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCheckBox>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KComboBox>
#include <KDE/KIntNumInput>
#include <KDE/KLineEdit>

/* configuration */
#include "pagewidget.h"
#include "websecuritytable.h"

class ConfigWebSecurity : public PageWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QString dbConnectionName;
    qint64 defaultQuota;
    WebSecurityTable* m_table;
    KLineEdit* m_hostname;
    KIntNumInput* m_port;
    KComboBox* m_scheme;
    QCheckBox* m_checkBox;
    KIntNumInput* m_dbQuota;
    QTableWidgetItem* createItem ( const QVariant & ) const;
    void fillTable ( QList<WebSecurityItem*> & );
    void loadSQLData();
    void saveSQLData();

  private Q_SLOTS:
    void itemRowChanged ( int );
    void schemeChanged ( int );
    void quotaChanged ( int );
    void clearInput();
    void itemSubmitted();
    void itemModified ( bool );

  public:
    ConfigWebSecurity ( QWidget * parent = 0 );
    void load ( Settings * );
    void save ( Settings * );
    bool isModified ();
    bool isSighted ();
    virtual ~ConfigWebSecurity();
};

#endif
