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

#include "keywordswidget.h"
#include "keywordsdom.h"
#include "keywordstable.h"
#include "keywordstableitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QSplitter>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/* QtXml */
#include <QtXml/QDomDocument>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordsWidget::KeywordsWidget ( QWidget * parent )
    : QWidget ( parent )
    , siblingName ( QString::fromUtf8 ( "keywords" ) )
{
  setObjectName ( QLatin1String ( "KeywordsWidget" ) );
  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  QSplitter* m_splitter = new QSplitter ( Qt::Vertical, this );

  // Schlüsselwort Tabelle
  m_table = new KeywordsTable ( m_splitter );
  m_splitter->insertWidget ( 0, m_table );

  // Standard Schlüsselwörter
  QWidget* defaultsWidget = new QWidget ( m_splitter );
  defaultsWidget->setMinimumHeight ( 60 );
  QVBoxLayout* subLayout = new QVBoxLayout ( defaultsWidget );
  defaultsWidget->setLayout ( subLayout );

  subLayout->addWidget ( new QLabel ( i18n ( "fallback keywords for missing sites" ), defaultsWidget ) );
  m_defaultKeywords = new QTextEdit ( defaultsWidget );
  subLayout->addWidget ( m_defaultKeywords );

  m_splitter->insertWidget ( 1, defaultsWidget );
  m_splitter->setCollapsible ( 1, true );

  vLayout->addWidget ( m_splitter );

  setLayout ( vLayout );
}

bool KeywordsWidget::setContent ( const QString &filePath, const QString &type )
{
  QFile fp ( filePath );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    siblingName = type;
    KeywordsDom dom ( type );
    QString errorMsg;
    int errorLine;
    if ( dom.setContent ( &fp, true, &errorMsg, &errorLine ) )
    {
      if ( ! dom.isNull() )
      {
        m_table->setDomDocument ( dom );
        m_defaultKeywords->setPlainText ( dom.defaultKeywords() );
        return true;
      }
    }
    else
      qWarning ( "(XHTMLDBG) Can not open Keyword File \"%s\" %d.", qPrintable ( errorMsg ), errorLine );

    return false;
  }
  return false;
}

const KeywordsDom KeywordsWidget::getDocument()
{
  KeywordsDom dom ( siblingName );
  dom.setDefaults ( m_defaultKeywords->toPlainText().split ( "," ) );
  dom.setKeywords ( m_table->keywords() );
  return dom;
}

KeywordsWidget::~KeywordsWidget()
{}
