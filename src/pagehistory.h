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

#ifndef PAGEHISTORY_H
#define PAGEHISTORY_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QUrl>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class PageHistory : private QDomDocument
{
  private:
    inline const QDomElement mimeType();
    inline const QDomElement timeStamp();
    const QDomElement createEntry ( const QUrl &url );

  public:
    PageHistory ();

    /** get Saved History Url's
    * \param filePath Absolute Path to history.xml
    */
    static const QList<QUrl> history ( const QUrl &filePath );

    /** add Page Urls */
    void addEntries ( const QList<QUrl> &list );

    /** save Document
    * \param filePath Absolute Path to history.xml
    */
    bool saveHistory ( const QUrl &filePath );

    virtual ~PageHistory();
};

#endif
