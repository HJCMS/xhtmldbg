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

#include "webwallet.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QCryptographicHash>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KWebWallet>
#include <KDE/KWebPage>
#include <KDE/KWallet/Wallet>

WebWallet::WebWallet ( QObject * parent )
    : KWebWallet ( parent )
{
  setObjectName ( QLatin1String ( "WebWallet" ) );
  connect ( this, SIGNAL ( fillFormRequestCompleted ( bool ) ),
            this, SLOT ( operationComplete ( bool ) ) );

//     connect ( this, SIGNAL ( saveFormDataRequested ( const QString &, const QUrl & ) ),
//               this, SLOT ( savePostFormDataRequest ( const QString &, const QUrl & ) ) );

}

/** Erstelle einen MD5 Schluessel */
const QString WebWallet::createWalletKey ( const QString &data ) const
{
  QCryptographicHash crypto ( QCryptographicHash::Md5 );
  crypto.addData ( data.toAscii() );
  return crypto.result().toHex();
}

/** Anfrage */
void WebWallet::savePostFormDataRequest ( const QString &key, const QUrl &url )
{
  qDebug() << Q_FUNC_INFO << key << url;
}

/** Abgeschlossen */
void WebWallet::operationComplete ( bool b )
{
  qDebug() << Q_FUNC_INFO << b;
}

void WebWallet::triggerRequest ( QWebFrame * frame, const QUrl &url )
{
#ifdef DEBUG_VERBOSE
  // Keine Fragmente in Anfragen zulassen!
  QString key = createWalletKey ( frame->url().toString ( QUrl::RemoveFragment ) );
  qDebug() << Q_FUNC_INFO << key << url;
#else
  Q_UNUSED ( url )
  Q_UNUSED ( frame )
#endif
}

WebWallet::~WebWallet() {}
