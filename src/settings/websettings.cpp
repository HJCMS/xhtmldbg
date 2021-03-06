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

#include "websettings.h"

#ifndef XHTMLDBG_VERSION_STRING
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QHash>
#include <QtCore/QHashIterator>
#include <QtCore/QLibraryInfo>
#include <QtCore/QList>
#include <QtCore/QTextCodec>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtWebKit */
#include <QtWebKit/QWebSecurityOrigin>

/**
* In diesem Kontainer werden alle Auswahl Boxen registriert!
* Dabei ist der Objektname identisch mit dem Parameter
* der Einstellungen. Siehe auch @ref QWebSettings
* Der Boolische Wert ist der Standardwert.
* http://doc.qt.nokia.com/main-snapshot/qwebsettings.html#WebAttribute-enum
*/
static inline const QHash<QString,bool> default_web_settings()
{
  QHash<QString,bool> hash;
  hash.insert ( "AutoLoadImages", true );
  hash.insert ( "DnsPrefetchEnabled", true );
  hash.insert ( "JavascriptEnabled", true );
  hash.insert ( "JavaEnabled", false );
  hash.insert ( "PluginsEnabled", false );
  hash.insert ( "PrivateBrowsingEnabled", false );
  hash.insert ( "JavascriptCanOpenWindows", false );
  hash.insert ( "JavascriptCanAccessClipboard", false );
  hash.insert ( "DeveloperExtrasEnabled", true );
  hash.insert ( "SpatialNavigationEnabled", false );
  hash.insert ( "LinksIncludedInFocusChain", false );
  hash.insert ( "ZoomTextOnly", true );
  hash.insert ( "PrintElementBackgrounds", false );
  hash.insert ( "LocalContentCanAccessRemoteUrls", true );
  hash.insert ( "LocalContentCanAccessFileUrls", true );
  hash.insert ( "XSSAuditingEnabled", false );
  hash.insert ( "AcceleratedCompositingEnabled", true );
  hash.insert ( "TiledBackingStoreEnabled", false );
  hash.insert ( "FrameFlatteningEnabled", false );
  hash.insert ( "SiteSpecificQuirksEnabled", false );
  hash.insert ( "TiledBackingStoreEnabled", false );
// NOTE Wird in WebSettings::setAttributes() gesetzt!
//   hash.insert ( "LocalStorageEnabled", false );
//   hash.insert ( "OfflineStorageDatabaseEnabled", false );
//   hash.insert ( "OfflineWebApplicationCacheEnabled", false );
  return hash;
}

static const inline QHash<QString,QWebSettings::WebAttribute> supported_attribute_list()
{
  QHash<QString,QWebSettings::WebAttribute> hash;
  hash.insert ( "AutoLoadImages", QWebSettings::AutoLoadImages );
  hash.insert ( "DnsPrefetchEnabled", QWebSettings::DnsPrefetchEnabled );
  hash.insert ( "JavascriptEnabled", QWebSettings::JavascriptEnabled );
  hash.insert ( "JavaEnabled", QWebSettings::JavaEnabled );
  hash.insert ( "PluginsEnabled", QWebSettings::PluginsEnabled );
  hash.insert ( "PrivateBrowsingEnabled", QWebSettings::PrivateBrowsingEnabled );
  hash.insert ( "JavascriptCanOpenWindows", QWebSettings::JavascriptCanOpenWindows );
  hash.insert ( "JavascriptCanAccessClipboard", QWebSettings::JavascriptCanAccessClipboard );
  hash.insert ( "DeveloperExtrasEnabled", QWebSettings::DeveloperExtrasEnabled );
  hash.insert ( "SpatialNavigationEnabled", QWebSettings::SpatialNavigationEnabled );
  hash.insert ( "LinksIncludedInFocusChain", QWebSettings::LinksIncludedInFocusChain );
  hash.insert ( "ZoomTextOnly", QWebSettings::ZoomTextOnly );
  hash.insert ( "PrintElementBackgrounds", QWebSettings::PrintElementBackgrounds );
  hash.insert ( "LocalContentCanAccessRemoteUrls", QWebSettings::LocalContentCanAccessRemoteUrls );
  hash.insert ( "LocalContentCanAccessFileUrls", QWebSettings::LocalContentCanAccessFileUrls );
  hash.insert ( "XSSAuditingEnabled", QWebSettings::XSSAuditingEnabled );
  hash.insert ( "AcceleratedCompositingEnabled", QWebSettings::AcceleratedCompositingEnabled );
  hash.insert ( "TiledBackingStoreEnabled", QWebSettings::TiledBackingStoreEnabled );
  hash.insert ( "FrameFlatteningEnabled", QWebSettings::FrameFlatteningEnabled );
  hash.insert ( "SiteSpecificQuirksEnabled", QWebSettings::SiteSpecificQuirksEnabled );
  hash.insert ( "TiledBackingStoreEnabled", QWebSettings::TiledBackingStoreEnabled );
  return hash;
}

WebSettings::WebSettings ( QObject * parent )
    : Settings ( parent )
    , m_ws ( QWebSettings::globalSettings() )
{
  m_ws->setDefaultTextEncoding ( QString::fromUtf8 ( "utf-8" ) );
}

