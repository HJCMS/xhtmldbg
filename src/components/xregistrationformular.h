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
*/

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
    /** Initial default Map Properties e.g. encoding,use_space... */
    inline const QVariantMap initMap() const;

    /** Modify GroupBox title */
    void setElementData ( const QString &name, const QVariant &value );

  public Q_SLOTS:
    /**
    * Reset Form
    */
    void restore();

  public:
    explicit XRegistrationFormular ( QWidget * parent = 0 );

    /**
    * Modify GroupBox title
    */
    void setTitle ( const QString &t );

    /**
    * Read current GroupBox title
    */
    QString title() const;

    /**
    * Set Input Values with Json String, for construction about
    * this String see @ref data
    */
    void setData ( const QString &d );

    /**
    * create a list with all scriptable objects and here values.
    * This method will build a json string.
    * @code
    *   {
    *     "encoding" : "UTF-8",
    *     "length" : Array size,
    *     "use_space" : Boolean,
    *     "::objectName()" : mixed Values, ...
    *   }
    * @endcode
    * for more information read: @ref http://qjson.sourceforge.net/usage.html
    */
    QString data() const;

    virtual ~XRegistrationFormular();
};

#endif
