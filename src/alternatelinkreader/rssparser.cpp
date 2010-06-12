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
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtCore/QTextStream>

void messageHandler ( void* ptr, raptor_locator* locator, const char* message )
{
  // RSSParser* p = static_cast<RSSParser*> ( ptr );
  qDebug() << Q_FUNC_INFO << locator->line << locator->column << message;
}

/**
* Set the statement callback function for the parser.
* The raptor_statement structure is defined in raptor.h
* and includes fields for the subject, predicate,
* object of the statements along with their types and for literals,
* language and datatype.
*/
void statementHandler ( void* ptr, const raptor_statement* triple )
{
  Q_UNUSED ( ptr )

  switch ( triple->subject_type )
  {
    case RAPTOR_IDENTIFIER_TYPE_UNKNOWN:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_UNKNOWN";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_RESOURCE:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_RESOURCE";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_ANONYMOUS:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_ANONYMOUS";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_PREDICATE:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_PREDICATE";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_ORDINAL:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_ORDINAL";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_LITERAL:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_LITERAL";
    }
    break;

    case RAPTOR_IDENTIFIER_TYPE_XML_LITERAL:
    {
      qDebug() << Q_FUNC_INFO << "RAPTOR_IDENTIFIER_TYPE_XML_LITERAL";
    }
    break;

    default:
      break;
  }
}

RSSParser::RSSParser ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "rssparser" ) );
}

raptor_parser* RSSParser::createParser () const
{
  QByteArray ntr ( "ntriples" );
  raptor_parser* parser = raptor_new_parser ( ntr.data() );
  Q_CHECK_PTR ( parser );

  // Setze an dieser stelle die Methode für die Fehlerbehandlung
  if ( parser )
  {
    RSSParser* me = const_cast<RSSParser*> ( this );
    raptor_set_fatal_error_handler ( parser, me, messageHandler );
    raptor_set_error_handler ( parser, me, messageHandler );
    raptor_set_warning_handler ( parser, me, messageHandler );
  }
  return parser;
}

bool RSSParser::parse ( const QUrl &url ) const
{
  if ( !url.isValid() )
    return false;

  QMutexLocker lock ( &m_mutex );
  raptor_init();
  raptor_parser* parser = createParser ();
  if ( parser )
  {
    RSSParserState data;
    raptor_set_statement_handler ( parser, &data, statementHandler );
    // Parser Url Objekt erstellen
    QByteArray bUrl = url.toString().toUtf8();
    raptor_uri* rssUrl = raptor_new_uri ( ( unsigned char* ) ( bUrl.data() ) );

    if ( raptor_start_parse ( parser, rssUrl ) )
    {
      raptor_free_parser ( parser );
      if ( rssUrl )
      {
        qDebug() << Q_FUNC_INFO << url;
        raptor_free_uri ( rssUrl );
      }
    }
  }
  raptor_finish();
  return true;
}

RSSParser::~RSSParser()
{}
