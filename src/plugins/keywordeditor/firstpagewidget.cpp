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
  label1->setText ( i18n ( "This Editor will Help you to Build a individual list of keywords for your website" ) );
  label1->setWordWrap ( true );
  gridLayout->addWidget ( label1, 0, 0, 1, 2 );

  QLabel* label2 = new QLabel ( this );
  label2->setText ( i18n ( "open Keywordfile or click next for create a new file" ) );
  label2->setWordWrap ( true );
  gridLayout->addWidget ( label2, 1, 0, 1, 2 );

  m_fileEdit = new QLineEdit ( this );
  gridLayout->addWidget ( m_fileEdit, 2, 0, 1, 1 );

  QToolButton* m_openFile =  new QToolButton ( this );
  m_openFile->setIcon ( KIcon ( "document-open" ) );
  gridLayout->addWidget ( m_openFile, 2, 1, 1, 1 );

  setLayout ( gridLayout );

  connect ( m_openFile, SIGNAL ( clicked() ),
            this, SLOT ( triggerFileOpenDialog() ) );
}

void FirstPageWidget::triggerFileOpenDialog()
{
  QString f;
  f = KFileDialog::getOpenFileName ( KUrl (), QLatin1String ( "text/xml" ), this, i18n ( "Open Keywords File" ) );
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
