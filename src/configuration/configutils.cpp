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

#include "configutils.h"
#include "directorydialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QFileDialog>

/* KDE */
#include <KDE/KColorDialog>
#include <KDE/KDirSelectDialog>
#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KUrl>

/**
* Öffne einen KColorDialog und setze die Farbe.
*/
const QString ConfigUtils::openColorDialog ( const QString &current, QWidget * parent )
{
  KColorDialog* dialog = new KColorDialog ( parent );
  dialog->setDefaultColor ( QColor ( current ) );
  if ( dialog->exec() )
    return dialog->color().name();

  return current;
}

/**
* Dialog zum finden des einer Programm Datei
*/
const QString ConfigUtils::findBinaryDialog ( const QString &searchName,
        QWidget * parent, const QString &path )
{
  KFileDialog kfile ( KUrl ( path ), QString ( "application/x-executable" ), parent );
  kfile.setSelection ( searchName );

  if ( kfile.exec() != QDialog::Accepted )
    return searchName;

  QFileInfo db ( kfile.selectedFile() );
  if ( db.exists() )
    return db.absoluteFilePath();

  return searchName;
}

/** Sucht nach einer Datei */
const QString ConfigUtils::findFileDialog ( const QString &path,
        const QStringList &filters, QWidget * parent )
{
  KFileDialog kfile ( KUrl ( path ), QLatin1String ( "all/allfiles" ), parent );
  foreach ( QString m, filters )
  {
    kfile.setFilter ( m );
  }

  if ( kfile.exec() != QDialog::Accepted )
    return path;

  QFileInfo db ( kfile.selectedFile() );
  if ( db.exists() )
    return db.absoluteFilePath();

  return path;
}

/**
* Dialog zum öffnen eines Verzeichnisses
*/
const QString ConfigUtils::findDirectoryDialog ( QWidget * parent,
        const QString &info, const QString &path )
{
  QDir d ( path );
  if ( ! d.exists() )
    d.setPath ( d.homePath() );

  KDirSelectDialog dialog ( KUrl ( d.path() ), false, parent );
  dialog.setCaption ( info );
  if ( dialog.exec() == QDialog::Accepted )
    return dialog.url().toLocalFile();

  return d.path();
}

/**
* Dialog für die Suche nach dem Privaten Schlüssel.
*/
const QString ConfigUtils::openPrivateKeyDialog ( const QString &fallback,
        QWidget * parent, const QString &path )
{
  QString p = KFileDialog::getOpenFileName ( KUrl ( path ), QString ( "application/x-pkcs12" ),
              parent, i18n ( "Open Private Certificate" ) );

  QFileInfo info ( p );
  if ( info.exists() )
    return info.absoluteFilePath();

  return fallback;
}

/**
* Dialog für die Suche nach dem Öffentlichen Schlüssel
*/
const QString ConfigUtils::openPuplicKeyDialog ( const QString &fallback,
        QWidget * parent, const QString &path )
{
  QString p = KFileDialog::getOpenFileName ( KUrl ( path ), QString ( "application/x-x509-ca-cert" ),
              parent, i18n ( "Open Public Certificate" ) );

  QFileInfo info ( p );
  if ( info.exists() )
    return info.absoluteFilePath();

  return fallback;
}

const QIcon ConfigUtils::folderIcon()
{
  return KIcon ( QLatin1String ( "document-open" ) );
}
