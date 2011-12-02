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

#include "configurelogfiles.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QList>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidgetItemIterator>

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
  setSizeGripEnabled ( true );

  QWidget* layer = new QWidget ( this );
  QGridLayout* gridLayout = new QGridLayout ( layer );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_treeWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  QStringList labels;
  labels << i18n ( "Directory" ) << i18n ( "Logfile" );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->header()->setResizeMode ( 0, QHeaderView::ResizeToContents );
  m_treeWidget->header()->setResizeMode ( 1, QHeaderView::ResizeToContents );

  gridLayout->addWidget ( m_treeWidget, 0, 0, 1, 3 );

  QAction* m_delete = new QAction ( KIcon ( "edit-table-delete-row" ), i18n ( "Delete" ), m_treeWidget );
  m_treeWidget->insertAction ( 0, m_delete );

  QToolButton* insert = new  QToolButton ( layer );
  insert->setIcon ( KIcon ( "edit-table-insert-row-below" ) );
  gridLayout->addWidget ( insert, 1, 0, 1, 1 );

  m_lineEdit = new QLineEdit ( layer );
  gridLayout->addWidget ( m_lineEdit, 1, 1, 1, 1 );

  QToolButton* openlogdir = new  QToolButton ( layer );
  openlogdir->setIcon ( KIcon ( "document-open" ) );
  openlogdir->setToolTip ( i18n ( "Open Apache Logfiles Directory" ) );
  gridLayout->addWidget ( openlogdir, 1, 2, 1, 1 );

  // add Central Widget
  setMainWidget ( layer );

  // Signals
  connect ( insert, SIGNAL ( clicked() ),
            this, SLOT ( insertClicked() ) );

  connect ( m_delete, SIGNAL ( triggered() ),
            this, SLOT ( removeItemClicked() ) );

  connect ( openlogdir, SIGNAL ( clicked() ),
            this, SLOT ( openLogFileDialog() ) );

  connect ( this, SIGNAL ( applyClicked() ),
            this, SLOT ( writeConfiguration() ) );

  connect ( m_treeWidget, SIGNAL ( itemClicked ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemChecked ( QTreeWidgetItem *, int ) ) );

  readConfiguration();
}

/** Liste aller Verzeichnis Einträge */
const QStringList ConfigureLogFiles::directories()
{
  QStringList list;
  for ( int i = 0; i < m_treeWidget->topLevelItemCount(); i++ )
  {
    list.append ( m_treeWidget->topLevelItem ( i )->data ( 0, Qt::UserRole ).toString() );
  }
  return list;
}

/** Ist dieses Verzeichnis Ausgewählt ? */
bool ConfigureLogFiles::isScanDirectory ( const QString &path )
{
  for ( int i = 0; i < m_treeWidget->topLevelItemCount(); i++ )
  {
    if ( m_treeWidget->topLevelItem ( i )->data ( 0, Qt::UserRole ).toString() == path )
      return ( m_treeWidget->topLevelItem ( i )->checkState ( 0 ) == Qt::Checked );
  }
  return false;
}

/** Alles außer \param path abwählen */
void ConfigureLogFiles::toggleScanDirectory ( const QString &path )
{
  for ( int i = 0; i < m_treeWidget->topLevelItemCount(); i++ )
  {
    if ( m_treeWidget->topLevelItem ( i )->data ( 0, Qt::UserRole ).toString() != path )
      m_treeWidget->topLevelItem ( i )->setCheckState ( 0,  Qt::Unchecked );
  }
}

/** Suche Verzeichnis Eintrag an Hand des Pfades */
QTreeWidgetItem* ConfigureLogFiles::logDirItem ( const QString &path )
{
  for ( int i = 0; i < m_treeWidget->topLevelItemCount(); i++ )
  {
    if ( m_treeWidget->topLevelItem ( i )->data ( 0, Qt::UserRole ).toString() == path )
      return m_treeWidget->topLevelItem ( i );
  }
  return m_treeWidget->invisibleRootItem();
}

/** Verzeichnis einfügen und bei bedarf selektieren */
void ConfigureLogFiles::insertDirItem ( const QString &data, bool checked )
{
  QFileInfo info ( data );
  if ( info.isReadable() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( m_treeWidget, QTreeWidgetItem::UserType );
    item->setChildIndicatorPolicy ( QTreeWidgetItem::ShowIndicator );
    item->setData ( 0, Qt::DecorationRole,  KIcon ( "inode-directory" ) );
    item->setData ( 0, Qt::DisplayRole, info.absoluteFilePath() );
    item->setData ( 0, Qt::UserRole, info.absoluteFilePath() );
    item->setCheckState ( 0, ( checked ? Qt::Checked : Qt::Unchecked ) );
    m_treeWidget->addTopLevelItem ( item );
    insertFileItems ( item, info );
  }
  else
    m_lineEdit->setFocus();
}

