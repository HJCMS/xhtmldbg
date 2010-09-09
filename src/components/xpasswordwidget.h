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

#ifndef XPASSWORDWIDGET_H
#define XPASSWORDWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

/* XHTMLDBG Designer */
#include <widgetinterface.h>

class Q_DECL_EXPORT XPasswordWidget
      : public QWidget
      , protected QScriptable
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString title READ title WRITE setTitle SCRIPTABLE false )
    Q_PROPERTY ( QString user READ user WRITE setUser )
    Q_PROPERTY ( QString password READ password WRITE setPassword )
    Q_PROPERTY ( bool status READ status NOTIFY submitted )

  private:
    bool isReady;

  private Q_SLOTS:
    void restore();
    void submit();

  protected:
    QGroupBox* m_groupBox;
    QLineEdit* m_onwerLineEdit;
    QLineEdit* m_passLineEdit;
    QPushButton* m_submitbutton;

  Q_SIGNALS:
    void submitted ();

  public:
    explicit XPasswordWidget ( QWidget * parent = 0 );

    bool status();

    /**
    * Set Meta-Object Property Title for @ref m_groupBox
    **/
    void setTitle ( const QString &t );

    /**
    * Current @ref m_groupBox Title
    **/
    QString title() const;

    /**
    * Set Meta-Object Property for @ref m_onwerLineEdit
    **/
    void setUser ( const QString &u );

    /**
    * Current @ref m_passLineEdit Plain Text Data
    **/
    QString user() const;

    /**
    * Set Meta-Object Property for @ref m_passLineEdit
    **/
    void setPassword ( const QString &p );

    /**
    * Current @ref m_passLineEdit Plain Text Data
    **/
    QString password() const;

    virtual ~XPasswordWidget();
};

#endif
