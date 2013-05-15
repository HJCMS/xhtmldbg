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

#ifndef FORMENTRY_H
#define FORMENTRY_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

/* QtWebKit */
#include <QtWebKit/QWebElement>

/* KDE */
#include <KDE/KTabWidget>

/* FormManager */
#include "formbookmarkitem.h"

class FormTable;
class FormConstructor;

class FormEntry : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    FormTable* m_table;
    QLineEdit* m_project;
    QLineEdit* m_url;
    QLineEdit* m_formId;
    QCheckBox* m_autoinsert;
    QPushButton* btn_remove;
    QPushButton* btn_insert;

  private Q_SLOTS:
    void removeBookmark();
    void createBookmark();

  Q_SIGNALS:
    void itemChanged ( const QString &, FormBookmarkItem * );

  public Q_SLOTS:
    void setAutoInsertEnabled ( bool );

  public:
    FormEntry ( const FormConstructor &form, const QUrl &url, KTabWidget * parent = 0 );
    const QUrl urlRealm();
    void setUniqueId ( const QString & );
    virtual ~FormEntry();
};

#endif