/** Logdateien von Verzeichnis einfügen
* \li Scanne nach Datein mit Endungen *_log oder *.log
* \li Wenn keine Leseberechtigung, dann Ignorieren!
*/
void ConfigureLogFiles::insertFileItems ( QTreeWidgetItem* parent, const QFileInfo &info ) const
{
  QFileInfo fileInfo;
  QStringList filter;
  filter << "*_log" << "*.log";

  QDir dir ( info.absoluteFilePath() );
  dir.setNameFilters ( filter );

  foreach ( QString log, dir.entryList ( QDir::Files, QDir::Name ) )
  {
    fileInfo.setFile ( dir, log );
    if ( ! fileInfo.isReadable() )
      continue;

    QTreeWidgetItem* item = new QTreeWidgetItem ( QTreeWidgetItem::UserType );
    item->setData ( 0, Qt::DisplayRole, fileInfo.fileName() );
    item->setData ( 0, Qt::DecorationRole,  KIcon ( "text-x-log" ) );
    item->setData ( 0, Qt::UserRole, fileInfo.absoluteFilePath() );
    item->setCheckState ( 0, Qt::Unchecked );
    item->setData ( 1, Qt::DisplayRole, fileInfo.absoluteFilePath() );
    parent->addChild ( item );
  }
}

/** Logdatei Eintrag als Selektiert markieren */
void ConfigureLogFiles::setChecked ( const QString &log )
{
  QTreeWidgetItemIterator it ( m_treeWidget, QTreeWidgetItemIterator::UserFlag );
  while ( *it )
  {
    if ( ( *it )->data ( 0, Qt::UserRole ).toString() == log )
    {
      ( *it )->setCheckState ( 0, Qt::Checked );
      break;
    }
    ++it;
  }
}

/** Konfiguration einlesen */
void ConfigureLogFiles::readConfiguration()
{
  QSettings cfg ( this );
  m_treeWidget->clear();
  QString scandir = cfg.value ( "ApacheLoggerPlugin/scandir", "/fake" ).toString();

  foreach ( QString d, cfg.value ( "ApacheLoggerPlugin/directories" ).toStringList() )
  {
    insertDirItem ( d, ( ( scandir == d ) ? true : false ) );
  }
  if ( m_treeWidget->topLevelItemCount() > 0 )
  {
    foreach ( QString log, cfg.value ( "ApacheLoggerPlugin/logs" ).toStringList() )
    {
      setChecked ( log );
    }
  }
}

/** Konfiguration Komplett neu Schreiben */
void ConfigureLogFiles::writeConfiguration()
{
  QStringList logs;
  QStringList dirs = directories();

  QSettings cfg ( this );
  cfg.remove ( "ApacheLoggerPlugin/directories" );
  cfg.remove ( "ApacheLoggerPlugin/logs" );
  cfg.remove ( "ApacheLoggerPlugin/scandir" );

  if ( dirs.size() < 1 )
    return;

  foreach ( QString d, dirs )
  {
    if ( isScanDirectory ( d ) )
      cfg.setValue ( "ApacheLoggerPlugin/scandir", d );

    logs.append ( logFiles ( d ) );
  }

  if ( dirs.size() > 0 )
    cfg.setValue ( "ApacheLoggerPlugin/directories", dirs );

  if ( logs.size() > 0 )
    cfg.setValue ( "ApacheLoggerPlugin/logs", logs );
}

/** Dialog zum öffnen eines Verzeichnisses */
void ConfigureLogFiles::openLogFileDialog()
{
  QString f = KFileDialog::getExistingDirectory ( KUrl ( "kfiledialog:///" ), this,
              i18n ( "Open Apache Log-directory" ) );
  if ( ! f.isEmpty() )
    m_lineEdit->setText ( f );
}

/** Wenn einfügen an geklickt wurde */
void ConfigureLogFiles::insertClicked()
{
  if ( m_lineEdit->text().isEmpty() )
    return;

  insertDirItem ( m_lineEdit->text() );
}

void ConfigureLogFiles::removeItemClicked()
{
  QTreeWidgetItem* item = m_treeWidget->currentItem();
  if ( item->checkState ( 0 ) == Qt::Checked )
  {
    QMessageBox::critical ( this, i18n ( "Failure" ), i18n ( "You can not remove a selected scan directory!" ) );
    return;
  }

  int c = item->childCount();
  if ( c > 0 )
  {
    for ( int i = 0; i < c; i++ )
      item->removeChild ( item->child ( i ) );
  }
  item->setSelected ( false );
  m_treeWidget->removeItemWidget ( item, 0 );
  delete item;
}

void ConfigureLogFiles::itemChecked ( QTreeWidgetItem * item, int )
{
  if ( item->childCount() < 1 )
    return;

  toggleScanDirectory ( item->data ( 0, Qt::UserRole ).toString() );
}

/** Alle ausgewählten Logdateien von Verzeichnis auflisten */
const QStringList ConfigureLogFiles::logFiles ( const QString &path )
{
  QStringList files;
  QTreeWidgetItem* root = logDirItem ( path );
  for ( int i = 0; i < root->childCount(); ++i )
  {
    if ( ( root->checkState ( 0 ) == Qt::Checked )
            && ( root->child ( i )->checkState ( 0 ) == Qt::Checked ) )
      files.append ( root->child ( i )->data ( 0, Qt::UserRole ).toString() );
  }
  return files;
}

ConfigureLogFiles::~ConfigureLogFiles()
{}
