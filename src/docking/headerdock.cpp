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

#include "headerdock.h"
#include "networkcookie.h"
#include "xhtmldbgmain.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QMapIterator>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QFrame>
#include <QtGui/QSizePolicy>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTreeWidgetItemIterator>

HeaderDock::HeaderDock ( QWidget * parent )
    : Docking ( parent )
    , minColumnWidth ( 50 )
{
  setObjectName ( "headerviews" );
  setWindowTitle ( trUtf8 ( "Headers" ) );

  QStringList labels;
  labels << trUtf8 ( "Header" ) << trUtf8 ( "Value" );

  // Header Data
  setColumnCount ( 2, 0 );
  setTreeHeaderLabels ( labels, 0 );
  labels.clear();

  // Post Data
  m_treePostVars = new QTreeWidget ( this );
  m_treePostVars->setObjectName ( QLatin1String ( "postvarstreewidget" ) );
  m_treePostVars->setToolTip ( trUtf8 ( "POST Variables" ) );
  addTreeWidget ( m_treePostVars );

  labels << trUtf8 ( "Header" ) << trUtf8 ( "Value" );
  setColumnCount ( 2, 1 );
  setTreeHeaderLabels ( labels, 1 );
  labels.clear();

  // Cookies Data
  m_treeCookies = new QTreeWidget ( this );
  m_treeCookies->setObjectName ( QLatin1String ( "cookiestreewidget" ) );
  m_treeCookies->setToolTip ( trUtf8 ( "Cookies" ) );
  addTreeWidget ( m_treeCookies );
  labels << trUtf8 ( "Name" ) << trUtf8 ( "Value" );
  setColumnCount ( 2, 2 );
  setTreeHeaderLabels ( labels, 2 );
  labels.clear();

  // get CookieJar
  m_networkCookie = xhtmldbgmain::instance()->cookieManager();
}

void HeaderDock::setTreeHeaderLabels ( const QStringList &labels, int index )
{
  Docking::setTreeHeaderLabels ( labels, index );
}

