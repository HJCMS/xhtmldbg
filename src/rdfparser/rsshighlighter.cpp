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

#include "rsshighlighter.h"

/* QtCore */
#include <QtCore/QDebug>

RSSHighlighter::RSSHighlighter ( QTextDocument *parent )
    : QSyntaxHighlighter ( parent )
    , block_stat ( 0 )
{
  otherFormat.setForeground ( Qt::black );
  elementFormat.setForeground ( Qt::darkBlue );
  attributeFormat.setForeground ( Qt::blue );
  stringFormat.setForeground ( Qt::darkGreen );
  commentFormat.setForeground ( Qt::lightGray );

  RuleHighlight step1; // Strings
  step1.pattern = QRegExp ( "(?!=[\\s\\>\\/])([\\\"\\\'](.*)[\\\"\\\'])" );
  step1.format = stringFormat;
  highlightRules.append ( step1 );

  RuleHighlight step2; // Elements
  step2.pattern = QRegExp ( "(\\<\\/?(\\w+:)?\\w+(\\s+\\b|\\>))|(\\<(\\w+|\\s?\\/)\\>)|(\\B\\>)" );
  step2.format = elementFormat;
  highlightRules.append ( step2 );

  RuleHighlight step3; // Attribute
  step3.pattern = QRegExp ( "(\\s[\\w:-]+(?=[=]))" );
  step3.format = attributeFormat;
  highlightRules.append ( step3 );

  RuleHighlight step4; // Operator
  step4.pattern = QRegExp ( "=" );
  step4.format = otherFormat;
  highlightRules.append ( step4 );

  RuleHighlight step5; // Comments 1
  step5.pattern = QRegExp ( "(\\<!(--|\\[)(.+)(--|\\]){1}\\>)+" );
  step5.format = commentFormat;
  highlightRules.append ( step5 );
}

void RSSHighlighter::highlightBlock ( const QString &text )
{
  if ( text.isEmpty() )
    return;

  setCurrentBlockState ( ++block_stat );

  foreach ( const RuleHighlight &r, highlightRules )
  {
    QRegExp reg ( r.pattern );
    int in = text.indexOf ( reg );
    while ( in >= 0 )
    {
      int le = reg.matchedLength();
      setFormat ( in, le, r.format );
      in = text.indexOf ( reg, in + le );
    }
  }
}

RSSHighlighter::~RSSHighlighter()
{}
