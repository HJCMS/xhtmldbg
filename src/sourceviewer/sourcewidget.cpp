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

#include "sourcewidget.h"
#include "contextmenu.h"

#include <cstdio>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QSizePolicy>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QVBoxLayout>

/* QTidy */
#include <QTidy/QTidySettings>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KTextEditor/Cursor>
#include <KDE/KTextEditor/Editor>
#include <KDE/KTextEditor/EditorChooser>

SourceWidget::SourceWidget ( QWidget * parent )
    : QWidget ( parent )
    , tidyrc ( QLatin1String ( "~/.qtidyrc" ) )
    , m_document ( 0 )
    , m_view ( 0 )
    , m_parser ( 0 )
{
  setObjectName ( QLatin1String ( "sourcewidget" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setBackgroundRole ( QPalette::NoRole );
  setWindowTitle ( i18n ( "Source" ) );
  setContextMenuPolicy ( Qt::CustomContextMenu );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  KTextEditor::Editor* m_editor = KTextEditor::EditorChooser::editor();
  m_editor->setSimpleMode ( false );

  // KTextEditor::Document
  m_document = m_editor->createDocument ( this );
  m_document->setMode ( QLatin1String ( "XML (Debug)" ) );
  m_document->setHighlightingMode ( QLatin1String ( "HTML" ) );

  // KTextEditor::View
  m_view = m_document->createView ( this );
  vLayout->addWidget ( m_view );

  // Nehme KDE Standard Menü für ContextMenu
  m_menu = new ContextMenu ( this );
  m_menu->setHighlightModes ( m_document->highlightingModes() );
  m_view->setContextMenu ( m_view->defaultContextMenu ( m_menu ) );

  // QTidy::QTidyParser
  m_parser = new QTidy::QTidyParser ( this, getTidyrc() );

  setLayout ( vLayout );

  // Signals
  connect ( m_menu, SIGNAL ( sscheck() ), this, SLOT ( check() ) );
  connect ( m_menu, SIGNAL ( sformat() ), this, SLOT ( format() ) );
  connect ( m_menu, SIGNAL ( ssave() ), this, SLOT ( saveSource() ) );
  connect ( m_menu, SIGNAL ( sprint() ), this, SLOT ( printSource() ) );
  connect ( m_menu, SIGNAL ( sconfig() ), this, SLOT ( openConfig() ) );

  connect ( m_menu, SIGNAL ( updateHighlight ( const QString & ) ),
            this, SLOT ( switchHighlight ( const QString & ) ) );

  connect ( m_view, SIGNAL ( contextMenuAboutToShow ( KTextEditor::View *, QMenu * ) ),
            this, SLOT ( editorMenuEvent ( KTextEditor::View *, QMenu * ) ) );

  connect ( m_parser, SIGNAL ( showSingleDiagnose ( const QTidy::QTidyDiagnosis & ) ),
            this, SIGNAL ( triggered ( const QTidy::QTidyDiagnosis & ) ) );
}

/** TODO Mausmenü aufrufen */
void SourceWidget::editorMenuEvent ( KTextEditor::View *, QMenu * )
{}

/** Standard Dialog für das Speichern des aktuellen Dokumenten Inhaltes. */
void SourceWidget::saveSource()
{
  m_document->documentSaveAs();
}

/** Öffnet den KDE Internen Editor Config Dialog */
void SourceWidget::openConfig()
{
  m_document->editor()->configDialog ( this );
}

/** Standard Drucken Dialog für den Dokumenten Quelltext. */
void SourceWidget::printSource()
{
  QString timestamp = QDateTime::currentDateTime().toString ( "yy-dd-mm-zzz" );
  QPrinter printer ( QPrinter::PrinterResolution );
  printer.setCreator ( QString ( "XHTMLDBG http://hjcms.de" ) );
  printer.setColorMode ( QPrinter::GrayScale );
  printer.setPaperSize ( QPrinter::A4 );
  printer.setPageMargins ( 10, 10, 10, 10, QPrinter::DevicePixel );
  printer.setOutputFormat ( QPrinter::PdfFormat );
  printer.setOutputFileName ( QString::fromUtf8 ( "/tmp/xhtmldbg/html_source_%1.pdf" ).arg ( timestamp ) );
  printer.setPrintRange ( QPrinter::AllPages );
#ifdef Q_WS_X11
  printer.setFontEmbeddingEnabled ( true );
#endif

  if ( ! printer.isValid() )
    return;

  QPrintPreviewDialog dialog ( &printer, this );
  connect ( &dialog, SIGNAL ( paintRequested ( QPrinter * ) ),
            this, SLOT ( updatePrintPreview ( QPrinter * ) ) );

  dialog.exec();
}

/** Druckausgabe Zeichnen */
void SourceWidget::updatePrintPreview ( QPrinter * printer )
{
  QPainter painter;
  if ( ! painter.begin ( printer ) )
    return;

  // Font
  QFont font = m_view->font();
  painter.setFont ( font );

  // metric
  QFontMetrics metric ( font );
  QRect pRect = printer->pageRect();

  QRect lineRect = pRect;
  for ( int l = 0; l < m_document->lines(); l++ )
  {
    QRect brect;
    painter.drawText ( lineRect, Qt::TextWordWrap, m_document->line ( l ), &brect );
    lineRect.setTopLeft ( brect.bottomLeft() );

    if ( brect.bottomLeft().y() >= ( pRect.height() - ( 2 * metric.height() ) ) )
    {
      if ( ! printer->newPage() )
        break;

      lineRect = pRect;
    }
  }
  painter.end();
}

/** ändert die Syntax hervorhebung! */
void SourceWidget::switchHighlight ( const QString &m )
{
  if ( m_document->highlightingModes().contains ( m ) )
    m_document->setHighlightingMode ( m );
}

/** Quelltext einfügen */
void SourceWidget::setSource ( const QString &source )
{
  m_document->setText ( source );
}

/** Nehme Meldung für Zeile und Spalte entgegen. */
void SourceWidget::fetchBlock ( int row, int column )
{
  KTextEditor::Cursor cursor ( row, column );
  m_view->setCursorPosition ( cursor );
}

/** Standard QTidy Quelltextprüfung */
void SourceWidget::check()
{
  emit clearMessages();
  m_parser->checkContent ( m_document->text() );
}

/** Standard QTidy Bereinigung und Quelltextprüfung */
void SourceWidget::format()
{
  QString html = m_document->text();
  if ( html.isEmpty() )
    return;

  QString valid = m_parser->cleanContent ( html );
  if ( valid.isEmpty() )
    return;

  m_document->setText ( valid );
  check();
}

/** Setzt den Quelltext auf ein Leeres HTML! */
void SourceWidget::restore()
{
  setSource ( "<html>\n<head><title>Waiting</title></head>\n<body>\n</body>\n</html>\n" );
}

/** Nehme die Aktuelle ~/.tidyrc Konfiguration
* von @ref QTidy::QTidySettings */
const QString SourceWidget::getTidyrc()
{
  QTidy::QTidySettings rc ( tidyrc );
  return rc.TidyConfig();
}

/** Setze die tidy Konfiguration z.B: ~/.tidyrc */
void SourceWidget::setTidyrc ( const QString &rc )
{
  tidyrc = rc;
}

SourceWidget::~SourceWidget()
{
  if ( m_menu )
    delete m_menu;

  if ( m_document )
    delete m_document;
}
