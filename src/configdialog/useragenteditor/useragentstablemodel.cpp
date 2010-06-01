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

#include "useragentstablemodel.h"
#include "useragent.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QFontMetrics>

UserAgentsTableModel::UserAgentsTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
{
  setObjectName ( QLatin1String ( "UserAgentsTableModel" ) );

  agentList.clear();
  /* Zellen anpassen */
  QFontMetrics metrics ( parent->font() );
  m_tableHeader = parent->horizontalHeader();
  m_tableHeader->setCascadingSectionResizes ( true );
  m_tableHeader->setStretchLastSection ( true );
  m_tableHeader->setMouseTracking ( true );
  m_tableHeader->setMinimumSectionSize ( metrics.boundingRect ( "Product Tokens" ).width() );
  m_tableHeader->setDefaultAlignment ( Qt::AlignLeft );
  m_tableHeader->setResizeMode ( QHeaderView::ResizeToContents );
  connect ( m_tableHeader, SIGNAL ( entered ( const QModelIndex & ) ),
            this, SLOT ( displayHeaderInfo ( const QModelIndex & ) ) );
}

void UserAgentsTableModel::displayHeaderInfo ( const QModelIndex &index )
{
  switch ( index.column() )
  {
    case 0:
    {
      m_tableHeader->setToolTip ( trUtf8 ( "Bookmark Entry for Context Menue" ) );
      return;
    }

    case 1:
    {
      m_tableHeader->setToolTip ( trUtf8 ( "User-Agent" ) );
      return;
    }

    case 2:
    {
      m_tableHeader->setToolTip ( trUtf8 ( "Product tokens are used to allow communicating applications to identify themselves by software name and version. Most fields using product tokens also allow sub-products which form a significant part of the application to be listed, separated by white space. By convention, the products are listed in order of their significance for identifying the application." ) );
      return;
    }

    default:
    {
      m_tableHeader->setToolTip ( QString() );
      return;
    }
  }
}

Qt::DropActions UserAgentsTableModel::supportedDragActions() const
{
  return Qt::CopyAction;
}

Qt::DropActions UserAgentsTableModel::supportedDropActions() const
{
  return Qt::IgnoreAction;
}

/**
* Editieren Flags setzen
*/
Qt::ItemFlags UserAgentsTableModel::flags ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags defaultFlags  = ( Qt::ItemIsDragEnabled |
                                  Qt::ItemIsEnabled |
                                  Qt::ItemIsSelectable |
                                  Qt::ItemIsEditable );
  return defaultFlags;
}

/**
* Model Index ermitteln
*/
QModelIndex UserAgentsTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > agentList.size() )
    return QModelIndex();

  if ( column > 3 )
    return QModelIndex();

  return createIndex ( row, column, agentList.size() );
}

/**
* Datensätze anhängen
*/
void UserAgentsTableModel::addItems ( const QList<UserAgent*> &items )
{
  if ( items.size() > 0 )
  {
    beginInsertRows ( QModelIndex(), agentList.size(), ( agentList.size() + ( items.size() - 1 ) ) );
    agentList << items;
    endInsertRows();
  }
}

/**
* Aktuelle Zeilen Anzahl
*/
int UserAgentsTableModel::rowCount ( const QModelIndex & ) const
{
  return agentList.size();
}

/**
* Aktuelle Spalten Anzahl
*/
int UserAgentsTableModel::columnCount ( const QModelIndex & ) const
{
  return 3;
}

/**
* Datensatz von Zelle
*/
QVariant UserAgentsTableModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( !index.isValid() )
    return val;

  int r = index.row();
  if ( ( agentList.size() <= r ) || ( r < 0 ) )
    return val;

  if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) )
  {
    switch ( index.column() )
    {
      case 0:
        return agentList.at ( r )->getName();

      case 1:
        return agentList.at ( r )->getAgent();

      case 2:
        return agentList.at ( r )->getTokens();

      default:
        return val;
    }
  }
  return val;
}

/**
* Kopfdaten für Spalte
*/
QVariant UserAgentsTableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal )
  {
    switch ( section )
    {
      case 0:
        return trUtf8 ( "Bookmark" );

      case 1:
        return trUtf8 ( "User-Agent" );

      case 2:
        return trUtf8 ( "Product Tokens" );

      default:
        return trUtf8 ( "Unknown" );
    }
  }
  // Vertikaler eintrag
  return QString ( "%1" ).arg ( section );
}

/**
* Einen Eintrag einfügen
*/
void UserAgentsTableModel::addItem ( const QModelIndex &parent )
{
  UserAgent* item = new UserAgent ( this );
  beginInsertRows ( parent, agentList.size(), agentList.size() );
  agentList << item;
  endInsertRows();
  emit agentChanged ( true );
}

/**
* Einen Eintrag entfernen
*/
void UserAgentsTableModel::removeItem ( const QModelIndex &index )
{
  if ( ( index.row() < 0 ) || ( index.row() > agentList.size() ) )
    return;

  UserAgent* item = agentList.at ( index.row() );
  beginRemoveRows ( index.parent(), index.row(), index.row() );
  agentList.removeAt ( index.row() );
  endRemoveRows();

  item->deleteLater();
  emit agentChanged ( true );
}


/**
* Verschiebe Spalte
*/
void UserAgentsTableModel::moveRowTo ( int from, int to )
{
  if ( ( from < 0 ) || ( to > agentList.size() ) )
    return;

  agentList.move ( from, to );
  foreach ( QModelIndex m, persistentIndexList() )
  {
    for ( int r = 0; r < rowCount ( m ); r++ )
    {
      for ( int c = 0; c < columnCount ( m ); c++ )
        emit update ( index ( r, c, QModelIndex() ) );
    }
  }
}

/**
* Zellendaten Updaten
*/
bool UserAgentsTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool onUpdate = false;
  if ( ! value.isValid() )
    return onUpdate;

  if ( role != Qt::EditRole )
    return onUpdate;

  if ( agentList.size () >= index.row() )
  {
    int r = index.row();
    switch ( index.column() )
    {
      case 0:
      {
        agentList.at ( r )->setName ( value.toString() );
        onUpdate = true;
      }
      break;

      case 1:
      {
        agentList.at ( r )->setAgent ( value.toString() );
        onUpdate = true;
      }
      break;

      case 2:
      {
        agentList.at ( r )->setTokens ( value.toString() );
        onUpdate = true;
      }
      break;

      default:
        onUpdate = false;
        break;
    }
  }
  emit agentChanged ( onUpdate );
  return onUpdate;
}

/**
* Verfügbare Daten Typen
*/
QStringList UserAgentsTableModel::mimeTypes () const
{
  QStringList types;
  types << "text/plain" << "text/html" << "text/xml";
  return types;
}

/**
* Verfügbare Daten Typen bei Drag übermitteln
*/
QMimeData* UserAgentsTableModel::mimeData ( const QModelIndexList &indexes ) const
{
  QMimeData* mimeData = new QMimeData();
  QByteArray encodedData;
  foreach ( QModelIndex index, indexes )
  {
    if ( index.isValid() )
    {
      QString item = data ( index, Qt::DisplayRole ).toString();
      encodedData.append ( item.toAscii() );
    }
  }
  mimeData->setData ( "text/plain", encodedData );
  return mimeData;
}

UserAgentsTableModel::~UserAgentsTableModel()
{
  agentList.clear();
}
