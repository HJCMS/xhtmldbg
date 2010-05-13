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
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QRectF>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QCursor>
#include <QtGui/QColor>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QShortcut>
#include <QtGui/QTextOption>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextLayout>

SourceView::SourceView ( const QFont &font, QWidget * parent )
    : QTextEdit ( parent )
    , m_highlighter ( 0 )
{
  setObjectName ( "sourceview" );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumWidth ( 100 );
  setMinimumHeight ( 100 );
  setFrameStyle ( QFrame::NoFrame );
  setAcceptRichText ( false );
  setAutoFormatting ( QTextEdit::AutoNone );
  setWordWrapMode ( QTextOption::ManualWrap );
  setLineWrapMode ( QTextEdit::NoWrap );
  setUndoRedoEnabled ( true );
  setCurrentFont ( font );
  document()->setDefaultFont ( font );
  setTextInteractionFlags ( Qt::TextEditorInteraction );

  m_contextMenu = new ContextMenu ( this );

  QTextCharFormat format = currentCharFormat();
  format.setVerticalAlignment ( QTextCharFormat::AlignMiddle );

  m_highlighter = new Highlighter ( document() );

  // if Shortcuts from ContextMenu didn't work ???
  // Save ShortCut
  QShortcut* sc_save = new QShortcut ( this );
  sc_save->setKey ( QKeySequence ( QKeySequence::Save ) );
  connect ( sc_save, SIGNAL ( activated() ), this, SLOT ( saveSource() ) );
  // Print ShortCut
  QShortcut* sc_print = new QShortcut ( this );
  sc_print->setKey ( QKeySequence ( QKeySequence::Print ) );
  connect ( sc_print, SIGNAL ( activated() ), this, SLOT ( printSource() ) );
  // WrapMode ShortCut
  QShortcut* sc_wrap = new QShortcut ( this );
  sc_wrap->setKey ( QKeySequence ( Qt::Key_F10 ) );
  connect ( sc_wrap, SIGNAL ( activated() ), this, SLOT ( swapWordWrap() ) );

  // Editor Events
  connect ( this, SIGNAL ( cursorPositionChanged() ), this, SLOT ( cursorPosChanged () ) );
  connect ( this, SIGNAL ( textChanged() ), this, SLOT ( createListWidgetItems() ) );

  // Context Actions
  connect ( m_contextMenu, SIGNAL ( ssave() ), this, SLOT ( saveSource() ) );
  connect ( m_contextMenu, SIGNAL ( sprint() ), this, SLOT ( printSource() ) );
  connect ( m_contextMenu, SIGNAL ( sscheck() ), this, SIGNAL ( check() ) );
  connect ( m_contextMenu, SIGNAL ( sformat() ), this, SIGNAL ( format() ) );
  connect ( m_contextMenu, SIGNAL ( swrap() ), this, SLOT ( swapWordWrap() ) );

  update();
}

