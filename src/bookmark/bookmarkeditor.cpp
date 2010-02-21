/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "bookmarkeditor.h"
#include "bookmarktreereader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLayout>
#include <QtGui/QLabel>

BookmarkEditor::BookmarkEditor ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "bookmarkeditor" ) );
  setWindowTitle ( trUtf8 ( "Bookmark Editor" ) );
  setMinimumWidth ( 500 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( QLatin1String ( "verticallayout" ) );

  QStringList labels ( trUtf8 ( "Name" ) );
  labels << trUtf8 ( "Link" );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setObjectName ( QLatin1String ( "treeviewer" ) );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  vLayout->addWidget ( m_treeWidget );

  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setObjectName ( QLatin1String ( "horizontalbottomlayout" ) );
  hLayout->insertStretch ( 0, 1 );

  m_removeButton = new QToolButton ( this );
  m_removeButton->setObjectName ( QLatin1String ( "removebutton" ) );
  m_removeButton->setEnabled ( false );
  m_removeButton->setText ( QLatin1String ( "-" ) );
  hLayout->insertWidget ( 1, m_removeButton );

  m_addButton = new QToolButton ( this );
  m_addButton->setObjectName ( QLatin1String ( "addbutton" ) );
  m_addButton->setText ( QLatin1String ( "+" ) );
  hLayout->insertWidget ( 2, m_addButton );
  vLayout->addLayout ( hLayout );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( this );
  buttonBox->setObjectName ( QLatin1String ( "buttonBox" ) );
  buttonBox->setOrientation ( Qt::Horizontal );
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonRestore = buttonBox->addButton ( QDialogButtonBox::RestoreDefaults );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );

  vLayout->addWidget ( buttonBox );

  initBookmarkTree();

  connect ( m_removeButton, SIGNAL ( clicked() ), this, SLOT ( removeItemRow() ) );
  connect ( m_addButton, SIGNAL ( clicked() ), this, SLOT ( addNewItemRow() ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( accept() ) );
  connect ( m_buttonRestore, SIGNAL ( clicked() ), this, SLOT ( restore() ) );
  connect ( m_buttonSave, SIGNAL ( clicked() ), this, SLOT ( save() ) );
}

void BookmarkEditor::initBookmarkTree()
{
  QString bfile = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  bfile.append ( QDir::separator() );
  bfile.append ( QLatin1String ( "bookmarks.xbel" ) );
  QFileInfo info ( bfile );
  if ( ! info.exists() )
    bfile = QString::fromUtf8 ( ":/default.xml" );

  QFile fp ( bfile );
  if ( !fp.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  m_treeWidget->clear();
  BookmarkTreeReader treeReader ( m_treeWidget );
  if ( treeReader.read ( &fp ) )
    fp.close();
}

void BookmarkEditor::removeItemRow()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void BookmarkEditor::addNewItemRow()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void BookmarkEditor::restore()
{
  QFile fp ( QString::fromUtf8 ( ":/default.xml" ) );
  if ( !fp.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    return;

  m_treeWidget->clear();
  BookmarkTreeReader treeReader ( m_treeWidget );
  if ( treeReader.read ( &fp ) )
    fp.close();
}

void BookmarkEditor::save()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

BookmarkEditor::~BookmarkEditor()
{}
