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

#include "directorychooser.h"
#include "directorydialog.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>

DirectoryChooser::DirectoryChooser ( const QString &title, const QString &key, QWidget * parent )
    : QGroupBox ( parent )
    , configKey ( key )
{
  setObjectName ( QLatin1String ( "directorychooser" ) );
  setTitle ( title );
  setContentsMargins ( 0, 15, 0, 10 );
  setFlat ( true );

  Settings cfg ( this );

  QHBoxLayout* layout = new QHBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "directorychooser.layout" ) );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setObjectName ( QLatin1String ( "directorychooser.layout.linedit" ) );
  m_lineEdit->setText ( cfg.value ( configKey ).toString() );
  layout->addWidget ( m_lineEdit );

  m_toolButton = new QToolButton ( this );
  m_toolButton->setObjectName ( QLatin1String ( "directorychooser.layout.toolbutton" ) );
  m_toolButton->setIcon ( QIcon::fromTheme ( QString::fromUtf8 ( "document-open" ) ) );
  layout->addWidget ( m_toolButton );

  setLayout ( layout );

  connect ( m_lineEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( updateDirectory ( const QString & ) ) );

  connect ( m_toolButton, SIGNAL ( clicked () ),
            this, SLOT ( openDialogClicked () ) );
}

/**
* Hier wird der Verzeichnis öffnen Dialog ausgeführt!
*/
void DirectoryChooser::openDialog ()
{
  QDir d;
  if ( directory.isEmpty() )
    d.setPath ( QDesktopServices::storageLocation ( QDesktopServices::HomeLocation ) );
  else
    d.setPath ( directory );

  DirectoryDialog* diag = new DirectoryDialog ( d.absolutePath(), this );
  if ( diag->exec() == QDialog::Accepted )
    setDirectory ( diag->destination() );

  delete diag;
}

/**
* Wird von @ref m_lineEdit mit dem Signal @em textChanged aufgerufen.
* Es wird eine Gültigkeitsprüfung @ref isDirectory gemacht und danach
* @ref directory neu geschrieben und das Signal @ref directoryChanged
* abgestoßen.
*/
void DirectoryChooser::updateDirectory ( const QString &p )
{
  if ( isDirectory ( p ) )
  {
    directory = QDir ( p ).absolutePath();
    emit directoryChanged ( directory );
    emit modified ( true );
  }
}

/**
* Umleitung zu @ref openDialog
*/
void DirectoryChooser::openDialogClicked ()
{
  openDialog();
}

/**
* Überprüft ob es ein Verzeichnis ist und ob die
* Zurgiffs Berechtigungen für das Lesen vorhanden sind.
*/
bool DirectoryChooser::isDirectory ( const QString &path ) const
{
  QDir d ( path );
  if ( d.exists() )
  {
    QFileInfo info ( d.absolutePath() );
    return info.isReadable();
  }
  return false;
}

/**
* Wenn @ref directory nicht leer ist wird dies
* zurück gegeben andernfalls der Inhalt von @ref m_lineEdit
*/
const QString DirectoryChooser::getDirectory()
{
  if ( directory.isEmpty() )
    return m_lineEdit->text();
  else
    return directory;
}

/**
* Nimmt ein Verzeichnis Pfad und Überprüft gleichzeitig
* ob es sich um ein Lesbares Verzeichnis handelt!
* Wenn ja wird @ref directory und @ref m_lineEdit gesetzt.
*/
void DirectoryChooser::setDirectory ( const QString &p )
{
  if ( isDirectory ( p ) )
  {
    directory = p;
    m_lineEdit->setText ( p );
  }
}

DirectoryChooser::~DirectoryChooser()
{}
