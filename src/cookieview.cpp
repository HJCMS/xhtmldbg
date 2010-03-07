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

#include "cookieview.h"
#include "networkcookie.h"
#include "xhtmldbg.h"

/* QtCore */
#include <QtCore>
// #include <QtCore/QString>

/* QtGui */
#include <QtGui>

/* QtNetwork */
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

CookieView::CookieView ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( "cookieview" );
  setWindowTitle ( trUtf8 ( "Cookies" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setWindowIcon ( QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );

  QStringList labels;
  labels << trUtf8 ( "Name" ) << trUtf8 ( "Value" );

  m_treeWidget = new QTreeWidget ( this );
  m_treeWidget->setAutoScroll ( true );
  m_treeWidget->setWordWrap ( true );
  m_treeWidget->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Preferred );
  m_treeWidget->setHeaderLabels ( labels );
  m_treeWidget->setSortingEnabled ( false );
  m_treeWidget->header()->setResizeMode ( QHeaderView::ResizeToContents );
  m_treeWidget->setFrameStyle ( QFrame::Box );

  // get CookieJar
  m_networkCookie = xhtmldbg::instance()->cookieManager();

  setWidget ( m_treeWidget );
}

/**
* TEST unserialize
* @code
    a:3:{s:3:"BID";i:1;s:7:"BGRUPPE";i:4;s:4:"SKEY";s:32:"8b3c3634669c41f26c1a511cb03d6bc0";}
* @endcode
*/
QString CookieView::unserialize ( const QByteArray &data ) const
{
  if ( data.contains ( "%" ) )
  {
    QByteArray serial = QByteArray::fromPercentEncoding ( data );
    QString retval ( serial );
    retval.remove ( QRegExp ( "(^.{1,6}\\{)|(\\}$)" ) );
    retval.remove ( QRegExp ( ";$" ) );

    QString output;
    QStringList buffer;
    foreach ( QString p, retval.split ( QRegExp ( ";?[ais]:" ) ) )
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

void CookieView::setCookieData ( const QNetworkCookie &cookie, QTreeWidgetItem* parent )
{
  QString yes ( trUtf8 ( "Yes" ) );
  QString no ( trUtf8 ( "No" ) );
  QFontMetrics fontMetric = m_treeWidget->fontMetrics();
  QString values = unserialize ( cookie.value() );
  int minWidth = ( fontMetric.width ( values ) + parent->font ( 0 ).weight() );

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
  if ( minWidth >= item2->sizeHint ( 1 ).width() )
    m_treeWidget->setColumnWidth ( 1, minWidth );

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

void CookieView::cookiesFromUrl ( const QUrl &url )
{
  m_treeWidget->clear();
  if ( ! m_networkCookie )
    return;

  QList<QNetworkCookie> cookies = m_networkCookie->cookiesForUrl ( url );
  if ( cookies.size() >= 1 )
  {
    QString name = url.host().remove ( QRegExp ( "\\bwww\\." ) );
    QTreeWidgetItem* item = new QTreeWidgetItem ( m_treeWidget->invisibleRootItem() );
    item->setExpanded ( true );
    item->setData ( 0, Qt::UserRole, name );
    item->setText ( 0, name );
    item->setIcon ( 0, QIcon::fromTheme ( QLatin1String ( "preferences-web-browser-cookies" ) ) );
    item->setToolTip ( 0, url.host() );
    // Read all Cookies
    foreach ( QNetworkCookie keks, cookies )
    {
      setCookieData ( keks, item );
    }
  }
}

CookieView::~CookieView()
{}