/**
* Dieser SLOT wird von außerhalb für das setzen des Maus Cursors
* auf den Dokumenten Inhalt benötigt.
* @note Zu diesem Zeitraum werden die Signale blockiert damit es nicht
*  zu einer endlosschleife mit @class ListLines kommt.
**/
bool SourceView::setBlockWithNumber ( int row, int column )
{
  Q_UNUSED ( column )
  QTextBlock block = document()->findBlockByLineNumber ( row );
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

/**
* Erstelle die Listeneinträge für @class ListLines Anhand der anzahl
* der Textblöcke. Die höhe der einzelnen Texblöcke muss für die Zeilen-
* umbruch Darstellung mit übergeben werden. Diese Methode wird immer dann
* aufgerufen wenn sich bei den Textblöcken etwas verändert.
* Das verhalten wird mittels dem Signal textChanged() umgesetzt.
*/
void SourceView::createListWidgetItems()
{
  int lineHeight = cursorRect().height();
  QList<QListWidgetItem*> list;
  int r = document()->blockCount();
  if ( r >= 1 )
  {
    int f = QString::number ( r ).length();
    for ( int i = 1; i < r; i++ )
    {
      QTextLayout* tl =  document()->findBlockByNumber ( ( i - 1 ) ).layout();
      int blockHeight = tl->boundingRect().height();

      QString title = QString ( "%1" ).arg ( i, f, 10, QChar ( '0' ) );
      QListWidgetItem* item = new QListWidgetItem;
      item->setData ( Qt::DisplayRole, title );
      item->setData ( Qt::UserRole, i );

      if ( blockHeight > 0 )
      {
        item->setTextAlignment ( Qt::AlignTop );
        item->setSizeHint ( QSize ( item->font().weight(), blockHeight ) );
      }
      else
        item->setSizeHint ( QSize ( item->font().weight(), lineHeight ) );

      list << item;
    }
    emit textChanged ( list );
  }
}

/**
* Wird immer dann aufgerufen wenn der Maus Cursor im Dokument
* gesetzt oder verschoben wird und Übermittelt an @class SourceWidget
* das Signal lineChanged(int) damit in @class ListLines der Eintrag
* für die Zeilen Hervorhebung gesetzt werden kann.
*/
void SourceView::cursorPosChanged()
{
  emit lineChanged ( textCursor().blockNumber() );
}

/**
* Dieser SLOT wird für den Zeilenumbruch über Tastenkürzel "ShortCutKey_F10" aufgerufen.
* @note Durch einen Zeilenumbruch muss @ref createListWidgetItems aufgerufen werden
* damit die Listeneinträge aus @class ListLines sich dem Dokumenten inhalt anpassen.
*/
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
  // @note must be updated
  createListWidgetItems();
}

/**
* Standard Dialog für das Speichern des aktuellen Dokumenten Inhaltes.
*/
void SourceView::saveSource()
{
  QString path;
  QStringList filters;
  filters << trUtf8 ( "Markup Document %1" ).arg ( "*.html *.xhtml *.xml" );
  filters << trUtf8 ( "Text Document %1" ).arg ( "*.txt *.log" );

  path = QFileDialog::getSaveFileName ( this, trUtf8 ( "Save Content to..." ),
                                        QDir::tempPath(), filters.join ( ";;" ) );

  if ( path.isEmpty() )
    return;

  QFileInfo info ( path );
  if ( info.suffix().isEmpty() )
  {
    QString completeFileName = info.baseName() + QLatin1String ( ".html" );
    info.setFile ( info.absolutePath(), completeFileName );
  }

  QFile file ( info.absoluteFilePath() );
  if ( file.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &file );
    stream << toPlainText();
    file.close();
  }
  else
  {
    QString mess = trUtf8 ( "Can't write Source to \"%1\" Permission Denied." )
                   .arg ( info.absoluteFilePath() );
    QMessageBox::critical ( this, trUtf8 ( "To Save Source Operation dropout" ), mess );
  }
}

