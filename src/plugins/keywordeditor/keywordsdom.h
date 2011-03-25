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
#include <QtCore/QObject>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

class Q_DECL_EXPORT KeywordsDom : public QDomDocument
{
  public:
    explicit KeywordsDom ();
    explicit KeywordsDom ( const QDomDocument & );
    const QDomNode rootNode() const;
    const QDomNode defaultNode() const;
    const QDomNodeList keywordNodes() const;
    const QStringList fileNamesList() const;
};

#endif
