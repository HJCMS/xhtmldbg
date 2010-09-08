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

#ifndef UITOOLSLOADER_H
#define UITOOLSLOADER_H

/* QtCore */
#include <QtCore/QIODevice>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptable>

/* QtUiTools */
#include <QtUiTools/QUiLoader>

class UiToolsLoader : protected QUiLoader, protected QScriptable
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    /**
    * identical with qt's class name
    */
    const QString classID;

    /**
    * is this component a valid qt class and loadable
    */
    bool isValid;

    /**
    * Local Storage URL
    **/
    const QUrl uiUrl;

    /**
    * Inherit all valid predicates and values from xhtml:object and
    * xhtml:param elements.
    */
    QMap<QString,QVariant> uiConfig;

    /**
    * a list with all writeable properties
    */
    const QStringList findProperties ( const QString &classID, QWidget * parent ) const;

  public:
    explicit UiToolsLoader ( const QString &cid, const QUrl &file, QObject * parent = 0 );

    /**
    * first we set all params and values to the loader.
    * @note if size of @param params and @param values not equal it will fail!
    * @param params  xhtml:object properties
    * @param values  xhtml:object values from param Elements and object predicates
    */
    bool setConfiguration ( const QStringList &params, const QStringList &values );

    /**
    * Return a static Widget with failure message!
    */
    QWidget* displayFailWidget ( QWidget * parent, const QString &mess = QString() ) const;

    /**
    * Loading UI Internal
    */
    QWidget* getUiComponent ( QWidget * parent );

    virtual ~UiToolsLoader();
};

#endif