/**
* Standard Drucken Dialog für den Dokumenten Quelltext.
* @note Beim Aufruf wird der Zeilenumbruch eingeschaltet damit der
*   Druckbreich nicht aus dem Druckbreich heraus läuft.
* FIXME Das muss noch mal komplett Überarbeitet werden :-/
*/
void SourceView::printSource()
{
  QPrinter::PrinterMode mode;
  // buffer old Page Settings
  QTextEdit::LineWrapMode origLineWrapMode = lineWrapMode();
  QTextOption::WrapMode origWrapMode = wordWrapMode();
  int origWrapColumnOrWidth = lineWrapColumnOrWidth();

#ifdef Q_WS_X11
  mode = QPrinter::ScreenResolution;
#else
  mode = QPrinter::PrinterResolution;
#endif

  QPrinter* printer = new QPrinter ( mode );

#ifdef Q_WS_X11
  printer->setFontEmbeddingEnabled ( true );
#endif

  printer->setDocName ( QString ( "XHTMLDBG_Print%1" ).arg ( documentTitle() ) );
  printer->setCreator ( QString ( "xhtmldbg from QTidy Project http://www.hjcms.de" ) );
  printer->setColorMode ( QPrinter::GrayScale );
  printer->setPaperSize ( QPrinter::A4 );
  printer->setPageMargins ( 10, 20, 10, 20, QPrinter::DevicePixel );
  printer->setOutputFormat ( QPrinter::PdfFormat );

  if ( ! printer->isValid() )
  {
    delete printer;
    return;
  }

  // now make document content printable
  QRectF rectf = printer->pageRect ( QPrinter::DevicePixel );
  setLineWrapMode ( QTextEdit::FixedPixelWidth );
  setLineWrapColumnOrWidth ( rectf.width() );
  setWordWrapMode ( QTextOption::WrapAtWordBoundaryOrAnywhere );
  createListWidgetItems();

  QPrintDialog dialog ( printer, this );
  dialog.setEnabledOptions ( QAbstractPrintDialog::PrintToFile );

  if ( dialog.exec() != QDialog::Accepted )
  {
    setLineWrapColumnOrWidth ( origWrapColumnOrWidth );
    setLineWrapMode ( origLineWrapMode );
    setWordWrapMode ( origWrapMode );
    createListWidgetItems();
    return;
  }

  print ( printer );

  setLineWrapColumnOrWidth ( origWrapColumnOrWidth );
  setLineWrapMode ( origLineWrapMode );
  setWordWrapMode ( origWrapMode );
  createListWidgetItems();
}

/**
* Verwende das X-Server Clipboard bei jeder Auswahl
*/
void SourceView::Clipboard()
{
  setEnabled ( ! QApplication::clipboard()->text().isEmpty() );
}

/**
* Erweitertes Menü mit den QTidy und Datei Werkzeugen.
* @note Die Unicode einträge habe ich entfernt!
**/
void SourceView::contextMenuEvent ( QContextMenuEvent *e )
{
  // WARNING For undo redo copy paste the Context Menu must initialed in contextMenuEvent !!!
  editMenu = createStandardContextMenu();
  /* NOTICE It is recomment to set ObjectName for the MainMenu
  and then delete QUnicodeControlCharacterMenu Object */
  editMenu->setObjectName ( "primarycontextmenu" );
  foreach ( QMenu *m, editMenu->findChildren<QMenu*>() )
  {
    if ( m->objectName().isEmpty() )
      delete m;
  }
  editMenu->addMenu ( m_contextMenu );
  editMenu->exec ( e->globalPos() );
}

/**
* Diese Methode nimmt die von QTidy generierten Nachrichten Inhalte an.
* Im Moment wird nur die Zeile gesucht, das setzen der Zeilentiefe muss
* noch umgesetzt werden!
**/
void SourceView::fetchRow ( QListWidgetItem *item )
{
  QList<QVariant> data = item->data ( Qt::UserRole ).toList();
  int row = data.at ( 0 ).toInt();

  if ( row < 1 )
    return;

  // TODO set cursor position to source, used by tidymessanger
  if ( setBlockWithNumber ( ( row - 1 ) ) )
    qDebug() << Q_FUNC_INFO << "TODO find() Function";
}

/**
* Standard Methode für das setzen des Dokumenten Inhaltes!
* Es wird immer Text Blank verwendet kein HTML!
**/
void SourceView::setSource ( const QString &html )
{
  setPlainText ( html );
}

/**
* Im Prinzip das gleiche wie @ref fetchRow nur die
* Übergebenen Paramater haben sich geändert!
**/
void SourceView::setCursorToRow ( int row, int column )
{
  setBlockWithNumber ( row, column );
}

/**
* Standard Methode für das nehmen des Dokumenten Inhaltes!
* Es wird immer Text Blank verwendet kein HTML!
**/
const QString SourceView::source()
{
  return toPlainText();
}

SourceView::~SourceView()
{}
