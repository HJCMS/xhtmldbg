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

#include "headerdock.h"
#include "docktreewidget.h"
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

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>

HeaderDock::HeaderDock ( QWidget * parent )
    : Docking ( parent )
{
  setObjectName ( "headerviews" );
  setWindowTitle ( i18n ( "Headers" ) );

  QStringList labels;
  labels << i18n ( "Header" ) << i18n ( "Value" );

  // Header Data
  setColumnCount ( 2, 0 );
  setTreeHeaderLabels ( labels, 0 );
  labels.clear();

  // Post Data
  m_treePostVars = new DockTreeWidget ( this );
  m_treePostVars->setObjectName ( QLatin1String ( "postvarstreewidget" ) );
  m_treePostVars->setToolTip ( i18n ( "POST Variables" ) );
  addTreeWidget ( m_treePostVars );

  labels << i18n ( "Parameter" ) << i18n ( "Value" );
  setColumnCount ( 2, 1 );
  setTreeHeaderLabels ( labels, 1 );
  labels.clear();

  // Cookies Data
  m_treeCookies = new DockTreeWidget ( this );
  m_treeCookies->setObjectName ( QLatin1String ( "cookiestreewidget" ) );
  m_treeCookies->setToolTip ( i18n ( "Cookies" ) );
  addTreeWidget ( m_treeCookies );
  labels << i18n ( "Name" ) << i18n ( "Value" );
  setColumnCount ( 2, 2 );
  setTreeHeaderLabels ( labels, 2 );
  labels.clear();
}

/**
* Doppelte Einträge bei Pfad oder Query entfernen und unten neu Schreiben
*/
void HeaderDock::unscramble ( const QString &txt, DockTreeWidget *widget, QTreeWidgetItem *item ) const
{
  QTreeWidgetItemIterator it ( widget, QTreeWidgetItemIterator::HasChildren );
  while ( *it )
  {
    ( *it )->setExpanded ( false ); // Einträge einklappen
    if ( txt == ( *it )->data ( 0, Qt::DisplayRole ) )
    {
      item->removeChild ( ( *it ) );
      widget->sortItems ( 0, Qt::AscendingOrder );
      delete ( *it );
      break;
    }
    ++it;
  }
}

/**
* Setze die Beschriftung für den entsprechenden Baum
*/
void HeaderDock::setTreeHeaderLabels ( const QStringList &labels, int index )
{
  Docking::setTreeHeaderLabels ( labels, index );
}

/**
* Alle vorhandenen Datenbäume leeren.
*/
void HeaderDock::clearAll ()
{
  for ( int i = 0; i < count(); i++ )
    clearContent ( i );
}

/**
* Header Datenbaum mit allen Seiten Url's erstellen
* Es werden alle Datenköpfe der Seite aufgelöst die
* der Netzwerkmanager mit request verarbeitet.
*/
void HeaderDock::setHeaderData ( const QUrl &replyUrl, const QMap<QString,QString> &map )
{
  // qDebug() << Q_FUNC_INFO << replyUrl;
  int widgetIndex = 0;
  bool isHtmlContent = false;
  QString host = ( replyUrl.scheme().contains ( "http" ) ? replyUrl.host() : i18n ( "System File" ) );
  DockTreeWidget* tree = widget ( widgetIndex );
  QTreeWidgetItem* parent;

  if ( map.contains ( "Content-Type" ) )
    isHtmlContent = QString ( map["Content-Type"] ).contains ( "text/html" );

  // Nur wenn ein Hostname vorhanden, dann eine Cookie abfrage starten!
  if ( ! replyUrl.host().isEmpty() )
  {
    QUrl cookieUrl;
    cookieUrl.setScheme ( replyUrl.scheme() );
    cookieUrl.setHost ( replyUrl.host() );
    setCookieData ( cookieUrl );
  }

  // Oberster Eintrag mit Hostnamen
  if ( itemExists ( host, widgetIndex ) )
  {
    parent = tree->topLevelItem ( 0 );
    unscramble ( replyUrl.path(), tree, parent );
  }
  else
  {
    parent = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
    parent->setExpanded ( false );
    parent->setData ( 0, Qt::DisplayRole, host );
    parent->setText ( 1, i18n ( "Hostname" ) );
    parent->setForeground ( 1, Qt::lightGray );
  }

  QTreeWidgetItem* queryItem = addTopLevelItem ( parent, widgetIndex );
  queryItem->setData ( 0, Qt::DisplayRole, replyUrl.path() );
  queryItem->setExpanded ( false );
  if ( ! isHtmlContent )
    queryItem->setForeground ( 0, Qt::lightGray );
  queryItem->setText ( 1, i18n ( "Item" ) );
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
  }

  // Wenn nur ein TopLevel Eintrag, dann alles aufklappen!
  if ( tree->topLevelItemCount() == 1 )
    tree->expandAll ();


}