/** Dieser Pfad wird von QWebInspector benutzt! */
void WebSettings::setWebinspectorConfig ( bool b )
{
  QString bString ( ( (b) ? "true" : "false" ) );
  static const QString p ( "Qt/QtWebKit/QWebInspector/" );
  // Debugger
  setValue ( p + "debuggerEnabled", bString );
  setValue ( p + "debuggerEnabled.type", "bool" );
  setValue ( p + "debuggerAlwaysEnabled", bString );
  setValue ( p + "debuggerAlwaysEnabled.type", "bool" );
  // Resource Tracking
  setValue ( p + "resourceTrackingEnabled", bString );
  setValue ( p + "resourceTrackingEnabled.type", "bool" );
  setValue ( p + "resourceTrackingAlwaysEnabled", bString );
  setValue ( p + "resourceTrackingAlwaysEnabled.type", "bool" );
  // Profiler
  setValue ( p + "profilerEnabled", bString );
  setValue ( p + "profilerEnabled.type", "bool" );
  // Panel
  setValue ( p + "panelEnablerView", bString );
  setValue ( p + "panelEnablerView.type", "bool" );
  // Audits
  setValue ( p + "auditsPanelEnabled", bString );
  setValue ( p + "auditsPanelEnabled.type", "bool" );
}

void WebSettings::setPaths()
{
  // NOTE Muss vor Storage,Icon und Offline stehen!
  QWebSettings::enablePersistentStorage ( dataLocation() );
  m_ws->setLocalStoragePath ( webLocalStoragePath() );
  m_ws->setIconDatabasePath ( webDatabasePath() );
  m_ws->setOfflineStoragePath ( webDatabasePath() );
  m_ws->setOfflineWebApplicationCachePath ( webDatabasePath() );
}

/**
* http://www.w3.org/TR/webstorage
* http://webkit.org/blog/427/webkit-page-cache-i-the-basics/
*/
void WebSettings::setWebSecurity()
{
  QWebSecurityOrigin::removeLocalScheme ( QString::fromUtf8 ( "qrc" ) );
  m_ws->setOfflineStorageDefaultQuota ( value ( "WebDatabase/DefaultQuota", ( 5 * 1024 ) ).toUInt() );
  m_ws->setMaximumPagesInCache ( value ( "WebDatabase/PagesInCache", 5 ).toUInt() );
}

void WebSettings::setAttributes()
{
  QHash<QString,bool> defaults = default_web_settings();
  QHash<QString,QWebSettings::WebAttribute> hash = supported_attribute_list();
  QHash<QString,QWebSettings::WebAttribute>::iterator it;
  for ( it = hash.begin(); it != hash.end(); ++it )
  {
    bool fallback = defaults[ it.key() ];
    m_ws->setAttribute ( it.value(), value ( it.key(), fallback ).toBool() );
  }
  // HTML 5 Extras Verarbeiten
  m_ws->setAttribute ( QWebSettings::LocalStorageEnabled, value ( "LocalStorageEnabled" ).toBool() );
  m_ws->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, value ( "OfflineStorageDatabaseEnabled" ).toBool() );
  m_ws->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, value ( "OfflineWebApplicationCacheEnabled" ).toBool() );
}

void WebSettings::setFonts()
{
  m_ws->setFontSize ( QWebSettings::DefaultFontSize, value ( "DefaultFontSize", 16 ).toUInt() );
  m_ws->setFontSize ( QWebSettings::DefaultFixedFontSize, value ( "DefaultFixedFontSize", 16 ).toUInt() );
}

void WebSettings::reload()
{
  if ( setDefaults() )
    emit reloaded ( true );
}

const QStringList WebSettings::attributeList()
{
  QStringList list;
  foreach ( QString k, supported_attribute_list().keys () )
  {
    list.append ( k );
  }
  return list;
}

bool WebSettings::setDefaults()
{
  setPaths();
  setWebSecurity();
  setAttributes();
  setFonts();
  return true;
}

bool WebSettings::attribute ( const QString &key )
{
  QHash<QString,QWebSettings::WebAttribute> hash = supported_attribute_list();
  QHash<QString,QWebSettings::WebAttribute>::iterator it;
  for ( it = hash.begin(); it != hash.end(); ++it )
  {
    if ( it.key() == key )
      return m_ws->testAttribute ( it.value() );
  }
  return false;
}

void WebSettings::setAttribute ( const QString &key, bool value )
{
  QHash<QString,QWebSettings::WebAttribute> hash = supported_attribute_list();
  QHash<QString,QWebSettings::WebAttribute>::iterator it;
  for ( it = hash.begin(); it != hash.end(); ++it )
  {
    if ( it.key() == key )
    {
      m_ws->setAttribute ( it.value(), value );
      break;
    }
  }
}

/** Suche nach einer Templae Datei */
const QUrl WebSettings::templateSite ( const QString &key )
{
  QUrl url;
  url.setScheme ( "file" );
  QString sources;
  QStringList paths;
  paths << QString ( XHTMLDBG_TEMPLATE_DIR );
  paths << QCoreApplication::applicationDirPath() + "/../src/html";

  foreach ( QString p, paths )
  {
    QString file ( p );
    file.append ( "/" );
    file.append ( key );
    file.append ( ".html" );
    QFileInfo info ( file );
    if ( info.exists() )
    {
      url.setPath ( info.absoluteFilePath() );
      break;
    }
  }
  return url;
}

const QString WebSettings::templateSource ( const QString &key )
{
  QString sources;
  QString html = templateSite ( key ).path();
  if ( html.isEmpty() )
    return QString::null;

  QFile fp ( html );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QTextStream stream ( &fp );
    QByteArray buffer = stream.device()->readAll();
    QTextCodec* codec = QTextCodec::codecForHtml ( buffer );
    sources = codec->toUnicode ( buffer );
    fp.close();
  }
  return sources;
}

void WebSettings::inspector ( bool b )
{
  setWebinspectorConfig ( b );
  m_ws->setAttribute ( QWebSettings::DeveloperExtrasEnabled, b );
}

WebSettings::~WebSettings()
{}
