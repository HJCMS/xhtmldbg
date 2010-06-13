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
#include "rsstreeview.h"
#include "rssviewer.h"
#include "xhtmldbgmain.h"
#include "networkaccessmanager.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QTextCodec>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

/* Soprano */
#include <Soprano/Soprano>
// #include <Soprano/Backend>
// #include <Soprano/Model>
// #include <Soprano/Parser>
// #include <Soprano/PluginManager>
// #include <Soprano/StatementIterator>

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

  QSplitter* splitter = new QSplitter ( Qt::Vertical, this );
  splitter->setObjectName ( QLatin1String ( "rssparsersplitter" ) );
  vLayout->addWidget ( splitter );

  m_treeViewer = new RSSTreeView ( splitter );
  splitter->insertWidget ( 0, m_treeViewer );

  m_errorsList = new QListWidget ( splitter );
  m_errorsList->setObjectName ( QLatin1String ( "rssparsererrors" ) );
  splitter->insertWidget ( 1, m_errorsList );
  splitter->setCollapsible ( 1, true );

  m_sourceViewer = new RSSViewer ( splitter );
  splitter->insertWidget ( 2, m_sourceViewer );
  splitter->setCollapsible ( 2, true );

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
* Verwende hierbei die Standard RDF/XML Serialisierung
*/
bool RSSParserDialog::parse ( const QByteArray &data, const QUrl &url )
{
  Soprano::Model* model = Soprano::createModel();

  const Soprano::Parser* p = Soprano::PluginManager::instance()->discoverParserForSerialization (
                                 Soprano::SerializationRdfXml );

  QStringList messages;
  QTextStream stream ( data );
  Soprano::StatementIterator it = p->parseStream ( stream, url, Soprano::SerializationRdfXml );
  foreach ( Soprano::Statement s, it.allStatements() )
  {
    if ( model->addStatement ( s ) != Soprano::Error::ErrorNone )
      qDebug() << "###" << model->lastError().message();
  }

  if ( messages.size() > 0 )
    m_errorsList->addItems ( messages );

  return true;
}

/**
* Dokumenten Struktur in das TextEdit einfügen!
*/
void RSSParserDialog::setDocumentSource ( const QByteArray &data, const QUrl &url )
{
  QString errorMsg;
  int errorLine;
  int errorColumn;
  int indent = 1;

  QTextCodec* codec = QTextCodec::codecForHtml ( data, QTextCodec::codecForName ( "UTF-8" ) );

  QDomDocument dom;
  if ( dom.setContent ( codec->toUnicode ( data ), false, &errorMsg, &errorLine, &errorColumn ) )
  {
    m_treeViewer->createTreeView ( dom );
    m_sourceViewer->setText ( dom.toString ( indent ) );
    parse ( data, url );
  }
  else
  {
    qDebug() << Q_FUNC_INFO << errorMsg << errorLine << errorColumn;
  }
}

/**
* Wenn die Netzwerkanfrage abgeschlossen ist lese mit readAll die
* Daten in ein QByteArray und rufe @ref setDocumentSource auf.
*/
void RSSParserDialog::requestFinished()
{
  QUrl url = reply->url();
  QByteArray data = reply->readAll();

  if ( data.isEmpty() || !url.isValid() )
    return;

  m_errorsList->clear();
  setDocumentSource ( data, url );
}

RSSParserDialog::~RSSParserDialog()
{}
