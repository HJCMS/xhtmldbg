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

#ifndef KEYWORDSTABLEITEM_H
#define KEYWORDSTABLEITEM_H

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class Q_DECL_EXPORT KeywordsTableItem
{
  public:
    QString id;
    QString file;
    QStringList words;
    KeywordsTableItem ( const QString &identifier = QString(),
                        const QString &filename = QString(),
                        const QStringList &keywords = QStringList() );
    
};

#endif