/**
* Die vom Netzwerkmanager abgegriffenen POST Variablen
* an dieser Stelle einfügen.
*/
void HeaderDock::setPostedData ( const QUrl &url, const QStringList &list )
{
  int widgetIndex = 1;

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
    item->setData ( 0, Qt::DisplayRole, i18n ( "Anchor" ) );
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
  }
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
* und füge diese in ein.
*/
void HeaderDock::setCookieData ( const QNetworkCookie &cookie, QTreeWidgetItem* parent )
{
  QString yes ( i18n ( "Yes" ) );
  QString no ( i18n ( "No" ) );
  QString values = unserialize ( cookie.value() );

  // Name
  QTreeWidgetItem* root = new QTreeWidgetItem ( parent );
  root->setExpanded ( false );
  root->setData ( 0, Qt::UserRole, cookie.name() );
  root->setText ( 0, cookie.name() );
  root->setText ( 1, i18n ( "Cookie" ) );
  root->setForeground ( 1, Qt::lightGray );
  parent->addChild ( root );

  // Domain
  QTreeWidgetItem* item0 = new QTreeWidgetItem ( root );
  item0->setText ( 0, i18n ( "Domain" ) );
  item0->setForeground ( 0, Qt::blue );
  item0->setText ( 1, cookie.domain() );
  root->addChild ( item0 );

  // Path
  QTreeWidgetItem* item1 = new QTreeWidgetItem ( root );
  item1->setText ( 0, i18n ( "Path" ) );
  item1->setForeground ( 0, Qt::blue );
  item1->setText ( 1, cookie.path() );
  root->addChild ( item1 );

  // Values
  QTreeWidgetItem* item2 = new QTreeWidgetItem ( root );
  item2->setText ( 0, i18n ( "Value" ) );
  item2->setForeground ( 0, Qt::blue );
  item2->setText ( 1, values );
  root->addChild ( item2 );

  // isSessionCookie
  QTreeWidgetItem* item3 = new QTreeWidgetItem ( root );
  item3->setText ( 0, i18n ( "Session Cookie" ) );
  item3->setForeground ( 0, Qt::blue );
  item3->setText ( 1, ( ( cookie.isSessionCookie() ) ? yes : no ) );
  root->addChild ( item3 );

  // isHttpOnly
  QTreeWidgetItem* item4 = new QTreeWidgetItem ( root );
  item4->setText ( 0, i18n ( "Only for HTTP?" ) );
  item4->setForeground ( 0, Qt::blue );
  item4->setText ( 1, ( ( cookie.isHttpOnly() ) ? yes : no ) );
  root->addChild ( item4 );

  // expirationDate
  QTreeWidgetItem* item5 = new QTreeWidgetItem ( root );
  item5->setText ( 0, i18n ( "Expiration" ) );
  item5->setForeground ( 0, Qt::blue );
  item5->setText ( 1,  cookie.expirationDate().toString ( Qt::DefaultLocaleLongDate ) );
  root->addChild ( item5 );

  // isSecure
  QTreeWidgetItem* item6 = new QTreeWidgetItem ( root );
  item6->setText ( 0, i18n ( "Secure" ) );
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

  // get CookieJar
  NetworkCookie* m_networkCookie = xhtmldbgmain::instance()->cookieManager();
  if ( ! m_networkCookie )
    return;

  QString host = url.host();
  DockTreeWidget* tree = widget ( widgetIndex );
  // Oberster Eintrag mit Hostnamen
  if ( itemExists ( host, widgetIndex ) )
  {
    QTreeWidgetItem* parent = tree->topLevelItem ( 0 );
    unscramble ( host, tree, parent );
  }

  QList<QNetworkCookie> cookies = m_networkCookie->cookiesForUrl ( url );
  if ( cookies.size() >= 1 )
  {
    QTreeWidgetItem* item = addTopLevelItem ( rootItem ( widgetIndex ), widgetIndex );
    item->setData ( 0, Qt::UserRole, host );
    item->setText ( 0, host );
    item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
    item->setToolTip ( 0, url.toString () );
    item->setText ( 1, i18n ( "Cookie Hostname" ) );
    item->setForeground ( 1, Qt::lightGray );
    // Read all Cookies
    foreach ( QNetworkCookie keks, cookies )
    {
      setCookieData ( keks, item );
    }
  }

  // Wenn nur ein TopLevel Eintrag, dann alles aufklappen!
  if ( tree->topLevelItemCount() == 1 )
    tree->expandAll ();
}

HeaderDock::~HeaderDock()
{}
