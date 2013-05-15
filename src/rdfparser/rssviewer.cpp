/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "rssviewer.h"
#include "rsshighlighter.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QFrame>
#include <QtGui/QSizePolicy>
#include <QtGui/QTextOption>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextLayout>

RSSViewer::RSSViewer ( QWidget * parent )
    : QTextEdit ( parent )
    , m_highlighter ( 0 )
{
  setObjectName ( QLatin1String ( "rssviewer" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumWidth ( 100 );
  setMinimumHeight ( 100 );
  setFrameStyle ( QFrame::Box );
  setAcceptRichText ( false );
  setAutoFormatting ( QTextEdit::AutoNone );
  setWordWrapMode ( QTextOption::ManualWrap );
  setLineWrapMode ( QTextEdit::NoWrap );
  setTextInteractionFlags ( Qt::TextEditorInteraction );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );

  QTextCharFormat format = currentCharFormat();
  format.setVerticalAlignment ( QTextCharFormat::AlignMiddle );

  m_highlighter = new RSSHighlighter ( document() );

  update();
}

void RSSViewer::setText ( const QString &source )
{
  setPlainText ( source );
}

RSSViewer::~RSSViewer()
{}
