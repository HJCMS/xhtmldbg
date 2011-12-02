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

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

BookmarkEditor::BookmarkEditor ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "bookmarkeditor" ) );
  setWindowTitle ( i18n ( "Bookmark Manager[*]" ) );
  setMinimumWidth ( 550 );
  setMinimumHeight ( 450 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( QLatin1String ( "verticallayout" ) );
  vLayout->setContentsMargins ( 5, 5, 5, 15 );

  QStringList labels ( i18n ( "Headline" ) );
  labels << i18n ( "Hyperlink" );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setObjectName ( QLatin1String ( "treeviewer" ) );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setDragDropMode ( QAbstractItemView::InternalMove );
  m_treeWidget->setDropIndicatorShown ( true );
  m_treeWidget->setFrameStyle ( QFrame::Box );
  vLayout->addWidget ( m_treeWidget );

  QGridLayout* hLayout = new QGridLayout();
  hLayout->setObjectName ( QLatin1String ( "horizontalbottomlayout" ) );

  // Edit Title Label
  hLayout->addWidget ( new QLabel ( i18n ( "Name:" ) ), 0, 0, Qt::AlignRight );

  // LineEdit Title
  m_editTitle =  new QLineEdit ( this );
  m_editTitle->setObjectName ( QLatin1String ( "edittitle" ) );
  m_editTitle->setToolTip ( i18n ( "Folder or Bookmark Title" ) );
  m_editTitle->setStatusTip ( i18n ( "Folder or Bookmark Title" ) );
  hLayout->addWidget ( m_editTitle, 0, 1 );

  // Remove Selected Item
  m_removeButton = new QToolButton ( this );
  m_removeButton->setObjectName ( QLatin1String ( "removebutton" ) );
  m_removeButton->setText ( QLatin1String ( "-" ) );
  m_removeButton->setToolTip ( i18n ( "Remove Selected Item" ) );
  m_removeButton->setStatusTip ( i18n ( "Remove Selected Item" ) );
  m_removeButton->setWhatsThis ( i18n ( "Remove Selected Item" ) );
  m_removeButton->setIcon ( KIcon ( "format-remove-node" ) );
  hLayout->addWidget ( m_removeButton, 0, 2, Qt::AlignRight );

  // Edit Link Label
  hLayout->addWidget ( new QLabel ( i18n ( "Link:" ) ), 1, 0, Qt::AlignRight );

  // LineEdit HyperLink
  m_editLink =  new QLineEdit ( QLatin1String ( "http://" ), this );
  m_editLink->setObjectName ( QLatin1String ( "editlink" ) );
  m_editLink->setToolTip ( i18n ( "If this Line is Empty a Folder want created." ) );
  m_editLink->setStatusTip ( i18n ( "If this Line is Empty a Folder want created." ) );
  hLayout->addWidget ( m_editLink, 1, 1 );

  // Add Item to Selected MainItem
  m_addButton = new QToolButton ( this );
  m_addButton->setObjectName ( QLatin1String ( "addbutton" ) );
  m_addButton->setText ( QLatin1String ( "+" ) );
  m_addButton->setToolTip ( i18n ( "Add Item to Selected Main Item" ) );
  m_addButton->setStatusTip ( i18n ( "Add Item to Selected Main Item" ) );
  m_addButton->setWhatsThis ( i18n ( "Add Item to Selected Main Item" ) );
  m_addButton->setIcon ( KIcon ( "format-add-node" ) );
  hLayout->addWidget ( m_addButton, 1, 2, Qt::AlignRight );
  vLayout->addLayout ( hLayout );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( this );
  buttonBox->setObjectName ( QLatin1String ( "buttonBox" ) );
  buttonBox->setOrientation ( Qt::Horizontal );
  m_buttonCancel = buttonBox->addButton ( QDialogButtonBox::Cancel );
  m_buttonClose = buttonBox->addButton ( QDialogButtonBox::Close );
  m_buttonSave = buttonBox->addButton ( QDialogButtonBox::Save );

  vLayout->addWidget ( buttonBox );

  // qDebug() << Q_FUNC_INFO;

  initBookmarkTree();

  connect ( m_treeWidget, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ),
            this, SLOT ( itemChanged ( QTreeWidgetItem *, int ) ) );

  connect ( m_treeWidget, SIGNAL ( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ),
            this, SLOT ( currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * ) ) );

  connect ( m_removeButton, SIGNAL ( clicked() ), this, SLOT ( removeItemRow() ) );
  connect ( m_addButton, SIGNAL ( clicked() ), this, SLOT ( addNewItemRow() ) );
  connect ( m_buttonCancel, SIGNAL ( clicked() ), this, SLOT ( reject() ) );
  connect ( m_buttonClose, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
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

/**
* Leszeichen in QDesktopServices::DataLocation suchen und öffnen.
* Wenn nicht vorhanden nehme das qrc Object.
* Leere das TreeWidget und befülle neu in dem die
* Klasse @ref BookmarkTreeReader aufgerufen wird.
*/
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

/**
* Methode zum entfernen eines Eintrages
*/
void BookmarkEditor::removeItemRow()
{
  QTreeWidgetItem* item = m_treeWidget->currentItem();
  if ( ! item )
    return;

  if ( ! item->isSelected() )
    return;

  delete item;
}

/**
* Methode zum einfügen eines @b neuen Eintrages
*/
void BookmarkEditor::addNewItemRow()
{
  QTreeWidgetItem* mainItem = m_treeWidget->currentItem();
  if ( ! mainItem )
    mainItem = m_treeWidget->invisibleRootItem();

  if ( mainItem->columnCount() == 2 )
  {
    QMessageBox::warning ( this, i18n ( "Warning" ),
                           i18n ( "I didn't add Bookmark in to other Bookmark :-)\nPlease select a Folder." ) );
    return;
  }

  QTreeWidgetItem* item;
  QString title = m_editTitle->text();
  if ( title.isEmpty() )
    return;

  QString link = m_editLink->text();

  if ( link.isEmpty() )
  {
    item = new QTreeWidgetItem ( mainItem );
    item->setFlags ( item->flags() | Qt::ItemIsEditable );
    item->setData ( 0, Qt::UserRole, "folder" );
    item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "bookmark-new" ) ) );
    item->setText ( 0, title );
  }
  else
  {
    // Disable Drop Indicator for Bookmark Items
    Qt::ItemFlags flags = ( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled );
    item = new QTreeWidgetItem ( mainItem );
    item->setData ( 0, Qt::UserRole, "bookmark" );
    item->setFlags ( flags );
    item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "bookmarks" ) ) );
    item->setText ( 0, title );
    item->setText ( 1, link );
  }
}

/**
* Mit @ref BookmarkWriter alles Speichern
*/
void BookmarkEditor::save()
{
  BookmarkWriter writer ( this, m_treeWidget );
  setWindowModified ( ( writer.save() ) ? false : true );
}

/**
* Nachsehen ob es Änderungen gibt und den Benutzer bei
* bedarf Benachrichtigen.
* Wenn keine Änderungen dann Normal beenden.
*/
void BookmarkEditor::quit()
{
  QMessageBox::StandardButton status = QMessageBox::Yes;
  if ( isWindowModified() )
    status = QMessageBox::question ( this, i18n ( "Unsaved Changes" ),
                                     i18n ( "Bookmark Manager found unsaved Changes.\nDo you realy wan to exit?" ),
                                     ( QMessageBox::Cancel | QMessageBox::Yes ), QMessageBox::Cancel );

  if ( status == QMessageBox::Yes )
    accept();
}

/**
* Methode zum einfügen eines Lesezeichens in das Textfeld
*/
void BookmarkEditor::addBookmark ( const QUrl &url, const QString &title )
{
  if ( url.isValid() )
    m_editLink->setText ( url.toString() );

  m_editTitle->setText ( title );
}

BookmarkEditor::~BookmarkEditor()
{}
