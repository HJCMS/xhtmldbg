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

#include "rssparser.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QTextStream>

/* Soprano */
#include <Soprano/Model>
#include <Soprano/Parser>
#include <Soprano/PluginManager>
#include <Soprano/StatementIterator>

RSSParser::RSSParser ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "rssparser" ) );
}

bool RSSParser::parseRDF ( const QByteArray &data, const QUrl &url ) const
{
  const Soprano::Parser* m_parser = Soprano::PluginManager::instance()->discoverParserForSerialization (
                                        Soprano::SerializationRdfXml );

  Q_CHECK_PTR ( m_parser );
  QTextStream stream ( data );
  Soprano::StatementIterator it = m_parser->parseStream ( stream, url, Soprano::SerializationRdfXml );
  while ( it.next() )
  {
    qDebug() << Q_FUNC_INFO << *it;
  }
  return true;
}

void RSSParser::parseUrl ( const QUrl &url )
{
  if ( !url.isValid() )
    return;

  qDebug() << Q_FUNC_INFO << url;
}

RSSParser::~RSSParser()
{}
