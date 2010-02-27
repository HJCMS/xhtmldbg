/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QDebug>

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

  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );
}

void Page::javaScriptConsoleMessage ( const QString & m, int l, const QString & id )
{
  qDebug() << Q_FUNC_INFO << "TODO:" << m << " Line:" << id;
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
