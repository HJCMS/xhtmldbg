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
#include "bookmarkwriter.h"

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
#include <QtGui/QIcon>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>

BookmarkEditor::BookmarkEditor ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "bookmarkeditor" ) );
  setWindowTitle ( trUtf8 ( "Bookmark Manager[*]" ) );
  setMinimumWidth ( 550 );
  setMinimumHeight ( 450 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( QLatin1String ( "verticallayout" ) );

  QIcon icon;

  QStringList labels ( trUtf8 ( "Headline" ) );
  labels << trUtf8 ( "Hyperlink" );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setObjectName ( QLatin1String ( "treeviewer" ) );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setDragDropMode ( QAbstractItemView::InternalMove );
  m_treeWidget->setDropIndicatorShown ( true );
  vLayout->addWidget ( m_treeWidget );

  QGridLayout* hLayout = new QGridLayout();
  hLayout->setObjectName ( QLatin1String ( "horizontalbottomlayout" ) );

  // Edit Title Label
  hLayout->addWidget ( new QLabel ( trUtf8 ( "Name:" ) ), 0, 0, Qt::AlignRight );

  // LineEdit Title
  m_editTitle =  new QLineEdit ( this );
  m_editTitle->setObjectName ( QLatin1String ( "edittitle" ) );
  m_editTitle->setToolTip ( trUtf8 ( "Folder or Bookmark Title" ) );
  m_editTitle->setStatusTip ( trUtf8 ( "Folder or Bookmark Title" ) );
  hLayout->addWidget ( m_editTitle, 0, 1 );

  // Remove Selected Item
  m_removeButton = new QToolButton ( this );
  m_removeButton->setObjectName ( QLatin1String ( "removebutton" ) );
  m_removeButton->setText ( QLatin1String ( "-" ) );
  m_removeButton->setToolTip ( trUtf8 ( "Remove Selected Item" ) );
  m_removeButton->setStatusTip ( trUtf8 ( "Remove Selected Item" ) );
  m_removeButton->setWhatsThis ( trUtf8 ( "Remove Selected Item" ) );
  m_removeButton->setIcon ( icon.fromTheme ( "format-remove-node" ) );
  hLayout->addWidget ( m_removeButton, 0, 2, Qt::AlignRight );

  // Edit Link Label
  hLayout->addWidget ( new QLabel ( trUtf8 ( "Link:" ) ), 1, 0, Qt::AlignRight );

  // LineEdit HyperLink
  m_editLink =  new QLineEdit ( QLatin1String ( "http://" ), this );
  m_editLink->setObjectName ( QLatin1String ( "editlink" ) );
  m_editLink->setToolTip ( trUtf8 ( "If this Line is Empty a Folder want created." ) );
  m_editLink->setStatusTip ( trUtf8 ( "If this Line is Empty a Folder want created." ) );
  hLayout->addWidget ( m_editLink, 1, 1 );

  // Add Item to Selected MainItem
  m_addButton = new QToolButton ( this );
  m_addButton->setObjectName ( QLatin1String ( "addbutton" ) );
  m_addButton->setText ( QLatin1String ( "+" ) );
  m_addButton->setToolTip ( trUtf8 ( "Add Item to Selected Main Item" ) );
  m_addButton->setStatusTip ( trUtf8 ( "Add Item to Selected Main Item" ) );
  m_addButton->setWhatsThis ( trUtf8 ( "Add Item to Selected Main Item" ) );
  m_addButton->setIcon ( icon.fromTheme ( "format-add-node" ) );
  hLayout->addWidget ( m_addButton, 1, 2, Qt::AlignRight );
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

  connect ( m_treeWidget, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemChanged ( QTreeWidgetItem *, int ) ) );

  connect ( m_treeWidget, SIGNAL ( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ),
            this, SLOT ( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

  connect ( m_removeButton, SIGNAL ( clicked() ), this, SLOT ( removeItemRow() ) );
  connect ( m_addButton, SIGNAL ( clicked() ), this, SLOT ( addNewItemRow() ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
  connect ( m_buttonRestore, SIGNAL ( clicked() ), this, SLOT ( restore() ) );
  connect ( m_buttonSave, SIGNAL ( clicked() ), this, SLOT ( save() ) );
}

void BookmarkEditor::itemChanged ( QTreeWidgetItem *item, int column )
{
  setWindowModified ( item->data ( column, Qt::EditRole ).isValid() );
}

void BookmarkEditor::currentItemChanged ( QTreeWidgetItem * cur, QTreeWidgetItem * prev )
{
  /* This hack check : if drag move and drop was changed */
  if ( ! cur->isSelected() )
    return;

  if ( prev )
    setWindowModified ( true );
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
  QTreeWidgetItem* item = m_treeWidget->currentItem();
  if ( ! item )
    return;

  if ( ! item->isSelected() )
    return;

  delete item;
}

void BookmarkEditor::addNewItemRow()
{
  QTreeWidgetItem* mainItem = m_treeWidget->currentItem();
  if ( ! mainItem->isSelected() )
    return;

  if ( mainItem->columnCount() == 2 )
  {
    QMessageBox::warning ( this, trUtf8 ( "Warning" ),
                           trUtf8 ( "I didn't add Bookmark in to other Bookmark :-)\nPlease select a Folder." ) );
    return;
  }

  QTreeWidgetItem* item;
  QIcon icon = QIcon::fromTheme ( QLatin1String ( "bookmarks" ) );
  QString title = m_editTitle->text();
  if ( title.isEmpty() )
    return;

  QString link = m_editLink->text();

  if ( link.isEmpty() )
  {
    item = new QTreeWidgetItem ( mainItem );
    item->setData ( 0, Qt::UserRole, "folder" );
    item->setIcon ( 0, icon );
    item->setText ( 0, title );
  }
  else
  {
    // Disable Drop Indicator for Bookmark Items
    Qt::ItemFlags flags = ( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled );
    item = new QTreeWidgetItem ( mainItem );
    item->setData ( 0, Qt::UserRole, "bookmark" );
    item->setFlags ( flags );
    item->setIcon ( 0, icon );
    item->setText ( 0, title );
    item->setText ( 1, link );
  }
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
  BookmarkWriter writer ( this, m_treeWidget );
  setWindowModified ( ( writer.save() ) ? false : true );
}

void BookmarkEditor::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
    status = QMessageBox::question ( this, trUtf8 ( "Unsaved Changes" ),
                                     trUtf8 ( "Bookmark Manager found unsaved Changes.\nDo you realy wan to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  if ( status == QMessageBox::Yes )
    accept();
}

BookmarkEditor::~BookmarkEditor()
{}
