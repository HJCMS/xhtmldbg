/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QList>
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

  QWebSettings* ws = settings ();
  ws->setAttribute ( QWebSettings::DeveloperExtrasEnabled, false );
  ws->setAttribute ( QWebSettings::AutoLoadImages, true );
  ws->setAttribute ( QWebSettings::JavascriptEnabled, true );
  ws->setAttribute ( QWebSettings::PluginsEnabled, false );
  ws->setAttribute ( QWebSettings::JavaEnabled, true );
  ws->setAttribute ( QWebSettings::PrivateBrowsingEnabled, false );
  ws->setAttribute ( QWebSettings::OfflineStorageDatabaseEnabled, false );
  ws->setAttribute ( QWebSettings::OfflineWebApplicationCacheEnabled, false );
  ws->setAttribute ( QWebSettings::LocalStorageEnabled, false );

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
  emit scriptConsoleMessage ( l, m );
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
