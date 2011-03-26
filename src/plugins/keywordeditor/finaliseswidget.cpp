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

#include "finaliseswidget.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>
#include <KDE/KTextEditor/View>
#include <KDE/KTextEditor/Editor>
#include <KDE/KTextEditor/EditorChooser>

FinalisesWidget::FinalisesWidget ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "FinalisesWidget" ) );
  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  // KTextEditor::Editor
  KTextEditor::Editor* m_editor = KTextEditor::EditorChooser::editor();
  m_editor->setSimpleMode ( true );

  // KTextEditor::Document
  m_document = m_editor->createDocument ( this );
  m_document->setMode ( QLatin1String ( "XML" ) );
  m_document->setHighlightingMode ( QLatin1String ( "XML" ) );

  // KTextEditor::View
  KTextEditor::View* m_view = m_document->createView ( this );
  vLayout->addWidget ( m_view );

  setLayout ( vLayout );
}

void FinalisesWidget::setContent ( const QString &xml )
{
  m_document->setText ( xml.trimmed() );
}

FinalisesWidget::~FinalisesWidget()
{}
