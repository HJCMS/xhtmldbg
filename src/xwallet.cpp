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

#ifndef XHTMLDBG_APPS_NAME
# include "version.h"
#endif
#include "xwallet.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtWebKit */
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>

XWallet::XWallet ( QWidget * parent )
    : QWidget ( parent )
    , m_wallet ( KWallet::Wallet::openWallet ( KWallet::Wallet::NetworkWallet(), parent->winId(), KWallet::Wallet::Path ) )
{
  setObjectName ( QLatin1String ( "XWallet" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QString wn ( i18n ( "Wallet" ) );
  setWindowTitle ( wn );
  setToolTip ( wn );
  setStatusTip ( wn );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "xwallet.layout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  m_button = new QToolButton ( this );
  m_button->setObjectName ( QLatin1String ( "xwallet.layout.button" ) );
  m_button->setIcon ( KIcon ( "wallet-closed" ) );
  m_button->setAutoRaise ( true );
  layout->addWidget ( m_button );

  setLayout ( layout );

  // SIGNALS
  connect ( m_wallet, SIGNAL ( walletClosed() ),
            this, SLOT ( displayWalletIconStatus() ) );

  connect ( m_wallet, SIGNAL ( walletOpened ( bool ) ),
            this, SLOT ( displayWalletIconStatus ( bool ) ) );

  initWalletFolder();
}

/**
* Sehe nach ob die Wallet Ressource existiert und das "xhtmldbg" Verzeichnis Vorhanden ist...
* \li Wenn nicht - dann anlegen
* \li Status Icon ändern
*/
void XWallet::initWalletFolder()
{
  /* Mit Dbus testen: qdbus org.kde.kwalletd "/modules/kwalletd" isEnabled */
  if ( m_wallet && m_wallet->isOpen() )
    {
      if ( m_wallet->folderDoesNotExist ( m_wallet->NetworkWallet(), QLatin1String ( "xhtmldbg" ) ) )
        m_wallet->createFolder ( QLatin1String ( "xhtmldbg" ) );

      displayWalletIconStatus ( m_wallet->isOpen() );
    }
}

/**
* Aktuellen Wallet Status mit Icon darstellen
*/
void XWallet::displayWalletIconStatus ( bool b )
{
  if ( b )
    m_button->setIcon ( KIcon ( "wallet-open" ) );
  else
    m_button->setIcon ( KIcon ( "wallet-closed" ) );
}

XWallet::~XWallet()
{
  m_wallet->disconnectApplication ( m_wallet->NetworkWallet(), XHTMLDBG_APPS_NAME );
}
