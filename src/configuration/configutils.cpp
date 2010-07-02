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

#include "configutils.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QFileDialog>

/**
* Öffne einen QColorDialog und setze die Farbe.
*/
const QString ConfigUtils::openColorDialog ( const QString &current, QWidget * parent )
{
  QColorDialog* dialog = new QColorDialog ( parent );
  dialog->setCurrentColor ( QColor ( current ) );
  if ( dialog->exec() )
    return dialog->selectedColor().name();

  return current;
}

/**
* Dialog zum finden des einer Programm Datei
*/
const QString ConfigUtils::findBinaryDialog ( const QString &searchName,
        QWidget * parent, const QString &path )
{
  QStringList filt;
  filt << trUtf8 ( "Binary *%1*" ).arg ( searchName );
  filt << trUtf8 ( "All %1" ).arg ( "*" );
  QString p = QFileDialog::getOpenFileName ( parent,
              trUtf8 ( "Find Application" ),
              path, filt.join ( ";;" ) );

  QFileInfo db ( p );
  if ( db.exists() )
    return db.absoluteFilePath();

  return searchName;
}

/** Sucht nach einer Datei */
const QString ConfigUtils::findFileDialog ( const QString &path,
        const QStringList &mimeTypes, QWidget * parent )
{
  QStringList filt;
  filt << mimeTypes;
  filt << trUtf8 ( "All %1" ).arg ( "*" );
  QString p = QFileDialog::getOpenFileName ( parent,
              trUtf8 ( "Find File" ),
              path, filt.join ( ";;" ) );

  QFileInfo db ( p );
  if ( db.exists() )
    return db.absoluteFilePath();

  return path;
}

/**
* Dialog für die Suche nach dem Privaten Schlüssel.
*/
const QString ConfigUtils::openPrivateKeyDialog ( const QString &fallback,
        QWidget * parent, const QString &path )
{
  QStringList filt;
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );

  QString p = QFileDialog::getOpenFileName ( parent,
              trUtf8 ( "Open Certificate" ),
              path, filt.join ( ";;" ) );

  QFileInfo db ( p );
  if ( db.exists() )
    return db.absoluteFilePath();

  return fallback;
}

/**
* Dialog für die Suche nach dem Öffentlichen Schlüssel
*/
const QString ConfigUtils::openPuplicKeyDialog ( const QString &fallback,
        QWidget * parent, const QString &path )
{
  QStringList filt;
  filt << trUtf8 ( "PEM or DER Encoding X.509 Format %1" ).arg ( "*.pem *.der *.cert" );
  filt << trUtf8 ( "PKCS#12 Format %1" ).arg ( "*.p12" );

  QString p = QFileDialog::getOpenFileName ( parent,
              trUtf8 ( "Open Certificate" ),
              path, filt.join ( ";;" ) );

  QFileInfo db ( p );
  if ( db.exists() )
    return db.absoluteFilePath();

  return fallback;
}

const QIcon ConfigUtils::folderIcon()
{
  return QIcon::fromTheme ( QLatin1String ( "document-open" ) );
}
