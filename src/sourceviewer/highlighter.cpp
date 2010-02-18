
#include "highlighter.h"

#include <QRegExp>
#include <QString>
#include <QDebug>

Highlighter::Highlighter ( QTextDocument *parent )
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
  step2.pattern = QRegExp ( "(\\<\\/?\\w+(\\s+\\b|\\>))|(\\<(\\w+|\\s?\\/)\\>)|(\\B\\>)" );
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

  RuleHighlight step6; // Comments 2
  step6.pattern = QRegExp ( "(\\/\\*(.+)\\*\\/)" );
  step6.format = commentFormat;
  highlightRules.append ( step6 );
}

void Highlighter::highlightBlock ( const QString &text )
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

Highlighter::~Highlighter()
{
}
