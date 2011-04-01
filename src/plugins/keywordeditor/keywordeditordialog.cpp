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

#include "keywordeditordialog.h"
#include "firstpagewidget.h"
#include "keywordswidget.h"
#include "finaliseswidget.h"
#include "keywordsdom.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QSizePolicy>

/* KDE */
#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KPageWidget>

KeywordEditorDialog::KeywordEditorDialog ( QWidget * parent )
    : KAssistantDialog ( parent )
{
  setObjectName ( QLatin1String ( "KeywordEditorDialog" ) );
  setMinimumWidth ( 620 );
  setPlainCaption ( i18n ( "SEO Keyword Editor" ) );
  showButton ( KAssistantDialog::Help, false );
  setSizeGripEnabled ( true );
  setWindowModality ( Qt::NonModal );

  m_firstPageWidget = new FirstPageWidget ( this );
  page1 = addPage ( m_firstPageWidget, i18n ( "Open Keyword File" ) );
  page1->setIcon ( KIcon ( "xhtmldbg" ) );

  m_keywordsWidget = new KeywordsWidget ( this );
  page2 = addPage ( m_keywordsWidget, i18n ( "Edit Keyword File" ) );
  page2->setIcon ( KIcon ( "xhtmldbg" ) );

  m_finalisesWidget = new FinalisesWidget ( this );
  page3 = addPage ( m_finalisesWidget, i18n ( "Document Preview" ) );
  page3->setIcon ( KIcon ( "xhtmldbg" ) );

  connect ( m_firstPageWidget, SIGNAL ( fileChanged ( const QString & ) ),
            this, SLOT ( fileSelected ( const QString & ) ) );

  connect ( this, SIGNAL ( currentPageChanged ( KPageWidgetItem *, KPageWidgetItem * ) ),
            this, SLOT ( currentPageChanged ( KPageWidgetItem *, KPageWidgetItem * ) ) );

  connect ( this, SIGNAL ( buttonClicked ( KDialog::ButtonCode ) ),
            this, SLOT ( isClicked ( KDialog::ButtonCode ) ) );
}

void KeywordEditorDialog::saveKeywordFile()
{
  KeywordsDom xml = m_keywordsWidget->getDocument();
  if ( ! xml.isNull() )
  {
    QString f;
    f = KFileDialog::getSaveFileName ( KUrl ( "kfiledialog:///" ), QLatin1String ( "text/xml" ),
                                       this, i18n ( "Save Keywords File" ) );
    if ( ! f.isEmpty() )
      xml.saveDocument ( f );
  }
}

void KeywordEditorDialog::currentPageChanged ( KPageWidgetItem * current, KPageWidgetItem * before )
{
  if ( ( current == page3 ) && ( before == page2 ) )
    m_finalisesWidget->setContent ( m_keywordsWidget->getDocument().dump() );
}

void KeywordEditorDialog::isClicked ( KDialog::ButtonCode code )
{
  if ( code == KDialog::User1 )
  {
    saveKeywordFile();
    return; // finished()
  }
}

void KeywordEditorDialog::fileSelected ( const QString &path )
{
  if ( path.isEmpty() )
    return;

  if ( m_keywordsWidget->setContent ( path, m_firstPageWidget->metaType() ) )
    setCurrentPage ( page2 );
}

KeywordEditorDialog::~KeywordEditorDialog()
{}
