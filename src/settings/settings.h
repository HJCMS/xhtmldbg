/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#ifndef SETTINGS_H
#define SETTINGS_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

/* KDE */
#include <KDE/KGlobal>
#include <KDE/KGlobalSettings>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>

class Settings : public QSettings
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  public Q_SLOTS:
    /**
    * set QWebsettings/QNetworkSettings default cache behavior
    */
    void setCacheDefaults();

  public:
    Settings ( QObject * parent = 0 );

    /** Default Data Location */
    static const QString dataLocation();

    /** Default Cache Location */
    static const QString cacheLocation();

    /** Temp Directory for internal Application usage */
    static const QString tempDir ( const QString &subdir = QString() );

    /**
    * On X11, the search path will use the \em XDG_DATA_DIRS environment variable if available.
    * This function will reduce this path list for speed up XHTMLDBG Application start.
    * And set the current icon theme to \em oxygen
    */
    void setIconTheme();

    /**
    * This function will create all \em XDG_DATA_HOME paths that needed by XHTMLDBG.
    */
    void setDataPaths();

    /**
    * Restore all Settings for next startup
    */
    void setSaveMode();

    /**
    * Return for \em QWebSettings the path of the icon database.
    */
    static const QString webDatabasePath();

    /**
    * Returns for \em QWebSettings the path for HTML5 local storage.
    */
    static const QString webLocalStoragePath();

    /**
    * Url Path to History XML File
    */
    const QUrl historyXml();

    /**
    * Url Path to Bookmark XBEL File
    */
    const QUrl bookmarkXbel();

    /**
    * set last used Directory
    * \param d Directory Path
    */
    void setRecentDirectory ( const QString &d );

    /**
    * get last used directory, if empty HOME is set
    */
    const QString getRecentDirectory();

    /**
    * set last used Url
    * \param d Directory Path
    */
    void setRecentUrl ( const QUrl &u );

    /**
    * get last used url
    */
    const QUrl getRecentUrl();

    /**
    * Alias for QSettings::value(QString,int).toUInt()
    * \param p Settings Key(path)
    * \param i Value
    */
    int intValue ( const QString &p, int i );

    /**
    * Alias for QSettings::value(QString,bool).toBool()
    * \param p Settings Key(path)
    * \param b Value
    */
    bool boolValue ( const QString &p, bool b = false );

    /**
    * Alias for QSettings::value(QString,QString).toString()
    * \param p Settings Key(path)
    * \param d Value
    */
    const QString strValue ( const QString &p, const QString &d = QString() );

    virtual ~Settings();
};

#endif