void HeaderDock::setHeaderData ( const QUrl &url, const QMap<QString,QString> &map )
{
  int widgetIndex = 0;
  int minWidth = 0;
  bool typeHtml = false;
  QString host = url.host();
  QTreeWidget* tree = widget ( widgetIndex );
  QTreeWidgetItem* parent;

  if ( map.contains ( "Content-Type" ) )
    typeHtml = QString ( map["Content-Type"] ).contains ( "text/html" );

  // Oberster Eintrag mit Hostnamen
  if ( itemExists ( host, widgetIndex ) )
  {
    parent = tree->topLevelItem ( 0 );
    /* Doppelte Einträge bei Pfad oder Query entfernen und unten neu Schreiben */
    QTreeWidgetItemIterator it ( tree, QTreeWidgetItemIterator::HasChildren );
    while ( *it )
    {
      if ( host != ( *it )->data ( 0, Qt::DisplayRole ) )
        ( *it )->setExpanded ( false ); // Einträge einklappen

      if ( url.path() == ( *it )->data ( 0, Qt::DisplayRole ) )
      {
        parent->removeChild ( ( *it ) );
        tree->sortItems ( 0, Qt::AscendingOrder );
        delete ( *it );
        break;
      }
      ++it;
    }
  }
  else
  {
    clearContent ( widgetIndex );
    parent = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
    parent->setExpanded ( true );
    parent->setData ( 0, Qt::DisplayRole, host );
    parent->setText ( 1, trUtf8 ( "Hostname" ) );
    parent->setForeground ( 1, Qt::lightGray );
  }

  QTreeWidgetItem* queryItem = addTopLevelItem ( parent, widgetIndex );
  queryItem->setData ( 0, Qt::DisplayRole, url.path() );
  queryItem->setExpanded ( typeHtml );
  if ( ! typeHtml )
    queryItem->setForeground ( 0, Qt::lightGray );

  queryItem->setText ( 1, trUtf8 ( "Item" ) );
  queryItem->setForeground ( 1, Qt::lightGray );
  parent->addChild ( queryItem );

  QMapIterator<QString,QString> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    QTreeWidgetItem* item = new QTreeWidgetItem ( queryItem );
    item->setData ( 0, Qt::DisplayRole, it.key() );
    item->setData ( 1, Qt::DisplayRole, it.value() );
    queryItem->addChild ( item );
    int cw = ( fontMetric ( widgetIndex ).width ( it.value() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  setColumnWidth ( 1, minWidth, widgetIndex );
  resizeSections ( widgetIndex );
  tree->scrollToItem ( queryItem, QAbstractItemView::PositionAtTop );
}

void HeaderDock::setPostedData ( const QUrl &url, const QStringList &list )
{
  int widgetIndex = 1;
  int minWidth = 0;

  clearContent ( widgetIndex );

  QTreeWidgetItem* parent = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
  parent->setData ( 0, Qt::DisplayRole, url.host() );
  parent->setText ( 1, url.path() );
  parent->setForeground ( 1, Qt::lightGray );

  if ( url.hasQuery() )
  {
    QList<QPair<QString, QString> > pairs ( url.queryItems() );
    for ( int i = 0; i < pairs.size(); i++ )
    {
      QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
      item->setData ( 0, Qt::DisplayRole, pairs.at ( i ).first );
      item->setForeground ( 0, Qt::lightGray );
      item->setData ( 1, Qt::DisplayRole, pairs.at ( i ).second );
      parent->addChild ( item );
    }
  }

  if ( url.hasFragment() )
  {
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setData ( 0, Qt::DisplayRole, trUtf8 ( "Anchor" ) );
    item->setForeground ( 0, Qt::lightGray );
    item->setData ( 1, Qt::DisplayRole, url.fragment() );
    item->setData ( 1, Qt::ToolTipRole, QLatin1String ( "Fragment" ) );
    parent->addChild ( item );
  }

  foreach ( QString str, list )
  {
    QStringList data = str.split ( QString::fromUtf8 ( "=" ) );
    QTreeWidgetItem* item = new QTreeWidgetItem ( parent );
    item->setData ( 0, Qt::DisplayRole, data.first() );
    item->setData ( 1, Qt::DisplayRole, data.last() );
    parent->addChild ( item );
    int cw = ( fontMetric ( widgetIndex ).width ( data.last() ) + item->font ( 0 ).weight() );
    if ( cw > minWidth )
      minWidth = cw;
  }

  setColumnWidth ( 1, minWidth, widgetIndex );
  resizeSections ( widgetIndex );
}

/**
* Ein von PHP Serialisierten Keks wieder in einen lesbaren
* Text verwandeln.
* @code
    a = Array
    s = String
    i = Integer
    o = Object
    a:3:{s:3:"ID";i:1;s:7:"GID";i:4;s:4:"KEY";s:32:"8b3c3634669c41f26c1a511cb03d6bc0";}
* @endcode
*/
QString HeaderDock::unserialize ( const QByteArray &data ) const
{
  if ( data.contains ( "%" ) )
  {
    QByteArray serial = QByteArray::fromPercentEncoding ( data );
    QString retval ( serial );
    retval.remove ( QRegExp ( "(^.{1,5}\\{)|(\\}$)" ) );
    retval.remove ( QRegExp ( ";$" ) );

    QString output;
    QStringList buffer;
    foreach ( QString p, retval.split ( QRegExp ( ";?[asio]:" ) ) )
    {
      if ( ! p.isEmpty() )
      {
        QString b = p.replace ( "\"", "" );
        buffer << b.split ( QRegExp ( "\\d+:" ) ).last();
      }
    }

    if ( buffer.size() % 2 != 0 )
      buffer << "";

    for ( int i = 0; i < buffer.size(); i += 2 )
    {
      output.append ( buffer.at ( i ) + "=" + buffer.at ( ( i+1 ) ) + "\n" );
    }

    return ( output.isEmpty() ) ?  retval : output;
  }

  return QString ( data );
}

/**
* Durchlaufe alle nutzbaren Cookie Parameter
* und füge diese in ein Item ein.
*/
void HeaderDock::setCookieData ( const QNetworkCookie &cookie, QTreeWidgetItem* parent )
{
  QString yes ( trUtf8 ( "Yes" ) );
  QString no ( trUtf8 ( "No" ) );
  QString values = unserialize ( cookie.value() );
  int minWidth = ( fontMetric().width ( values ) + parent->font ( 0 ).weight() );
  if ( minWidth > minColumnWidth )
  {
    minColumnWidth = minWidth;
    setColumnWidth ( 1, 2 ); // widgetIndex= 2
  }

  if ( cookie.name() == QString::fromUtf8 ( "XDEBUG_SESSION" ) && ! values.isEmpty() )
    emit isXdebugCookie ( values );

  // Name
  QTreeWidgetItem* root = new QTreeWidgetItem ( parent );
  root->setExpanded ( true );
  root->setData ( 0, Qt::UserRole, cookie.name() );
  root->setText ( 0, cookie.name() );
  root->setText ( 1, trUtf8 ( "Cookie" ) );
  root->setForeground ( 1, Qt::lightGray );
  parent->addChild ( root );

  // Domain
  QTreeWidgetItem* item0 = new QTreeWidgetItem ( root );
  item0->setText ( 0, trUtf8 ( "Domain" ) );
  item0->setForeground ( 0, Qt::blue );
  item0->setText ( 1, cookie.domain() );
  root->addChild ( item0 );

  // Path
  QTreeWidgetItem* item1 = new QTreeWidgetItem ( root );
  item1->setText ( 0, trUtf8 ( "Path" ) );
  item1->setForeground ( 0, Qt::blue );
  item1->setText ( 1, cookie.path() );
  root->addChild ( item1 );

  // Values
  QTreeWidgetItem* item2 = new QTreeWidgetItem ( root );
  item2->setText ( 0, trUtf8 ( "Value" ) );
  item2->setForeground ( 0, Qt::blue );
  item2->setText ( 1, values );
  root->addChild ( item2 );

  // isSessionCookie
  QTreeWidgetItem* item3 = new QTreeWidgetItem ( root );
  item3->setText ( 0, trUtf8 ( "Session Cookie" ) );
  item3->setForeground ( 0, Qt::blue );
  item3->setText ( 1, ( ( cookie.isSessionCookie() ) ? yes : no ) );
  root->addChild ( item3 );

  // isHttpOnly
  QTreeWidgetItem* item4 = new QTreeWidgetItem ( root );
  item4->setText ( 0, trUtf8 ( "Only for HTTP?" ) );
  item4->setForeground ( 0, Qt::blue );
  item4->setText ( 1, ( ( cookie.isHttpOnly() ) ? yes : no ) );
  root->addChild ( item4 );

  // expirationDate
  QTreeWidgetItem* item5 = new QTreeWidgetItem ( root );
  item5->setText ( 0, trUtf8 ( "Expiration" ) );
  item5->setForeground ( 0, Qt::blue );
  item5->setText ( 1,  cookie.expirationDate().toString ( Qt::DefaultLocaleLongDate ) );
  root->addChild ( item5 );

  // isSecure
  QTreeWidgetItem* item6 = new QTreeWidgetItem ( root );
  item6->setText ( 0, trUtf8 ( "Secure" ) );
  item6->setForeground ( 0, Qt::blue );
  item6->setText ( 1, ( ( cookie.isSecure() ) ? yes : no ) );
  root->addChild ( item6 );
}

/**
* Suche anhand der URL nach einem Cookie von
* @class NetworkCookie
*/
void HeaderDock::setCookieData ( const QUrl &url )
{
  int widgetIndex = 2;
  clearContent ( widgetIndex );

  if ( ! m_networkCookie )
    return;

  QList<QNetworkCookie> cookies = m_networkCookie->cookiesForUrl ( url );
  if ( cookies.size() >= 1 )
  {
    QString name = url.host().remove ( QRegExp ( "\\bwww\\." ) );
    QTreeWidgetItem* item = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
    item->setData ( 0, Qt::UserRole, name );
    item->setText ( 0, name );
    item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
    item->setToolTip ( 0, url.host() );
    // Read all Cookies
    foreach ( QNetworkCookie keks, cookies )
    {
      setCookieData ( keks, item );
    }
    resizeSections ( widgetIndex );
  }
}

HeaderDock::~HeaderDock()
{}
