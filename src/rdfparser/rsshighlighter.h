/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2011
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

#ifndef RSSHIGHLIGHTER_H
#define RSSHIGHLIGHTER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QTextDocument>
#include <QtGui/QTextCharFormat>
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextBlock>

class RSSHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    int block_stat;
    struct RuleHighlight
    {
      QRegExp pattern;
      QTextCharFormat format;
    };
    QVector<RuleHighlight> highlightRules;

    QTextCharFormat otherFormat;
    QTextCharFormat elementFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;

  protected:
    void highlightBlock ( const QString &text );

  public:
    RSSHighlighter ( QTextDocument *parent = 0 );
    virtual ~RSSHighlighter();

};

#endif
