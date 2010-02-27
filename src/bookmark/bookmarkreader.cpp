/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "bookmarkreader.h"

#include <QtCore>

/* QtXml */
#include <QtXml/QDomNode>

BookmarkReader::BookmarkReader ( QMenu * menu )
    : QObject ( menu )
    , m_BoockmarkMenu ( menu )
    , bookmarkIcon ( QIcon::fromTheme ( QLatin1String ( "bookmarks" ) ) )
    , folderIcon ( QIcon::fromTheme ( QLatin1String ( "folder-bookmark" ) ) )
{
  m_signalMapper = new QSignalMapper ( this );
}

void BookmarkReader::rebuildMenu ( const QDomElement &root, QMenu* menu )
{
  QDomNode n = root.firstChild();
  while ( !n.isNull() )
  {
    QDomElement e = n.toElement();
    if ( ! e.isNull() )
    {
      if ( e.tagName() == "bookmark" )
      {
        QDomElement sub = e.elementsByTagName ( "title" ).item ( 0 ).toElement();
        QString title = sub.firstChild().nodeValue();
        if ( e.hasAttribute ( "href" ) && ! title.isEmpty() )
        {
          QString href = e.attributeNode ( QLatin1String ( "href" ) ).value();
          QAction* ac = menu->addAction ( bookmarkIcon, title );
          ac->setObjectName ( title );
          ac->setStatusTip ( href );
          connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
          m_signalMapper->setMapping ( ac, href );
        }
      }
      else if ( e.tagName() == "separator" )
      {
        menu->addSeparator();
      }
      else if ( e.tagName() == "folder" )
      {
        QDomElement sub = e.elementsByTagName ( "title" ).item ( 0 ).toElement();
        if ( e.hasChildNodes() )
          rebuildMenu ( e, menu->addMenu ( folderIcon, sub.firstChild().nodeValue() ) );
      }
    }
    n = n.nextSibling();
  }
}

bool BookmarkReader::read ( QIODevice* fp )
{
  QString errorMsg = 0;
  int errorLine = 0;
  int errorColumn = 0;
  bool status = dom.setContent ( fp, false, &errorMsg, &errorLine, &errorColumn );
  if ( status )
  {
    m_BoockmarkMenu->clear();
    rebuildMenu ( dom.documentElement(), m_BoockmarkMenu );
    connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
              this, SIGNAL ( openBookmark ( const QString & ) ) );
  }
  else
  {
    qWarning ( "XML Error: %s", qPrintable ( errorMsg ) );
  }
  return status;
}

BookmarkReader::~BookmarkReader()
{}
