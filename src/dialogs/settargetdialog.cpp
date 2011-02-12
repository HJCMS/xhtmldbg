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

#include "settargetdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>

/* KDE */
#include <KDE/KLocale>

static inline const QString defaultDestdir()
{
  return QDesktopServices::storageLocation ( QDesktopServices::TempLocation );
}

SetTargetDialog::SetTargetDialog ( const QUrl &url, QWidget * parent )
    : QFileDialog ( parent, i18n ( "Output Directory" ), defaultDestdir() )
    , fromUrl ( url )
    , lastSelection ( QString::null )
{
  setObjectName ( QLatin1String ( "settargetdialog" ) );
  // Optionen modifizieren
  setMinimumWidth ( 250 );
  setViewMode ( QFileDialog::Detail );
  setAcceptMode ( QFileDialog::AcceptSave );
  setFileMode ( QFileDialog::AnyFile );
  setFilter ( ( filter() & ~QDir::System ) );

  // Ein paar Optionale Erweiterungs Filter setzen.
  QStringList filters;
  filters << i18n ( "Any files (*)" );
  filters << i18n ( "%1 files %2" ).arg ( i18n ( "Compression" ), "*.tar* *.gz *.bz2 *.xz *.lzma" );
  filters << i18n ( "%1 files %2" ).arg ( i18n ( "Image" ), "*.png *.xpm *.jpg *.gif" );
  filters << i18n ( "%1 files %2" ).arg ( "video/mpeg", "*.mpeg *.mpg *.mpe *.m1v *.m2v *.vob" );
  filters << i18n ( "%1 files %2" ).arg ( "ogg/theora", "*.ogv *.ogg *.ogm" );
  filters << i18n ( "%1 files %2" ).arg ( "Quicktime", "*.qt *.mov *.moov *.qtvr" );
  filters << i18n ( "%1 files %2" ).arg ( "Markup", "*.html *.xhtml *.xml *.xslt *.rss *.ts" );
  filters << i18n ( "%1 files %2" ).arg ( "Text", "*.txt *.log *.text" );
  setNameFilters ( filters );

  // Datei Namen von Url ermitteln und zu vor alles Überflüssige entfernen !
  QUrl urlStripped ( fromUrl.toString ( ( QUrl::RemoveQuery | QUrl::RemoveFragment ) ),
                     QUrl::TolerantMode );

  QString fileName = urlStripped.path().split ( "/" ).last();
  if ( ! fileName.isEmpty() )
  {
    // Setze den Dateinamen in das QLineEdit
    selectFile ( fileName );

    // Wenn nicht in der Historie vorhanden dann in einfügen.
    if ( ! history().contains ( fileName ) )
    {
      QStringList hist = history();
      hist << fileName;
      setHistory ( hist );
    }
  }

  connect ( this, SIGNAL ( currentChanged ( const QString & ) ),
            this, SLOT ( checkPermissions ( const QString & ) ) );
}

/**
* Wenn der Benutzer in eine anderes Verzeichnis wechselt dann
* nachsehen ob diese Datei nicht existiert oder Überschreibbar ist!
* Wenn ja - dann in @ref lastSelection schreiben und das Signal
* @ref destinationChanged abstossen.
*/
void SetTargetDialog::checkPermissions ( const QString &path )
{
  QDir dir ( path );
  QString name = selectedFiles().first();
  // Nichts machen wenn keine Ausgabedatei gesetzt worden ist.
  if ( name.isEmpty() )
    return;

  QFileInfo info ( dir, name );
  if ( ! info.exists() || info.isWritable() )
  {
    lastSelection = info.absoluteFilePath();
    emit destinationChanged ( lastSelection );
  }
  else
    qWarning ( "Permission Denied %s", qPrintable ( info.absoluteFilePath() ) );
}

/**
* Ausgabe Pfad für das Ziel erstellen.
* @li Es wird das Schema file vorgesetzt.
* @li Nachsehen ob @ref lastSelection gesetzt ist wenn ja verwenden.
* @li Oder dann selectedFiles() verwenden.
*/
const QUrl SetTargetDialog::destination()
{
  QUrl dest;
  dest.setScheme ( QLatin1String ( "file" ) );
  if ( lastSelection.isEmpty() )
    dest.setPath ( selectedFiles().first() );
  else
    dest.setPath ( lastSelection );

  return dest;
}

SetTargetDialog::~SetTargetDialog()
{
  lastSelection.clear();
}
