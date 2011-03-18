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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "aboutdata.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* KDE */
#include <KDE/KGlobal>
#include <KDE/KLocale>

AboutData::AboutData()
    : KAboutData ( appsname(), appsname(), ki18n ( appsname() ),
                   version(), smallDescription(), KAboutData::License_LGPL_V3,
                   copyright(), description(), homepage(), eMail() )
{
  setOrganizationDomain ( XHTMLDBG_DOMAIN ); // This is implicitly required for D-Bus
  setProgramIconName ( appsname() );
  setProductName ( appsname() ); // This is implicitly required for D-Bus
  setVersion ( version() );
  addAuthor ( maintainer(), ki18n ( "developer" ), eMail(), homepage(), ocsUserName() );
  setTranslator ( maintainer(), ki18n ( eMail() ) );
}

const QByteArray AboutData::version()
{
  return QByteArray ( XHTMLDBG_VERSION_STRING );
}

const QByteArray AboutData::eMail()
{
  QByteArray m ( "xhtmldbg" );
  m.append ( "@" );
  m.append ( "hjcms.de" );
  return m;
}

const QByteArray AboutData::homepage()
{
  return QByteArray ( "http://xhtmldbg.hjcms.de" );
}

const QByteArray AboutData::appsname()
{
  return QByteArray ( XHTMLDBG_APPS_NAME );
}

const QByteArray AboutData::ocsUserName()
{
  return QByteArray ( "undefined" );
}

const KLocalizedString AboutData::maintainer()
{
  return ki18n ( "Juergen Heinemann (Undefined)" );
}

const KLocalizedString AboutData::copyright()
{
  return ki18n ( "Copyright (C) 2007-2011, Juergen Heinemann (Undefined)" );
}

const KLocalizedString AboutData::smallDescription()
{
  return ki18n ( "A XHTML/HTML Debugger and CSS Validator for Webdevelopers" );
}

const KLocalizedString AboutData::description()
{
  return ki18n ( "<div><p><strong>„xhtmldbg“ is a XHTML/HTML Debugger for Website Developers.</strong></p><p>This software provides you with the facility to check and analyze web page content directly.</p><p>All these are good starting points for targeted xhtml development debugging.</p><div><ul><li>to watch HTTP headers,</li><li>to watch _POST variables when send html forms,</li><li>css stylesheet watching,</li><li>dom inspector for better documents structure watching,</li><li>to indicate css stylesheet attributes,</li><li>to indicate current used cookies,</li><li>to watch javascript errors,</li><li>auto reload function up to 60 seconds,</li><li>analyzing <acronym title=\"Search Engine Optimisation\">SEO</acronym> keywords,</li><li>quickly HTTP_USER_AGENT switching,</li><li>strict RFC 2109 (HTTP State Management Mechanism) validation for Cookies,</li><li>validate ATOM/RSS/RDF Documents,</li><li>of course, this requires to validate and check the source.</li></ul></div></div>" );
}
