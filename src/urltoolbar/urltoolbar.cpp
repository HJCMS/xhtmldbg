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

#include "urltoolbar.h"
#include "urllineedit.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCompleter>
#include <QtGui/QLabel>
#include <QtGui/QLayout>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>
#include <KDE/KUrl>

UrlToolBar::UrlToolBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "UrlToolBar" ) );
  setWindowTitle ( i18n ( "Url" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );
  setMinimumWidth ( 250 );

  QAction* m_urlUpButton = addAction ( QLatin1String ( "Index" ) );
  m_urlUpButton->setIcon ( KIcon ( QLatin1String ( "go-up" ) ) );

  QAction* m_reloadButton = addAction ( i18n ( "Reload" ) );
  m_reloadButton->setIcon ( KIcon ( QLatin1String ( "view-refresh" ) ) );

  QLabel* m_label = new QLabel ( i18n ( "Url:" ), this );
  m_label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( m_label );

  m_urlLineEdit = new UrlLineEdit ( this );
  addWidget ( m_urlLineEdit );

  QAction* m_clearButton = addAction ( i18n ( "Clear" ) );
  m_clearButton->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );

  QAction* m_startButton = addAction ( QLatin1String ( "Go" ) );
  m_startButton->setIcon ( KIcon ( QLatin1String ( "go-jump-locationbar" ) ) );

  connect ( m_urlUpButton, SIGNAL ( triggered() ),
            this, SLOT ( moveUpUrl() ) );

  connect ( m_reloadButton, SIGNAL ( triggered() ),
            this, SLOT ( urlReloadPage() ) );

  connect ( m_urlLineEdit, SIGNAL ( urlChanged ( const QUrl & ) ),
            this, SIGNAL ( urlChanged ( const QUrl & ) ) );

  connect ( m_clearButton, SIGNAL ( triggered() ),
            m_urlLineEdit, SLOT ( startAutoEdit() ) );

  connect ( m_startButton, SIGNAL ( triggered() ),
            m_urlLineEdit, SLOT ( urlEntered() ) );
}

/**
* Im URL Pfad eins aufwärts
*/
void UrlToolBar::moveUpUrl ()
{
  KUrl url = m_urlLineEdit->url().upUrl();
  if ( url.isValid() )
  {
    m_urlLineEdit->setUrl ( url );
    emit urlChanged ( url );
  }
}

/**
* URL neu Laden
*/
void UrlToolBar::urlReloadPage()
{
  m_urlLineEdit->setUrl ( m_urlLineEdit->url() );
  emit reloadUrl ( m_urlLineEdit->url() );
}

/**
* Die Adresse ungeprüft in die Adresszeile einfügen.
*/
void UrlToolBar::setUrl ( const QUrl &url )
{
  m_urlLineEdit->setUrl ( url.toString() );
}

UrlToolBar::~UrlToolBar()
{}
