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

#include "soupreader.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QUrl>

SoupReader::SoupReader ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "soupreader" ) );
}

/**
* Lese alle Kinder Textknoten aus @param node und
* schreibe diese in einen String in dem Überflüssige
* Zeilenumbrüche und Leerzeichen entfernt werden.
* Die folgenden Kinder Element werden ignoriert!
* @li m:errortype
* @li m:errorsubtype
* @li m:skippedstring
*/
const QString SoupReader::prepareMessage ( const QDomNode &node ) const
{
  QString value;
  if ( ! node.isElement() )
    return value;

  QDomElement element = node.toElement();
  if ( ! element.hasChildNodes() )
    return value;

  QStringList exclude ( "m:errortype" );
  exclude << "m:errorsubtype" << "m:skippedstring";

  QStringList messages ( i18n ( "Line" ) );
  for ( QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling() )
  {
    QDomText t = n.firstChild().toText();
    if ( !t.isNull() && ! exclude.contains ( n.nodeName() ) )
    {
      QString m = t.data().trimmed();
      QString block = m.replace ( QRegExp ( "\\s{2,}|[\\n\\r]" ), " " );
      messages << block;
    }
  }

  if ( messages.size() > 2 )
    return messages.join ( " " );

  return value;
}

/**
* Suche m:errorlist Element
* @code
*   <m:errorlist>...</m:errorlist>
* @endcode
**/
const QDomElement SoupReader::errorListNode ( int index )
{
  QDomNode node = dom.elementsByTagName ( "m:errorlist" ).item ( index );
  if ( node.isElement() )
    return node.toElement();

  return QDomElement();
}

/**
* Suche m:warninglist Element
* @code
*   <m:warninglist>...</m:warninglist>
* @endcode
**/
const QDomElement SoupReader::warningListNode ( int index )
{
  QDomNode node = dom.elementsByTagName ( "m:warninglist" ).item ( index );
  if ( node.isElement() )
    return node.toElement();

  return QDomElement();
}

/**
* Sucht in einem Element nach einem #TEXT Wert.
**/
const QString SoupReader::nodeItem ( const QDomNode &node ) const
{
  QString value;
  if ( ! node.isElement() )
    return value;

  QDomElement childNode = node.toElement();
  if ( childNode.firstChild().isElement() )
  {
    QDomElement nextChild = childNode.firstChild().toElement();
    if ( ! nextChild.nodeValue().isEmpty() )
      return nextChild.nodeValue().trimmed();
  }
  return value;
}

/**
* Suche in einem listen Element nach dem "m:uri" Knoten
* und löse den Dateinamen auf.
*/
const QString SoupReader::cssFileName ( const QDomElement &element ) const
{
  QString file;
  QDomNode node = element.elementsByTagName ( "m:uri" ).item ( 0 );
  if ( ! node.isElement() )
    return file;

  if ( ! node.firstChild().nodeValue().isEmpty() )
  {
    QUrl url ( node.firstChild().nodeValue().trimmed() );
    if ( ! url.path().isEmpty() )
      return i18n ( "File:.%1" ).arg ( url.path () );
  }
  return file;
}

/**
* Suche alle errors im gesamtem dom.
* @code
*   <m:errors>...</m:errors>
* @endcode
**/
void SoupReader::readAllErrors()
{
  QString css = cssFileName ( errorListNode() );
  QDomNodeList nodes = errorListNode().elementsByTagName ( "m:error" );
  for ( int i = 0; i < nodes.count(); i++ )
  {
    QString buffer = prepareMessage ( nodes.item ( i ) );
    if ( buffer.isEmpty() )
      continue;

    if ( ! css.isEmpty() )
      buffer.prepend ( css + " " );

    emit parserError ( buffer );
  }
}

/**
* Suche alle warnings im gesamtem dom.
* @code
*   <m:warnings>...</m:warnings>
* @endcode
**/
void SoupReader::readAllWarnings()
{
  QString css = cssFileName ( warningListNode() );
  QDomNodeList nodes = warningListNode().elementsByTagName ( "m:warning" );
  for ( int i = 0; i < nodes.count(); i++ )
  {
    QString buffer = prepareMessage ( nodes.item ( i ) );
    if ( buffer.isEmpty() )
      continue;

    if ( ! css.isEmpty() )
      buffer.prepend ( css + " " );

    emit warnings ( buffer );
  }
}

/**
* Wenn m:validity true ist dann nicht weiter nach fehlern suchen.
* @code
*   <m:validity>true</m:validity>
* @endcode
**/
bool SoupReader::hasErrors()
{
  QDomNode node = dom.elementsByTagName ( "m:validity" ).item ( 0 );
  if ( node.isElement() && node.toElement().firstChild().nodeValue() == "true" )
  {
    emit congratulation ( i18n ( "Congratulations! No Error Found. (%1)" ).arg ( currenUrl ) );
    return false;
  }
  return true;
}

/**
* Nehme den Datestrom entgegen und füge diesen in ein DomDocument.
* Wenn es zu fehlern kommt sofort abbrechen und ein signal
* @ref parserError abstossen. Ist das Dokument Valiede schreibe
* für die weiteren Nachrichten die Adresse nach @ref currenUrl.
* Prüfe im nächsten Schritt mit @ref hasErrors ob Fehler vorhanden
* sind, wenn ja dann @ref readAllErrors aufrufen und danach weiter
* zu @ref readAllWarnings gehen. Warnungen werden immer ausgelesen!
*/
bool SoupReader::readReceivedXML ( const QByteArray &xml, const QString &url )
{
  QString errorMsg;
  int errorLine;
  int errorColumn;

#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG

  QTemporaryFile fp ( Settings::tempDir () + "/xhtmldbg_soup_XXXXXX.xml" );
  fp.setAutoRemove ( false );
  if ( fp.open () )
  {
    QTextStream stream ( &fp );
    stream << xml;
    fp.close();
  }

#endif

  if ( ! dom.setContent ( xml, &errorMsg, &errorLine, &errorColumn ) )
  {
    QString erdom = QString ( "%1 at Line %2 Column %3" )
                    .arg ( errorMsg, QString::number ( errorLine ), QString::number ( errorColumn ) );
    emit parserError ( erdom );
    return false;
  }

  // vor der Nachrichten generierung die URL setzen.
  currenUrl = url;

  emit beginParsed();

  if ( hasErrors() )
    readAllErrors();

  readAllWarnings();

  return true;
}

SoupReader::~SoupReader()
{}
