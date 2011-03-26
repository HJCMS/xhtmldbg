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

#include "firstpagewidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>

/* KDE */
#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KUrl>

FirstPageWidget::FirstPageWidget ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "FirstPageWidget" ) );
  QGridLayout* gridLayout = new QGridLayout ( this );
  QLabel* label1 = new QLabel ( this );
  label1->setText ( i18n ( "This editor will help you to build an individual keywords listing for your website.<br/>It create an XML Document with following sheme:" ) );
  label1->setWordWrap ( true );
  gridLayout->addWidget ( label1, 0, 0, 1, 2 );

  QLabel* sourceLabel = new QLabel ( this );
  sourceLabel->setStyleSheet ( "*{background-color:white;padding:5px 5px 5px 5px;}" );
  sourceLabel->setWordWrap ( true );
  sourceLabel->setText ( QString::fromUtf8 ( "&lt;?xml version=\"1.0\" encoding=\"utf-8\"?&gt;<br />&lt;tags xmlns=\"http://www.w3.org/2001/XMLSchema/\"&gt;<br /> &lt;default&gt;&lt;![CDATA[fallback,keyword,list]]&gt;&lt;/default&gt;<br /> &lt;keywords id=\"identifier\" file=\"filename.html\"&gt;&lt;![CDATA[xhtmldbg,debugger]]&gt;&lt;/keywords&gt;<br />&lt;/tags&gt;<br />" ) );
  gridLayout->addWidget ( sourceLabel, 1, 0, 1, 2 );

  gridLayout->setRowStretch ( 2, 1 ); //Spacer

  QLabel* label2 = new QLabel ( this );
  label2->setText ( i18n ( "Open existing keyword file or click next to create a new keyword listing:" ) );
  label2->setWordWrap ( true );
  gridLayout->addWidget ( label2, 3, 0, 1, 2 );

  m_fileEdit = new QLineEdit ( this );
  gridLayout->addWidget ( m_fileEdit, 4, 0, 1, 1 );

  QToolButton* m_openFile =  new QToolButton ( this );
  m_openFile->setIcon ( KIcon ( "document-open" ) );
  gridLayout->addWidget ( m_openFile, 4, 1, 1, 1 );

  setLayout ( gridLayout );

  connect ( m_openFile, SIGNAL ( clicked() ),
            this, SLOT ( triggerFileOpenDialog() ) );
}

void FirstPageWidget::triggerFileOpenDialog()
{
  QString f;
  f = KFileDialog::getOpenFileName ( KUrl ( "kfiledialog:///" ), QLatin1String ( "text/xml" ), this, i18n ( "Open Keywords File" ) );
  if ( !f.isEmpty() )
  {
    m_fileEdit->setText ( f );
    emit fileChanged ( f );
  }
}

const QString FirstPageWidget::keywordFile()
{
  return m_fileEdit->text();
}

FirstPageWidget::~FirstPageWidget()
{}
