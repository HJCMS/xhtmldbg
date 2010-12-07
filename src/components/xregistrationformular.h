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

#ifndef XREGISTRATIONFORMULAR_H
#define XREGISTRATIONFORMULAR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

/* Ui */
#include "ui_xregistrationformcomponent.h"

class Q_DECL_EXPORT XRegistrationFormular : public QWidget
      , protected Ui::XRegistrationFormComponent
      , protected QScriptable
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString title READ title WRITE setTitle )
    Q_PROPERTY ( QString data READ data WRITE setData )

  private:
    void setElementData ( const QString &name, const QVariant &value );

  public:
    explicit XRegistrationFormular ( QWidget * parent = 0 );

    /**
    * Set Meta-Object Property Title for GroupBox
    **/
    void setTitle ( const QString &t );

    /**
    * Current GroupBox Title
    **/
    QString title() const;

    /**
    * Set Meta-Object Property Title for GroupBox
    **/
    void setData ( const QString &d );

    /**
    * Returns a List with all Scriptable Object an here Values (if exists ;-))!
    **/
    QString data() const;

    virtual ~XRegistrationFormular();
};

#endif
