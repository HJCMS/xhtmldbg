/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "sourceview.h"
#include "highlighter.h"
#include "contextmenu.h"

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
#include <QtGui/QBrush>
#include <QtGui/QCursor>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QShortcut>
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
  setLineWrapMode ( QTextEdit::NoWrap );
  setCurrentFont ( font );
  document()->setDefaultFont ( font );

  // Context Menu
  editMenu = createStandardContextMenu();
  /* FIXME It is recomment to set ObjectName for the MainMenu
  and then delete QUnicodeControlCharacterMenu Object */
  editMenu->setObjectName ( "primarycontextmenu" );
  foreach ( QMenu *m, editMenu->findChildren<QMenu*>() )
  {
    if ( m->objectName().isEmpty() )
      delete m;
  }
  m_contextMenu = new ContextMenu ( this );
  editMenu->addMenu ( m_contextMenu );

  QTextCharFormat format = currentCharFormat();
  format.setVerticalAlignment ( QTextCharFormat::AlignMiddle );

  m_highlighter = new Highlighter ( document() );

  // Save ShortCut
  QShortcut* sc_save = new QShortcut( this );
  sc_save->setKey( QKeySequence( QKeySequence::Save ) );
  connect ( sc_save, SIGNAL ( activated() ), this, SLOT ( saveSource() ) );
  // Print ShortCut
  QShortcut* sc_print = new QShortcut( this );
  sc_print->setKey( QKeySequence( QKeySequence::Print ) );
  connect ( sc_print, SIGNAL ( activated() ), this, SLOT ( printSource() ) );
  // WrapMode ShortCut
  QShortcut* sc_wrap = new QShortcut( this );
  sc_wrap->setKey( QKeySequence( Qt::Key_F10 ) );
  connect ( sc_wrap, SIGNAL ( activated() ), this, SLOT ( swapWordWrap() ) );

  // Editor Events
  connect ( this, SIGNAL ( cursorPositionChanged() ), this, SLOT ( cursorPosChanged () ) );
  connect ( this, SIGNAL ( textChanged() ), this, SLOT ( setLines() ) );

  // Context Actions
  connect ( m_contextMenu, SIGNAL ( ssave() ), this, SLOT ( saveSource() ) );
  connect ( m_contextMenu, SIGNAL ( sprint() ), this, SLOT ( printSource() ) );
  connect ( m_contextMenu, SIGNAL ( sscheck() ), this, SLOT ( checkSource() ) );
  connect ( m_contextMenu, SIGNAL ( sformat() ), this, SLOT ( formatSource() ) );
  connect ( m_contextMenu, SIGNAL ( swrap() ), this, SLOT ( swapWordWrap() ) );
}

bool SourceView::setBlockWithNumber ( int n )
{
  QTextBlock block = document()->findBlockByLineNumber ( n );
  if ( block.isValid() )
  {
    /* NOTE Block Signals an didn't send cursorPositionChanged */
    blockSignals ( true );
    QString txt = block.text();
    scrollToAnchor ( txt );
    setVisible ( true );

    setFocus ( Qt::ActiveWindowFocusReason );

    QTextCursor cur = textCursor();
    cur.setPosition ( block.position() );
    setTextCursor ( cur );
    blockSignals ( false );
    return true;
  }
  return false;
}

void SourceView::setLines()
{
  int height = cursorRect().height();
  QList<QListWidgetItem*> list;
  int r = document()->lineCount();
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
    emit textChanged ( list );
  }
}

void SourceView::cursorPosChanged()
{
  emit lineChanged ( textCursor().blockNumber() );
}

void SourceView::swapWordWrap()
{
  if ( lineWrapMode() == QTextEdit::NoWrap )
  {
    setLineWrapMode ( QTextEdit::WidgetWidth );
    setWordWrapMode ( QTextOption::WrapAtWordBoundaryOrAnywhere );
  }
  else
  {
    setLineWrapMode ( QTextEdit::NoWrap );
    setWordWrapMode ( QTextOption::ManualWrap );
  }
}

void SourceView::saveSource()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void SourceView::printSource()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void SourceView::checkSource()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void SourceView::formatSource()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void SourceView::contextMenuEvent ( QContextMenuEvent *e )
{
  editMenu->exec ( e->globalPos() );
}

void SourceView::fetchRow ( QListWidgetItem *item )
{
  QList<QVariant> data = item->data ( Qt::UserRole ).toList();
  int row = data.at ( 0 ).toInt();

  if ( row < 1 )
    return;

  if ( setBlockWithNumber ( ( row - 1 ) ) )
    qDebug() << Q_FUNC_INFO << "TODO find() Function";
}

void SourceView::setSource ( const QString &html )
{
  setPlainText ( html );
}

void SourceView::setCursorToRow ( int row )
{
  setBlockWithNumber ( row );
}

const QString SourceView::source()
{
  return toPlainText();
}

SourceView::~SourceView()
{}
