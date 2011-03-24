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

#include "keywordswidget.h"
#include "keywordsdom.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>

/* QtXml */
#include <QtXml/QDomDocument>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordsWidget::KeywordsWidget ( QWidget * parent )
    : QWidget ( parent )
    , xml ( 0 )
{
  setObjectName ( QLatin1String ( "KeywordsWidget" ) );
}

bool KeywordsWidget::setContent ( const QString &filePath )
{
  QFile fp ( filePath );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QDomDocument dom;
    QString errorMsg;
    int errorLine;
    if ( dom.setContent ( &fp, true, &errorMsg, &errorLine ) )
    {
      xml = new KeywordsDom ( dom );
      return true;
    }
    else
      qWarning ( "(XHTMLDBG) Can not open Keyword File \"%s\" %d.", qPrintable ( errorMsg ), errorLine );

    return false;
  }
  return false;
}

KeywordsWidget::~KeywordsWidget()
{}
