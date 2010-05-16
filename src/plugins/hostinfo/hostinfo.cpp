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

#include "hostinfo.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QHostAddress>

HostInfo::HostInfo ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "HostInfo" ) );
  proc = new QProcess ( this );
  proc->setReadChannel ( QProcess::StandardOutput );

  dialog = new QDialog;
  dialog->setMinimumWidth ( 480 );
  dialog->setObjectName ( QLatin1String ( "HostInfoDialog" ) );
  dialog->setSizeGripEnabled ( true );
  dialog->setWindowTitle ( trUtf8 ( "Host Information" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( dialog );
  vLayout->addWidget ( new QLabel ( trUtf8 ( "Resolved Host Information with given Url" ) ) );

  infoLabel = new QLabel ( dialog );
  infoLabel->setObjectName ( QLatin1String ( "ip_txt" ) );
  vLayout->addWidget ( infoLabel );

  digText = new QListWidget ( dialog );
  digText->setObjectName ( QLatin1String ( "bind_dig_txt" ) );
  digText->setAlternatingRowColors ( false );
  digText->setSortingEnabled ( false );
  vLayout->addWidget ( digText );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, dialog );
  box->setStandardButtons ( QDialogButtonBox::Ok );
  box->setCenterButtons ( true );
  vLayout->addWidget ( box );

  connect ( proc, SIGNAL ( readyReadStandardOutput() ),
            this, SLOT ( readHostnameInfo() ) );

  connect ( box, SIGNAL ( accepted() ), dialog, SLOT ( accept() ) );
}

void HostInfo::readHostnameInfo()
{
  digText->clear();
  QPalette pal = digText->palette();
  QString data = proc->readAll();
  foreach ( QString line, data.split ( QRegExp ( "[\r\n]" ) ) )
  {
    if ( ! line.isEmpty() )
    {
      QListWidgetItem* item = new QListWidgetItem ( line, digText );
      if ( line.contains ( QRegExp ( "^;;" ) ) )
      {
        item->setForeground ( pal.brush ( QPalette::Normal, QPalette::Midlight ) );
        item->setBackground ( pal.brush ( QPalette::Normal, QPalette::Dark ) );
      }
      else if ( line.contains ( QRegExp ( "^;" ) ) )
      {
        item->setForeground ( pal.brush ( QPalette::Normal, QPalette::Midlight ) );
        item->setBackground ( pal.brush ( QPalette::Normal, QPalette::Mid ) );
      }
    }
  }
  proc->close();
}

void HostInfo::prepareHostInfo ( const QHostInfo &host )
{
  QStringList info;
  // Zuserst eine Bind Anfrage senden
  QStringList args;
  args << "-x" << "-b" << host.hostName();
  proc->start ( "dig", args );

  QString content = QString ( "<b>Host:</b> %1<br/>" ).arg ( host.hostName() );
  foreach ( QHostAddress address, host.addresses() )
  {
    if ( address.isNull() )
      continue; // Kein INADDR_ANY oder in6addr_any anzeigen.

    if ( address.toIPv4Address() )
    {
      content.append ( QString ( "<b>IPv4:</b> %1<br/>" ).arg ( address.toString() ) );
    }
    else
    {
      content.append ( QString ( "<b>IPv6:</b> %1<br/>" ).arg ( address.toString() ) );
    }
  }
  infoLabel->setText ( content );
  dialog->exec();
}

void HostInfo::setDomain ( const QUrl &url )
{
  QHostInfo::lookupHost ( url.host(), this, SLOT ( prepareHostInfo ( const QHostInfo & ) ) );
  // prepareHostInfo ( QHostInfo::fromName ( url.host() ) );
}

HostInfo::~HostInfo()
{}
