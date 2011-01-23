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
#if QT_VERSION >= 0x040600
  hash.insert ( "DnsPrefetchEnabled", true );
#endif
  hash.insert ( "JavascriptEnabled", true );
  hash.insert ( "JavaEnabled", false );
  hash.insert ( "PluginsEnabled", false );
  hash.insert ( "PrivateBrowsingEnabled", false );
  hash.insert ( "JavascriptCanOpenWindows", false );
  hash.insert ( "JavascriptCanAccessClipboard", false );
  hash.insert ( "DeveloperExtrasEnabled", false );
  hash.insert ( "SpatialNavigationEnabled", false );
  hash.insert ( "LinksIncludedInFocusChain", false );
  hash.insert ( "ZoomTextOnly", true );
  hash.insert ( "PrintElementBackgrounds", false );
  hash.insert ( "OfflineStorageDatabaseEnabled", false );
  hash.insert ( "OfflineWebApplicationCacheEnabled", false );
  hash.insert ( "LocalStorageEnabled", false );
#if QT_VERSION >= 0x040700
  hash.insert ( "LocalContentCanAccessRemoteUrls", true );
  hash.insert ( "LocalContentCanAccessFileUrls", false );
  hash.insert ( "XSSAuditingEnabled", false );
  hash.insert ( "AcceleratedCompositingEnabled", true );
  hash.insert ( "TiledBackingStoreEnabled", false );
  hash.insert ( "FrameFlatteningEnabled", false );
  hash.insert ( "SiteSpecificQuirksEnabled", false );
  hash.insert ( "TiledBackingStoreEnabled", false );
#endif
  return hash;
}

static const inline QHash<QString,QWebSettings::WebAttribute> supported_attribute_list()
{
  QHash<QString,QWebSettings::WebAttribute> hash;
  hash.insert ( "AutoLoadImages", QWebSettings::AutoLoadImages );
#if QT_VERSION >= 0x040600
  hash.insert ( "DnsPrefetchEnabled", QWebSettings::DnsPrefetchEnabled );
#endif
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
  hash.insert ( "OfflineStorageDatabaseEnabled", QWebSettings::OfflineStorageDatabaseEnabled );
  hash.insert ( "OfflineWebApplicationCacheEnabled", QWebSettings::OfflineWebApplicationCacheEnabled );
  hash.insert ( "LocalStorageEnabled", QWebSettings::LocalStorageEnabled );
#if QT_VERSION >= 0x040700
  hash.insert ( "LocalContentCanAccessRemoteUrls", QWebSettings::LocalContentCanAccessRemoteUrls );
  hash.insert ( "LocalContentCanAccessFileUrls", QWebSettings::LocalContentCanAccessFileUrls );
  hash.insert ( "XSSAuditingEnabled", QWebSettings::XSSAuditingEnabled );
  hash.insert ( "AcceleratedCompositingEnabled", QWebSettings::AcceleratedCompositingEnabled );
  hash.insert ( "TiledBackingStoreEnabled", QWebSettings::TiledBackingStoreEnabled );
  hash.insert ( "FrameFlatteningEnabled", QWebSettings::FrameFlatteningEnabled );
  hash.insert ( "SiteSpecificQuirksEnabled", QWebSettings::SiteSpecificQuirksEnabled );
  hash.insert ( "TiledBackingStoreEnabled", QWebSettings::TiledBackingStoreEnabled );
#endif

  return hash;
}

WebSettings::WebSettings ( QObject * parent )
    : Settings ( parent )
    , m_ws ( QWebSettings::globalSettings() )
{
  m_ws->setDefaultTextEncoding ( QString::fromUtf8 ( "utf-8" ) );
}

void WebSettings::setPaths()
{
  m_ws->setLocalStoragePath ( webLocalStoragePath() );
  m_ws->setIconDatabasePath ( webIconDatabasePath() );
}

void WebSettings::setAttributes()
{
  QHash<QString,bool> defaults = default_web_settings();
  QHash<QString,QWebSettings::WebAttribute> hash = supported_attribute_list();
  QHash<QString,QWebSettings::WebAttribute>::iterator it;
  for ( it = hash.begin(); it != hash.end(); ++it )
  {
    bool fallback = defaults[it.key()];
    m_ws->setAttribute ( it.value(), value ( it.key(), fallback ).toBool() );
  }
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
  setAttributes();
  setFonts();
  return true;
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

WebSettings::~WebSettings()
{}
