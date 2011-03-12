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

#include "localsource.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

LocalSource::LocalSource ( const QString &path )
    : src ( path )
{}

const LocalSource LocalSource::localSource ( const QUrl &filePath )
{
  QString p;
  QFileInfo info ( filePath.path() );
  if ( info.exists() )
    p = info.absoluteFilePath();
  else
    p = QString::null;

  LocalSource sources ( p );
  return sources;
}

const QString LocalSource::source()
{
  QString data;
  if ( src.isNull() )
    return data;

  if ( ! src.contains ( QRegExp ( "\\.(html|dtd|txt|xml|xsl[t]?)$", Qt::CaseInsensitive ) ) )
    return data;

  QFile fp ( src );
  if ( fp.open ( ( QIODevice::ReadOnly | QIODevice::Text ) ) )
  {
    QTextStream rc ( &fp );
    QByteArray buffer = rc.device()->readAll();
    if ( ! buffer.isEmpty() )
    {
      QTextCodec* codec = QTextCodec::codecForHtml ( buffer, QTextCodec::codecForName ( "UTF-8" ) );
      data = codec->toUnicode ( buffer );
    }
    fp.close();
  }
  return data;
}

LocalSource::~LocalSource()
{}
