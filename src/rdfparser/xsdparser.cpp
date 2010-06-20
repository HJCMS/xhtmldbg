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

#include "xsdparser.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QMutexLocker>
#include <QtCore/QIODevice>

/* QtXml */
#include <QtXml/QDomDocument>

/* QtXmlPatterns */
#include <QtXmlPatterns/QXmlSchemaValidator>

/** @class XsdParserMessageHandler */
XsdParserMessageHandler::XsdParserMessageHandler ( QObject * parent )
    : QAbstractMessageHandler ( parent )
{
  setObjectName ( QLatin1String ( "xsdparsermessagehandler" ) );
}

/**
* Konvertiere die HTML Ausgabe zu einer einfachen Text Meldung.
*/
const QString XsdParserMessageHandler::toPlainText ( const QString &xml ) const
{
  QString ret;
  QDomDocument dom;
  if ( dom.setContent ( xml ) )
  {
    return dom.documentElement().text();
  }
  return ret;
}

/**
* Nehme die Nachricht zum aufbereiten entgegen und sende
* das Signal @ref message an die Klasse @ref XsdParser
*/
void XsdParserMessageHandler::handleMessage ( QtMsgType type, const QString &info,
        const QUrl &url, const QSourceLocation &location )
{
  Q_UNUSED ( type );
  Q_UNUSED ( url );

  QString m = toPlainText ( info );
  QString l = QString::number ( location.line () );
  QString mess = trUtf8 ( "Document at Line %1 (%2)" ).arg ( l, m );
  emit message ( mess );
}

/** @class XsdParser */
XsdParser::XsdParser ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "xsdparser" ) );
}

/**
* Öffne das XML Dokument und übergebe es an den QXmlSchemaValidator
*/
void XsdParser::parseDocument ( const QByteArray &data, const QString &xsd, const QUrl &baseUrl )
{
  QMutexLocker lock ( &m_mutex );
  QFile fp ( xsd );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    if ( ! xmlSchema.load ( &fp, QUrl::fromLocalFile ( xsd ) ) )
      qWarning ( "(XHTMLDBG) XSD Parser Error can not load RSS2 XSD Scheme!" );

    fp.close();
  }
  QXmlSchemaValidator validator ( xmlSchema );
  XsdParserMessageHandler handler;
  connect ( &handler, SIGNAL ( message ( const QString & ) ),
            this, SIGNAL ( errorMessage ( const QString & ) ) );

  validator.setMessageHandler ( &handler );
  if ( validator.validate ( data, baseUrl ) )
    emit noticeMessage ( trUtf8 ( "RSS 2.0 Document is Valid" ) );
  else
    emit errorMessage ( trUtf8 ( "RSS 2.0 Document is NOT Valid" ) );
}

XsdParser::~XsdParser()
{}
