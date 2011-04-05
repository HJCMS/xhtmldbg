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

#include "configurelogfiles.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

/* KDE */
#include <KDE/KFileDialog>
#include <KDE/KLocale>
#include <KDE/KIcon>

ConfigureLogFiles::ConfigureLogFiles ( QWidget * parent )
    : KDialog ( parent )
{
  setObjectName ( QLatin1String ( "ConfigureLogFiles" ) );
  setCaption ( i18n ( "Configure logfiles" ), false );
  setButtons ( ( KDialog::Apply | KDialog::Close ) );

  QWidget* layer = new QWidget ( this );
  QGridLayout* gridLayout = new QGridLayout ( layer );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  gridLayout->addWidget ( m_listWidget, 0, 0, 1, 3 );

  QAction* m_delete = new QAction ( KIcon ( "edit-table-delete-row" ), i18n ( "Delete" ), m_listWidget );
  m_listWidget->insertAction ( 0, m_delete );

  QToolButton* insert = new  QToolButton ( layer );
  insert->setIcon ( KIcon ( "edit-table-insert-row-below" ) );
  gridLayout->addWidget ( insert, 1, 0, 1, 1 );

  m_lineEdit = new QLineEdit ( layer );
  gridLayout->addWidget ( m_lineEdit, 1, 1, 1, 1 );

  QToolButton* openlog = new  QToolButton ( layer );
  openlog->setIcon ( KIcon ( "document-open" ) );
  gridLayout->addWidget ( openlog, 1, 2, 1, 1 );

  // add Central Widget
  setMainWidget ( layer );

  // Signals
  connect ( insert, SIGNAL ( clicked() ),
            this, SLOT ( insertClicked() ) );

  connect ( openlog, SIGNAL ( clicked() ),
            this, SLOT ( openLogFileDialog() ) );

  connect ( this, SIGNAL ( applyClicked() ),
            this, SLOT ( writeConfiguration() ) );

  readConfiguration();
}

void ConfigureLogFiles::insertLogFileItem ( const QString &data )
{
  QFileInfo info ( data );
  if ( info.exists() && info.permission ( QFile::ReadOwner ) )
  {
    QListWidgetItem* item = new QListWidgetItem ( info.fileName(), m_listWidget, QListWidgetItem::UserType );
    item->setData ( Qt::DisplayRole, info.fileName() );
    item->setData ( Qt::EditRole, info.absoluteFilePath() );
    item->setData ( Qt::DecorationRole,  KIcon ( "text-x-log" ) );
    m_listWidget->addItem ( item );
  }
  else
    m_lineEdit->setFocus();
}

void ConfigureLogFiles::readConfiguration()
{
  QSettings cfg ( this );
  m_listWidget->clear();
  foreach ( QString log, cfg.value ( "ApacheLoggerPlugin/logfiles" ).toStringList() )
  {
    insertLogFileItem ( log );
  }
}

void ConfigureLogFiles::writeConfiguration()
{
  QSettings cfg ( this );
  cfg.remove ( "ApacheLoggerPlugin/logfiles" );
  cfg.setValue ( "ApacheLoggerPlugin/logfiles", logFiles() );
}

void ConfigureLogFiles::openLogFileDialog()
{
  QString f = KFileDialog::getOpenFileName ( KUrl ( "kfiledialog:///" ),
              QLatin1String ( "text/plain" ), this, i18n ( "Open Logfile" ) );
  if ( ! f.isEmpty() )
    m_lineEdit->setText ( f );
}

void ConfigureLogFiles::insertClicked()
{
  if ( m_lineEdit->text().isEmpty() )
    return;

  insertLogFileItem ( m_lineEdit->text() );
}

const QStringList ConfigureLogFiles::logFiles()
{
  QStringList files;
  for ( int i = 0; i < m_listWidget->count(); ++i )
  {
    files.append ( m_listWidget->item ( i )->data ( Qt::EditRole ).toString() );
  }
  return files;
}

ConfigureLogFiles::~ConfigureLogFiles()
{}
