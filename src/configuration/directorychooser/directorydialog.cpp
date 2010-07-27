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

#include "directorydialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>

static inline const QString defaultDestdir ( const QString &p )
{
  if ( p.isEmpty() )
    return QDesktopServices::storageLocation ( QDesktopServices::HomeLocation );
  else
    return p;
}

DirectoryDialog::DirectoryDialog ( const QString &path, QWidget * parent )
    : QFileDialog ( parent, trUtf8 ( "Directory" ), defaultDestdir ( path ) )
    , fromPath ( path )
    , lastSelection ( QString::null )
{
  setObjectName ( QLatin1String ( "directorydialog" ) );
  // Optionen modifizieren
  setMinimumWidth ( 250 );
  setViewMode ( QFileDialog::Detail );
  setAcceptMode ( QFileDialog::AcceptOpen );
  setFileMode ( QFileDialog::Directory );
  setFilter ( ( filter() & ~QDir::Files ) );

  if ( ! fromPath.isEmpty() )
  {
    // Setze den Dateinamen in das QLineEdit
    selectFile ( fromPath );

    // Wenn nicht in der Historie vorhanden dann in einfügen.
    if ( ! history().contains ( fromPath ) )
    {
      QStringList hist = history();
      hist << fromPath;
      setHistory ( hist );
    }
  }

  connect ( this, SIGNAL ( currentChanged ( const QString & ) ),
            this, SLOT ( checkPermissions ( const QString & ) ) );
}

/**
* Wenn der Benutzer in eine anderes Verzeichnis wechselt dann
* nachsehen ob dieses existiert und Lesen Rechte besitzt!
* Wenn ja - dann in @ref lastSelection schreiben und das Signal
* @ref destinationChanged abstossen.
*/
void DirectoryDialog::checkPermissions ( const QString &path )
{
  QFileInfo info ( path );
  if ( ( info.isDir() || info.isSymLink() ) && info.isReadable() )
  {
    // qDebug() << Q_FUNC_INFO << info.absoluteFilePath();
    lastSelection = info.absoluteFilePath();
    emit destinationChanged ( lastSelection );
  }
}

/**
* Ausgabe Pfad erstellen.
* @li Nachsehen ob @ref lastSelection gesetzt ist wenn ja verwenden.
* @li Oder dann selectedFiles().first() verwenden.
*/
const QString DirectoryDialog::destination()
{
  QString dest;
  if ( lastSelection.isEmpty() )
    dest = selectedFiles().first();
  else
    dest = lastSelection;

  return dest;
}

DirectoryDialog::~DirectoryDialog()
{
  lastSelection.clear();
}
