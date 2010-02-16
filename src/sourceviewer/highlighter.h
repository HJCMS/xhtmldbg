/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QTextBlock>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qtidy.hjcms.de" )

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
    Highlighter ( QTextDocument *parent = 0 );
    virtual ~Highlighter();

};

#endif
