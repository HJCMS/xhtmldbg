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

#include "configtrustedhosts.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QListWidgetItem>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

ConfigTrustedHosts::ConfigTrustedHosts ( QWidget * parent )
    : QGroupBox ( trUtf8 ( "Whitelist for Untrusted Certificates" ), parent )
{
  setObjectName ( QLatin1String ( "configtrustedhosts" ) );
  setFlat ( true );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "configtrustedhosts_layout" ) );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setObjectName ( QLatin1String ( "configtrustedhosts_list" ) );
  m_listWidget->setEditTriggers ( ( QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked ) );
  m_listWidget->setProperty ( "showDropIndicator", QVariant ( false ) );
  m_listWidget->setSortingEnabled ( true );
  verticalLayout->addWidget ( m_listWidget );

  QHBoxLayout* editLayout = new QHBoxLayout;
  editLayout->setObjectName ( QLatin1String ( "configtrustedhosts_edit_trust_host_layout" ) );

  editLayout->addItem ( new QSpacerItem ( 10, 5, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred ) );

  trustedEdit = new QLineEdit ( this );
  trustedEdit->setObjectName ( QLatin1String ( "configtrustedhosts_add_trust_host" ) );
  editLayout->addWidget ( trustedEdit );

  QToolButton* btn1 = new QToolButton ( this );
  btn1->setObjectName ( QLatin1String ( "configtrustedhosts_btn_remove" ) );
  btn1->setIcon ( QIcon::fromTheme ( "list-remove" ) );
  editLayout->addWidget ( btn1 );

  QToolButton* btn2 = new QToolButton ( this );
  btn2->setObjectName ( QLatin1String ( "configtrustedhosts_btn_add" ) );
  btn2->setIcon ( QIcon::fromTheme ( "list-add" ) );
  editLayout->addWidget ( btn2 );

  QToolButton* btn3 = new QToolButton ( this );
  btn3->setObjectName ( QLatin1String ( "configtrustedhosts_btn_clear" ) );
  btn3->setIcon ( QIcon::fromTheme ( "archive-remove" ) );
  editLayout->addWidget ( btn3 );

  verticalLayout->addLayout ( editLayout );

  setLayout ( verticalLayout );

  connect ( btn1, SIGNAL ( clicked() ), this, SLOT ( delTrustedHost() ) );
  connect ( btn2, SIGNAL ( clicked() ), this, SLOT ( addTrustedHost() ) );
  connect ( btn3, SIGNAL ( clicked() ), m_listWidget, SLOT ( clear() ) );
  connect ( btn3, SIGNAL ( clicked() ), this, SIGNAL ( modified() ) );
}

/**
* Eine neue Domäne in die Vertrauenswürdige
* Zertifikat's Liste aufnehmen.
*/
void ConfigTrustedHosts::addTrustedHost()
{
  if ( trustedEdit->text().isEmpty() )
    return;

  QUrl url;
  url.setScheme ( QLatin1String ( "https" ) );
  url.setHost ( trustedEdit->text() );
  if ( ! url.isValid() )
    return;

  m_listWidget->addItem ( url.host() );
  trustedEdit->clear();
  trustedEdit->setFocus();
  emit modified ( true );
}

/**
* Suche alle ausgewählten Domänen in Liste für
* Vertrauenswürdige Zertifikate und entferne diese.
*/
void ConfigTrustedHosts::delTrustedHost()
{
  foreach ( QListWidgetItem* item, m_listWidget->selectedItems() )
  {
    if ( item->isSelected() )
    {
      m_listWidget->removeItemWidget ( item );
      delete item;
      emit modified ( true );
      break;
    }
  }
}

/**
* Lade die Verdrauenswürdigen Zertifikate aus der Konfiguration
*/
void ConfigTrustedHosts::setTrustedList ( const QStringList &list )
{
  m_listWidget->clear();
  m_listWidget->addItems ( list );
}

/**
* Aktuelle Liste der Verdrauenswürdigen Zertifikate ausgeben.
*/
const QStringList ConfigTrustedHosts::getTrustedList ()
{
  QStringList list;
  for ( int i = 0; i < m_listWidget->count(); ++i )
  {
    list.append ( m_listWidget->item ( i )->text() );
  }
  return list;
}

ConfigTrustedHosts::~ConfigTrustedHosts()
{}
