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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "aboutdata.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* KDE */
#include <KDE/KGlobal>
#include <KDE/KLocale>

/* http://api.kde.org/4.x-api/kdelibs-apidocs/kdecore/html/classKAboutData.html */
AboutData::AboutData()
    : KAboutData ( appsname(), appsname(), ki18n ( appsname() ), version(),
                   smallDescription(), KAboutData::License_LGPL_V3,
                   copyright(), description(), homepage(), eMail() )
{
  // This is implicitly required for D-Bus
  setProductName ( appsname() );
  setOrganizationDomain ( pagedomain() );
  setProgramIconName ( appsname() );
  setVersion ( version() );
  addAuthor ( maintainer(), ki18n ( "developer" ), eMail(), homepage(), ocsUserName() );
  setTranslator ( maintainer(), ki18n ( eMail() ) );
}

/**
* Aktuelle Version
*/
const QByteArray AboutData::version()
{
  return QByteArray ( XHTMLDBG_VERSION_STRING );
}

/**
* E-Mail Adresse
*/
const QByteArray AboutData::eMail()
{
  QByteArray m ( XHTMLDBG_APPS_NAME );
  m.append ( "@" );
  m.append ( "hjcms.de" );
  return m;
}

/**
* Die Seiten Domäne wird für DBus benötigt!
*/
const QByteArray AboutData::pagedomain()
{
  return QByteArray ( XHTMLDBG_DOMAIN );
}

/**
* XHTMLDBG Webseite
*/
const QByteArray AboutData::homepage()
{
  return QByteArray ( "http://xhtmldbg.hjcms.de" );
}

/**
* Programm Name
*/
const QByteArray AboutData::appsname()
{
  return QByteArray ( XHTMLDBG_APPS_NAME );
}

/**
* Open Collaboration Services
* Angezeigter Benutzer Name
*/
const QByteArray AboutData::ocsUserName()
{
  return QByteArray ( "undefined" );
}

/**
* DBus Schnittstelle, siehe main.cpp
*/
const QByteArray AboutData::service()
{
  return QByteArray ( "de.hjcms.xhtmldbg" );
}

/**
* Entwickler Benutzer Name
*/
const KLocalizedString AboutData::maintainer()
{
  return ki18n ( "Juergen Heinemann (Undefined)" );
}

/**
* Lizenz Hinweis kurz
*/
const KLocalizedString AboutData::copyright()
{
  return ki18n ( "Copyright (C) 2007-2013, Juergen Heinemann (Undefined)" );
}

/**
* Kurze Beschreibung
*/
const KLocalizedString AboutData::smallDescription()
{
  return ki18n ( "A XHTML/HTML Debugger and CSS Validator for Webdevelopers" );
}

/**
* Ausführliche Beschreibung
*/
const KLocalizedString AboutData::description()
{
  return ki18n ( "<div><p><strong>„xhtmldbg“ is a XHTML/HTML Debugger for Website Developers.</strong></p><p>This software provides you with the facility to check and analyze web page content directly.</p><p>All these are good starting points for targeted xhtml development debugging.</p><div><ul><li>to watch HTTP headers,</li><li>to watch _POST variables when send html forms,</li><li>css stylesheet watching,</li><li>dom inspector for better documents structure watching,</li><li>to indicate css stylesheet attributes,</li><li>to indicate current used cookies,</li><li>to watch javascript errors,</li><li>auto reload function up to 60 seconds,</li><li>analyzing <acronym title=\"Search Engine Optimisation\">SEO</acronym> keywords,</li><li>quickly HTTP_USER_AGENT switching,</li><li>strict RFC 2109 (HTTP State Management Mechanism) validation for Cookies,</li><li>validate ATOM/RSS/RDF Documents,</li><li>of course, this requires to validate and check the source.</li></ul></div></div>" );
}
