/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "sourceview.h"
#include "highlighter.h"

/* QtCore */
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBitmap>
#include <QtGui/QCursor>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QTextOption>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>
#include <QtGui/QTextCharFormat>

SourceView::SourceView ( const QFont &font, QWidget *parent )
    : QTextEdit ( parent )
    , m_highlighter ( 0 )
{
  setObjectName ( "sourceview" );
  setContentsMargins ( 0, 0, 0, 0 );
  setFrameStyle ( QFrame::NoFrame );
  setAcceptRichText ( false );
  setTextInteractionFlags ( Qt::TextEditorInteraction );
  setWordWrapMode ( QTextOption::ManualWrap );
  setAutoFormatting ( QTextEdit::AutoNone );
  setLineWrapMode ( QTextEdit::NoWrap );
  setCurrentFont ( font );
  document()->setDefaultFont ( font );

  QTextCharFormat format = currentCharFormat();
  format.setVerticalAlignment ( QTextCharFormat::AlignMiddle );

  m_highlighter = new Highlighter ( document() );

  connect ( this, SIGNAL ( cursorPositionChanged() ),
            this, SLOT ( updateSelection() ) );

  connect ( this, SIGNAL ( textChanged() ),
            this, SLOT ( setLines() ) );
}

void SourceView::setLines()
{
  int height = cursorRect().height();
  QList<QListWidgetItem*> list;
  int r = document()->blockCount();
  if ( r >= 1 )
  {
    int f = QString::number ( r ).length();
    for ( int i = 1; i < r; i++ )
    {
      QListWidgetItem* item = new QListWidgetItem;
      item->setData ( Qt::DisplayRole, QString ( "%1" ).arg ( i, f, 10, QChar ( '0' ) ) );
      item->setSizeHint ( QSize ( item->font().weight(), height ) );
      list << item;
    }
    emit linesChanged ( list );
  }
}

void SourceView::updateSelection()
{
  setCursorWidth ( 1 );
}

void SourceView::fetchRow ( QListWidgetItem *item )
{
  QList<QVariant> data = item->data ( Qt::UserRole ).toList();
  int row = data.at ( 0 ).toInt();

  if ( row < 1 )
    return;

  QTextCursor cur = textCursor();
  if ( cur.hasSelection() )
  {
    cur.clearSelection();
    cur.setPosition ( 1 );
    setTextCursor ( cur );
  }

  setFocus ( Qt::MouseFocusReason );

  QTextBlock block = document()->findBlockByLineNumber ( ( row - 1 ) );
  if ( block.isValid() )
  {
    QString txt = block.text();
    scrollToAnchor ( txt );
    find ( txt );
  }
}

void SourceView::setSource ( const QString &html )
{
  setPlainText ( html );
}

const QString SourceView::source()
{
  return toPlainText();
}

SourceView::~SourceView()
{}
