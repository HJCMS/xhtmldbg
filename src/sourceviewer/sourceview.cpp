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
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QFont>
#include <QtGui/QTextOption>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>
#include <QtGui/QTextCharFormat>

SourceView::SourceView ( QWidget *parent )
    : QTextEdit ( parent )
    , m_highlighter ( 0 )
    , tidyrcFile ( 0 )
{
  setObjectName ( "sourceView" );
  setAcceptRichText ( false );
  setTextInteractionFlags ( Qt::TextEditorInteraction );
  setWordWrapMode ( QTextOption::ManualWrap );

  m_highlighter = new Highlighter ( document() );

  connect ( this, SIGNAL ( cursorPositionChanged() ),
            this, SLOT ( updateSelection() ) );
}

void SourceView::documentLines()
{}

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
    cur.setPosition(1);
    setTextCursor( cur );
  }

  setFocus ( Qt::MouseFocusReason );

  QTextBlock block = document()->findBlockByLineNumber( ( row - 1 ) );
  if ( block.isValid() )
  {
    QString txt = block.text();
    scrollToAnchor ( txt );
    find( txt );
  }
}

void SourceView::saveContent ( const QUrl &uri )
{
  QString path = uri.path();
  QFile file ( path );
  if ( file.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &file );
    stream << source();
    file.close();
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
