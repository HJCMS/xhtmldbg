/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2011
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

#ifndef XHTMLDBGADAPTOR_H
#define XHTMLDBGADAPTOR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtDBus */
#include <QtDBus/QDBusAbstractAdaptor>

class XHtmldbgAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.xhtmldbg" )

  private:
    const QString service;

  public:
    XHtmldbgAdaptor ( QObject *parent = 0 );
    const QString busService();
    virtual ~XHtmldbgAdaptor();

  public Q_SLOTS:
    Q_NOREPLY void message ( const QString &mess );
    /** \return true if URL accepted otherwise false */
    bool open ( const QString &url );
    /** \return true if URL accepted otherwise false */
    bool setUrl ( const QString &oldUrl, const QString &newUrl );
    /** \return true if URL accepted otherwise false */
    bool setFile ( const QString &url );
    /** \return true if URL accepted otherwise false */
    bool setSource ( const QString &url, const QString &xhtml );
    /** \return true if URL accepted otherwise false */
    bool checkStyleSheet ( const QString &url );

};

#endif
