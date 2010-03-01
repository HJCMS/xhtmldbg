/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"
#include "xhtmldbg.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QClipboard>
#include <QtGui/QIcon>

Page::Page ( QObject * parent )
    : QWebPage ( parent )
{
  setObjectName ( "page" );

  // Settings
  QSettings* cfg = new QSettings ( QSettings::NativeFormat,
                                   QSettings::UserScope, "hjcms.de", "xhtmldbg", this );

  QWebSettings* ws = settings ();
  // Until QtWebkit defaults to 16
  ws->setFontSize ( QWebSettings::DefaultFontSize,
                    cfg->value ( QLatin1String ( "DefaultFontSize" ), 16 ).toUInt() );

  ws->setFontSize ( QWebSettings::DefaultFixedFontSize,
                    cfg->value ( QLatin1String ( "DefaultFixedFontSize" ), 16 ).toUInt() );
  // Page Settings
  ws->setAttribute ( QWebSettings::DeveloperExtrasEnabled,
                     cfg->value ( QLatin1String ( "DeveloperExtrasEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::AutoLoadImages,
                     cfg->value ( QLatin1String ( "AutoLoadImages" ), true ).toBool() );

  ws->setAttribute ( QWebSettings::JavascriptEnabled,
                     cfg->value ( QLatin1String ( "JavascriptEnabled" ), true ).toBool() );

  ws->setAttribute ( QWebSettings::PluginsEnabled,
                     cfg->value ( QLatin1String ( "PluginsEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::JavaEnabled,
                     cfg->value ( QLatin1String ( "JavaEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::PrivateBrowsingEnabled,
                     cfg->value ( QLatin1String ( "PrivateBrowsingEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled,
                     cfg->value ( QLatin1String ( "OfflineStorageDatabaseEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled,
                     cfg->value ( QLatin1String ( "OfflineWebApplicationCacheEnabled" ), false ).toBool() );

  ws->setAttribute ( QWebSettings::LocalStorageEnabled,
                     cfg->value ( QLatin1String ( "LocalStorageEnabled" ), false ).toBool() );

  action ( QWebPage::Reload )->setShortcut ( QKeySequence::Refresh );
  action ( QWebPage::Back )->setShortcut ( QKeySequence::Back );
  action ( QWebPage::Forward )->setShortcut ( QKeySequence::Forward );
  action ( QWebPage::Copy )->setShortcut ( QKeySequence::Copy );
  action ( QWebPage::Copy )->setIcon ( QIcon::fromTheme ( "edit-copy" ) );

  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );
}

void Page::javaScriptConsoleMessage ( const QString & m, int l, const QString & id )
{
  Q_UNUSED ( id )
  if ( m.isEmpty() )
    return;

  QList<QVariant> err;
  err << l << 0 << m;

  xhtmldbg::instance()->mainWindow()->debuggerMessage ( err );
}

void Page::triggerSelections()
{
  QString txt = selectedText();
  if ( txt.isEmpty() )
    return;

  QApplication::clipboard()->setText ( txt );
}

Page::~Page()
{}
