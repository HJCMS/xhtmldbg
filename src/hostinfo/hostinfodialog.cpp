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

#include "hostinfo.h"
#include "hostinfodialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QListWidgetItem>
#include <QtGui/QPalette>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KLocale>

HostInfoDialog::HostInfoDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "HostInfoDialog" ) );
  setWindowTitle ( i18n ( "Host Information" ) );
  setMinimumWidth ( 480 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->addWidget ( new QLabel ( i18n ( "Resolved Host Information with given Url" ) ) );

  m_label = new QLabel ( this );
  m_label->setObjectName ( QLatin1String ( "ip_txt" ) );
  vLayout->addWidget ( m_label );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setObjectName ( QLatin1String ( "bind_dig_txt" ) );
  m_listWidget->setAlternatingRowColors ( false );
  m_listWidget->setSortingEnabled ( false );
  vLayout->addWidget ( m_listWidget );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setStandardButtons ( QDialogButtonBox::Ok );
  box->setCenterButtons ( true );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  m_hostInfo = new HostInfo ( this );

  connect ( m_hostInfo, SIGNAL ( labelData ( const QString & ) ),
            this, SLOT ( setHeaderLabel ( const QString & ) ) );

  connect ( m_hostInfo, SIGNAL ( listData ( const QStringList & ) ),
            this, SLOT ( setListWidgetItems ( const QStringList & ) ) );

  connect ( m_hostInfo, SIGNAL ( closedNormal () ),
            this, SLOT ( hostDataChanged () ) );

  connect ( box, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( accepted() ), this, SLOT ( deleteLater() ) );
}

/**
* IP Adressen in das obere QLabel schreiben.
*/
void HostInfoDialog::setHeaderLabel ( const QString &text )
{
  m_label->setText ( text );
}

/**
* Ausgabe von /usr/bin/dig in das QListWidget schreiben!
*/
void HostInfoDialog::setListWidgetItems ( const QStringList &data )
{
  m_listWidget->clear();
  QPalette pal = m_listWidget->palette();
  foreach ( QString line, data )
  {
    if ( ! line.isEmpty() )
    {
      QString text ( line );
      QListWidgetItem* item = new QListWidgetItem ( text.remove( QRegExp( "^[; ]+" ) ), m_listWidget );
      if ( line.contains ( QRegExp ( "^;;" ) ) )
      {
        item->setForeground ( pal.brush ( QPalette::Normal, QPalette::Midlight ) );
        item->setBackground ( pal.brush ( QPalette::Normal, QPalette::Dark ) );
      }
    }
  }
}

void HostInfoDialog::hostDataChanged()
{
  exec();
}

void HostInfoDialog::getHostInfo ( const QString &host )
{
  QHostInfo::lookupHost ( host, m_hostInfo, SLOT ( dig ( const QHostInfo & ) ) );
}

HostInfoDialog::~HostInfoDialog()
{}
