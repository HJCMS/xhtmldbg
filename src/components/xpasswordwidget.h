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
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

/* XHTMLDBG Designer */
#include <widgetinterface.h>

/**
* @class XPasswordWidget
* @short x-qt-plugin for QDesigner or Website Integration
* @author Jürgen Heinemann (Undefined) http://www.hjcms.de
* @since 2010/09/10
* @code
  <object type="application/x-qt-plugin"
          classid="XPasswordWidget" name="xPasswordWidget"
          width="450" height="180">
    <param name="title" value="Enter your Password" valuetype="data" />
    <param name="user" value="Username" valuetype="data" />
    <param name="password" value="MyPassword" valuetype="data" />
  </object>
* @endcode
*/
class Q_DECL_EXPORT XPasswordWidget
      : public QWidget
      , protected QScriptable
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString title READ title WRITE setTitle )
    Q_PROPERTY ( QString user READ user WRITE setUser )
    Q_PROPERTY ( QString password READ password WRITE setPassword )
    Q_PROPERTY ( bool status READ status )

  private:
    QGroupBox* m_groupBox;
    QLineEdit* m_onwerLineEdit;
    QLineEdit* m_passLineEdit;

  private Q_SLOTS:
    void textModified ( const QString & );

  public Q_SLOTS:
    void restore();

  public:
    explicit XPasswordWidget ( QWidget * parent = 0 );

    /**
    * Returns the Status of @ref isReady
    **/
    bool status();

    /**
    * Set Meta-Object Property Title for GroupBox
    **/
    void setTitle ( const QString &t );

    /**
    * Current GroupBox Title
    **/
    QString title() const;

    /**
    * Set Meta-Object Property for Owner Edit
    **/
    void setUser ( const QString &u );

    /**
    * Current Owner Edit TextPlain Data
    **/
    QString user() const;

    /**
    * Set Meta-Object Property for Password Edit
    **/
    void setPassword ( const QString &p );

    /**
    * Current Password Edit TextPlain Data
    **/
    QString password() const;

    virtual ~XPasswordWidget();
};

#endif
