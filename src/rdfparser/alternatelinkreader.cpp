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

#include "alternatelinkreader.h"
#include "rssparserdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QPixmap>
#include <QtGui/QMenu>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/* QtWebKit */
#include <QtWebKit/QWebElementCollection>

AlternateLinkReader::AlternateLinkReader ( QWidget * parent )
    : QWidget ( parent )
    , comboTitle ( trUtf8 ( "RSS/Atom" ) )
    , rssIcon ( QIcon::fromTheme ( QLatin1String ( "application-rss+xml" ) ) )
{
  setObjectName ( QLatin1String ( "alternatelinkreader" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMaximumWidth ( 300 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "alternatelinkreaderlayout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  m_signalMapper =  new QSignalMapper ( this );
  m_signalMapper->setObjectName ( QLatin1String ( "alternatelinkreadermapper" ) );

  // Button erstellen
  m_toolButton = new QToolButton ( this );
  m_toolButton->setObjectName ( QLatin1String ( "alternatelinkreaderbutton" ) );
  m_toolButton->setIcon ( rssIcon );
  m_toolButton->setAutoRaise ( true );
  m_toolButton->setPopupMode ( QToolButton::InstantPopup );
  layout->addWidget ( m_toolButton );

  setLayout ( layout );

  connect ( m_signalMapper, SIGNAL ( mapped ( int ) ),
            this, SLOT ( itemClicked ( int ) ) );
}

/**
* Dialog für die Syntaxprüfung öffnen.
*/
void AlternateLinkReader::openParserDialog ( const QUrl &url, const QString &mimeType )
{
  if ( ! url.isValid() )
    return;

  RSSParserDialog* dialog = new RSSParserDialog ( url, mimeType, this );
  dialog->exec();
  delete dialog;
}

/**
*
*/
void AlternateLinkReader::itemClicked ( int index )
{
  if ( index < 0 )
    return;

  LinkItem item = items.at ( index );

  openParserDialog ( item.url, item.mime );
}

/**
* Im HEAD nach LINK mit Prädikat link[rel^=alternate] suchen und
* nachsehen ob es sich um RSS handelt.
*/
void AlternateLinkReader::setDomWebElement ( const QUrl &url, const QWebElement &dom )
{
  QMenu* menu = new QMenu ( m_toolButton );
  QWebElementCollection nodeList = dom.findAll ( QString::fromUtf8 ( "link[rel^=alternate]" ) );

  items.clear(); // Liste leeren
  // Alle application/rss+xml und application/atom+xml durchlaufen
  int index = 0;
  foreach ( QWebElement element, nodeList )
  {
    QString type = element.attribute ( QLatin1String ( "type" ), QString::null );
    if ( type.isEmpty() || ! type.contains ( QRegExp ( "(atom|rss)\\+xml", Qt::CaseInsensitive ) ) )
      continue;

    QString source = element.attribute ( QLatin1String ( "href" ), QString::null );
    if ( source.isEmpty() )
      continue;

    QString title = element.attribute ( QLatin1String ( "title" ), comboTitle );
    QAction* ac = menu->addAction ( rssIcon, title );
    ac->setData ( source );
    connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
    m_signalMapper->setMapping ( ac, index );

    QUrl linkUrl;
    if ( source.contains ( QRegExp ( "^http(s)?:\\/\\/" ) ) )
    {
      linkUrl.setUrl ( source );
    }
    else
    {
      linkUrl.setScheme ( url.scheme() );
      linkUrl.setHost ( url.host() );
      linkUrl.setPath ( source );
    }

    LinkItem item;
    item.url = linkUrl;
    item.mime = type;
    items << item;

    index++;
  }
  m_toolButton->setMenu ( menu );
}

AlternateLinkReader::~AlternateLinkReader()
{
}
