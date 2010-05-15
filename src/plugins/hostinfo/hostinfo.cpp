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
#include <QtCore/QSize>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QHostAddress>

HostInfo::HostInfo ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "HostInfo" ) );
}

void HostInfo::createPopUp ( const QString &data, const QString &details )
{
  QIcon icon = QIcon::fromTheme ( QString::fromUtf8 ( "xhtmldbg" ) );
  QPixmap pixmap = icon.pixmap ( QSize ( 48, 48 ), QIcon::Normal, QIcon::On );

  QString Style ( "QMessageBox { border: 2px ridge #353535; border-radius: 2px; }" );
  Style.append ( "QLabel#qt_msgboxex_icon_label { border: none; }" );

  QMessageBox box;
  box.setWindowFlags ( Qt::Popup );
  box.setTextFormat ( Qt::RichText );
  box.setStyleSheet ( Style );
  box.setIconPixmap ( pixmap );
  box.setWindowTitle ( trUtf8 ( "Host Information" ) );
  box.setInformativeText ( trUtf8 ( "Resolved Host Information with given Url" ) );
  box.setText ( data );
  if ( ! details.isEmpty() )
    box.setDetailedText ( details );

  box.setStandardButtons ( QMessageBox::Ok );
  box.setDefaultButton ( QMessageBox::Ok );
  box.exec();
}

void HostInfo::prepareHostInfo ( const QHostInfo &host )
{
  QStringList info;
  QString content = QString ( "<b>Host:</b> %1<br/>" ).arg ( host.hostName() );
  foreach ( QHostAddress address, host.addresses() )
  {
    if ( address.isNull() )
      continue; // Kein INADDR_ANY oder in6addr_any anzeigen.

    if ( address.toIPv4Address() )
      content.append ( QString ( "<b>IPv4:</b> %1<br/>" ).arg ( address.toString() ) );
    else
      content.append ( QString ( "<b>IPv6:</b> %1<br/>" ).arg ( address.toString() ) );
  }
  createPopUp ( content );
}

void HostInfo::setDomain ( const QUrl &url )
{
  QHostInfo::lookupHost ( url.host(), this, SLOT ( prepareHostInfo ( const QHostInfo & ) ) );
  // prepareHostInfo ( QHostInfo::fromName ( url.host() ) );
}

HostInfo::~HostInfo()
{}
