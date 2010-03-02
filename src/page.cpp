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

bool Page::acceptNavigationRequest ( QWebFrame *, const QNetworkRequest &, QWebPage::NavigationType )
{
  // qDebug() << Q_FUNC_INFO;
  return true;
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
