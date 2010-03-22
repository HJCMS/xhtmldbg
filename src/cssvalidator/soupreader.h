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

#ifndef SOUPREADER_H
#define SOUPREADER_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

class SoupReader : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QDomDocument dom;
    QString currenUrl;
    const QString prepareMessage ( const QDomNode & ) const;
    const QDomElement errorListNode ( int index = 0 );
    const QDomElement warningListNode ( int index = 0 );
    const QString nodeItem ( const QDomNode & ) const;
    const QString cssFileName ( const QDomElement & ) const;
    void readAllErrors();
    void readAllWarnings();
    bool hasErrors();

  Q_SIGNALS:
    void beginParsed();
    void congratulation ( const QString & );
    void warnings ( const QString & );
    void parserError ( const QString & );

  public:
    SoupReader ( QObject * parent = 0 );
    bool readReceivedXML ( const QByteArray &, const QString & );
    ~SoupReader();
};

#endif
