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

#ifndef WEBSETTINGS_H
#define WEBSETTINGS_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* QtWebKit */
#include <QtWebKit/QWebSettings>

#include "settings.h"

class WebSettings : public Settings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QWebSettings* m_ws;
    void setWebinspectorConfig ( bool b = true );
    void setPaths();
    void setWebSecurity();
    void setAttributes();
    void setFonts();

  Q_SIGNALS:
    void reloaded ( bool );

  public Q_SLOTS:
    void reload();

  public:
    explicit WebSettings ( QObject * parent = 0 );

    /** List with XHTMLDBG Supported QWebSettings::WebAttribute */
    static const QStringList attributeList();

    /** Set XHTMLDBG Defaults */
    bool setDefaults();

    /** get QWebSettings::WebAttribute value */
    bool attribute ( const QString &key );

    /** Modify QWebSettings::WebAttribute */
    void setAttribute ( const QString &key, bool value = false );

    /** read HTML Template */
    const QUrl templateSite ( const QString &key );

    /** read HTML Template Source */
    const QString templateSource ( const QString &key );

    /** enable/disable inspector */
    void inspector ( bool );

    virtual ~WebSettings();
};

#endif
