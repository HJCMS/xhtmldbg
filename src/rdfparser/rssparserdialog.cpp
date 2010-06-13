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

#include "rssparserdialog.h"
#include "xhtmldbgmain.h"
#include "networkaccessmanager.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

/* Soprano */
#include <Soprano/Model>
#include <Soprano/Parser>
#include <Soprano/PluginManager>
#include <Soprano/StatementIterator>

RSSParserDialog::RSSParserDialog ( const QUrl &url, QWidget * parent )
    : QDialog ( parent )
    , rssUrl ( url )
{
  setObjectName ( QLatin1String ( "rssparserdialog" ) );
  setMinimumWidth ( 350 );
  setMinimumHeight ( 250 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( QLatin1String ( "rssparserdialoglayout" ) );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "rssparserdialogbuttonbox" ) );
  box->setStandardButtons ( ( QDialogButtonBox::Close | QDialogButtonBox::Ok ) );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  QNetworkRequest request ( rssUrl );
  reply = xhtmldbgmain::instance()->networkAccessManager()->get ( request );

  connect ( reply, SIGNAL ( finished() ), this, SLOT ( requestFinished() ) );
  connect ( box, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected() ), this, SLOT ( reject() ) );
}

/**
* Starte mit Soprano die RDF Syntax Überprüfung
*/
bool RSSParserDialog::parse ( const QByteArray &data, const QUrl &url )
{
  const Soprano::Parser* p = Soprano::PluginManager::instance()->discoverParserForSerialization (
                                 Soprano::SerializationRdfXml );

  Q_CHECK_PTR ( p );
  QTextStream stream ( data );
  Soprano::StatementIterator it = p->parseStream ( stream, url, Soprano::SerializationRdfXml );
  while ( it.next() )
  {
    qDebug() << Q_FUNC_INFO << *it;
  }
  return true;
}

/**
* Wenn die Netzwerkanfrage abgeschlossen ist lese mit readAll die
* Daten in ein QByteArray und rufe @ref parse auf.
*/
void RSSParserDialog::requestFinished()
{
  QUrl url = reply->url();
  QByteArray data = reply->readAll();

  if ( data.isEmpty() || !url.isValid() )
    return;

  qDebug() << Q_FUNC_INFO << parse ( data, url );
}

RSSParserDialog::~RSSParserDialog()
{}
