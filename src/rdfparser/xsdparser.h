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

#ifndef XSDPARSER_H
#define XSDPARSER_H

/* QtCore */
#include <QtCore/QGlobalStatic>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtXml */
#include <QtXml/QDomDocument>

/* QtXmlPatterns */
#include <QtXmlPatterns/QAbstractMessageHandler>
#include <QtXmlPatterns/QSourceLocation>
#include <QtXmlPatterns/QXmlSchema>

class XsdParserMessageHandler : public QAbstractMessageHandler
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QString toPlainText ( const QString & ) const;

  protected:
    virtual void handleMessage ( QtMsgType, const QString &, const QUrl &, const QSourceLocation & );

  Q_SIGNALS:
    void message ( const QString & );

  public:
    XsdParserMessageHandler ( QObject * parent = 0 );
};

class XsdParser : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QString schemeFile;
    QXmlSchema xmlSchema;

  Q_SIGNALS:
    void noticeMessage ( const QString & );
    void errorMessage ( const QString & );

  public:
    XsdParser ( const QString &xsd, QObject * parent = 0 );
    void parseDocument ( const QDomDocument &, const QUrl & );
    ~XsdParser();
};

#endif
