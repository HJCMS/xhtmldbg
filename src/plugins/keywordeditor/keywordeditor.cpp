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

#include "keywordeditor.h"
#include "firstpagewidget.h"
#include "keywordswidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSize>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QStandardItem>
#include <QtGui/QVBoxLayout>

/* QtXml */
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

/* KDE */
#include <KDE/KPageWidget>
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordEditor::KeywordEditor ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "KeywordEditorMain" ) );
  dialog = new KAssistantDialog;
  dialog->setMinimumWidth ( 480 );
  dialog->setObjectName ( QLatin1String ( "KeywordEditorDialog" ) );
  dialog->setPlainCaption ( i18n ( "SEO Keyword Editor" ) );
  dialog->showButton ( KAssistantDialog::Help, false );
  dialog->setSizeGripEnabled ( true );

  m_firstPageWidget = new FirstPageWidget ( dialog );
  page1 = dialog->addPage ( m_firstPageWidget, i18n ( "Keyword Editor" ) );
  page1->setIcon ( KIcon ( "xhtmldbg" ) );

  m_keywordsWidget = new KeywordsWidget ( dialog );
  page2 = dialog->addPage ( m_keywordsWidget, i18n ( "Editor" ) );
  page2->setIcon ( KIcon ( "xhtmldbg" ) );

  connect ( m_firstPageWidget, SIGNAL ( fileChanged ( const QString & ) ),
            this, SLOT ( fileSelected ( const QString & ) ) );

  connect ( dialog, SIGNAL ( buttonClicked ( KDialog::ButtonCode ) ),
            this, SLOT ( isClicked ( KDialog::ButtonCode ) ) );
}

void KeywordEditor::saveKeywordFile()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void KeywordEditor::isClicked ( KDialog::ButtonCode code )
{
  switch ( code )
  {
    case KDialog::User1:
      saveKeywordFile();
      break; // finished()

    case KDialog::User2:
      break; // next()

    case KDialog::User3:
      break; // back()

    default:
      return;
  };
}

void KeywordEditor::fileSelected ( const QString &path )
{
  if ( path.isEmpty() )
    return;

  if ( m_keywordsWidget->setContent ( path ) )
    dialog->setCurrentPage ( page2 );
}

void KeywordEditor::open()
{
  dialog->show();
}

KeywordEditor::~KeywordEditor()
{}
