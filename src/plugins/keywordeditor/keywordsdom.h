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

#ifndef KEYWORDSDOM_H
#define KEYWORDSDOM_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

class KeywordsTableItem;

class Q_DECL_EXPORT KeywordsDom : public QDomDocument
{
  private:
    const QString keysName;
    QDomNode rootNode();
    const QDomNodeList keywordNodes() const;

  public:
    explicit KeywordsDom ( const QString &siblings = QString::fromUtf8 ( "keywords" ) );
    const QString defaultKeywords() const;
    void setDefaults ( const QStringList &keys );

    const QList<KeywordsTableItem*> keywordsItemList() const;
    void setKeywords ( const QList<KeywordsTableItem*> &list );

    const QString dump() const;
    bool saveDocument ( const QString &path ) const;
};

#endif
