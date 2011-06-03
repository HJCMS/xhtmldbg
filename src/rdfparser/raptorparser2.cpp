/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#include "raptorparser.h"

#include <cstring>
#include <limits.h>

/* Raptor */
#include <raptor2.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QMutexLocker>

/* KDE */
#include <KDE/KLocale>

// http://librdf.org/raptor/api/raptor-section-general.html#raptor-log-level
// http://librdf.org/raptor/api/tutorial-parser-set-error-warning-handlers.html
inline void raptorMessanger ( void *ptr, raptor_log_message* message )
{
  if ( message )
  {
#ifdef XHTMLDBG_DEBUG_VERBOSE
    qWarning ( "(XHTMLDBG) RDF Parser: %s", message->text );
#endif
    RaptorParser* p = static_cast<RaptorParser*> ( ptr );
    p->setMessage ( QString::fromUtf8 ( message->text ) );
  }
}

RaptorParser::RaptorParser ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "raptorparser" ) );
}

void RaptorParser::parseDocument ( const QByteArray &data, const QUrl &url )
{
  if ( ! url.isValid() )
    return;

  QMutexLocker lock ( &m_mutex );
  static const int bufSize = 1024;
  raptor_world* world = raptor_new_world();

  // Initialisiere den Nachrichten handler
  RaptorParser* that = const_cast<RaptorParser*> ( this );
  raptor_world_set_log_handler ( world, that, raptorMessanger );

//   if ( ! raptor_world_open ( world ) )
//   {
//     emit errorMessage ( i18n ( "RDF Parser Initialisation failed!" ) );
//     return;
//   }

  const char* ucUrl = url.toString().toLocal8Bit().data();
  raptor_uri* raptorUri = raptor_new_uri ( world, ( const unsigned char* ) ( ucUrl ) );

  // @see http://librdf.org/raptor/api/tutorial-parser-create.html
  raptor_parser* parser = raptor_new_parser_for_content ( world, raptorUri, "application/rdf+xml", NULL, 0, NULL );

  if ( raptor_parser_parse_start ( parser, raptorUri ) )
  {
    raptor_free_parser ( parser );
    if ( raptorUri )
      raptor_free_uri ( raptorUri );

    if ( world )
      raptor_free_world ( world );

    emit noticeMessage ( i18n ( "Abort" ) );
    return;
  }

  // jetzt durchlaufen
  QTextStream stream ( data );
  while ( !stream.atEnd() )
  {
    QString buf = stream.read ( bufSize );
    QByteArray utf8Data = buf.toUtf8();
    if ( raptor_parser_parse_chunk ( parser, ( const unsigned char* ) utf8Data.data(), utf8Data.length(), 0 ) )
    {
      raptor_free_parser ( parser );
      if ( raptorUri )
        raptor_free_uri ( raptorUri );

      if ( world )
        raptor_free_world ( world );

      emit noticeMessage ( i18n ( "Abort" ) );
      return;
    }
  }

  // aufräumen
  raptor_parser_parse_chunk ( parser, NULL, 0, 1 );
  raptor_free_parser ( parser );
  if ( raptorUri )
    raptor_free_uri ( raptorUri );

  if ( world )
    raptor_free_world ( world );

  emit noticeMessage ( i18n ( "Ready" ) );
}

void RaptorParser::setMessage ( const QString &m )
{
  if ( m.isEmpty() )
    return;

  emit errorMessage ( m );
}

RaptorParser::~RaptorParser()
{}

#include "raptorparser.moc"
