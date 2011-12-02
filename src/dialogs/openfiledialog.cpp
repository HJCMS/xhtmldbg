/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "openfiledialog.h"

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QVBoxLayout>
#include <QtGui/QFileDialog>

/* KDE */
#include <KDE/KUrl>
#include <KDE/KLocale>
#include <KDE/KIcon>

OpenFileDialog::OpenFileDialog ( const QString &target, QWidget * parent )
    : KFileDialog ( KUrl ( target ), QString ( "*" ), parent )
{
  setObjectName ( QLatin1String ( "OpenFileDialog" ) );
  setWindowTitle ( i18n ( "Open HTML File" ) );
  setOperationMode ( KFileDialog::Opening );

  QStringList filters;
  filters << "text/html" << "text/xml" << "text/plain" << "application/xslt+xml" << "application/x-xbel";
  setMimeFilter ( filters, QString ( "text/html" ) );
}

const QString OpenFileDialog::getDirectory()
{
  QString file;
  file = selectedFiles().first();
  QFileInfo info ( file );
  if ( info.exists() )
    return info.filePath();

  return file;
}

const QUrl OpenFileDialog::getFileUrl()
{
  QUrl url;
  QString path = selectedFiles().first();
  QFileInfo info ( path );
  if ( info.exists() )
  {
    url.setScheme ( "file" );
    url.setPath ( info.absoluteFilePath() );
  }
  return url;
}

OpenFileDialog::~OpenFileDialog()
{}
