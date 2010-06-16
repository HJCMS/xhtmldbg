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
#include "raptorparser.h"
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
#include <QtGui/QListWidgetItem>
#include <QtGui/QSizePolicy>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

RSSParserDialog::RSSParserDialog ( const QUrl &url, const QString &type, QWidget * parent )
    : QDialog ( parent )
    , rssUrl ( url )
    , mimeType ( type )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
{
  setObjectName ( QLatin1String ( "rssparserdialog" ) );
  setWindowTitle ( trUtf8 ( "RSS Parser" ) );
  setMinimumWidth ( 550 );
  setMinimumHeight ( 250 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setObjectName ( QLatin1String ( "rssparserdialoglayout" ) );

  QIcon boxIcon ( QString::fromUtf8 ( ":/icons/qtidy.png" ) );

  QToolBox* toolBox = new QToolBox ( this, Qt::Widget );
  toolBox->setObjectName ( QLatin1String ( "rssparsertoolbox" ) );
  vLayout->addWidget ( toolBox );

  m_parser = new RaptorParser ( this );

  m_treeViewer = new RSSTreeView ( toolBox );
  toolBox->addItem ( m_treeViewer, boxIcon, trUtf8 ( "Document Structure" ) );

  m_MessagesList = new QListWidget ( toolBox );
  m_MessagesList->setObjectName ( QLatin1String ( "rssparsererrors" ) );
  m_MessagesList->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Minimum );
  toolBox->addItem ( m_MessagesList, boxIcon, trUtf8 ( "Parser Messages" ) );

  m_sourceViewer = new RSSViewer ( toolBox );
  toolBox->addItem ( m_sourceViewer, boxIcon, trUtf8 ( "Source" ) );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setObjectName ( QLatin1String ( "rssparserdialogbuttonbox" ) );
  box->setStandardButtons ( ( QDialogButtonBox::Close | QDialogButtonBox::Ok ) );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  QNetworkRequest request ( rssUrl );
  reply = xhtmldbgmain::instance()->networkAccessManager()->get ( request );

  connect ( m_parser, SIGNAL ( errorMessage ( const QString & ) ),
            this, SLOT ( error ( const QString & ) ) );

  connect ( reply, SIGNAL ( finished() ), this, SLOT ( requestFinished() ) );
  connect ( box, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected() ), this, SLOT ( reject() ) );
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
    notice ( trUtf8 ( "Checking: %1" ).arg ( url.toString() ) );
    m_treeViewer->createTreeView ( dom );
    m_sourceViewer->setText ( dom.toString ( indent ) );
    QString nodeName = dom.documentElement().tagName();
    if ( ( nodeName.contains ( "rdf:", Qt::CaseInsensitive ) ) )
    {
      // Wenn es sich um ein rdf:RDF Element handelt dann mit "RDF" prüfen
      m_parser->parseDocument ( data, url, RaptorParser::RDF );
    }
    else if ( ( nodeName.contains ( "rss", Qt::CaseInsensitive ) ) )
      notice ( trUtf8 ( "RSS 2.0 Scheme validation currently bot Supported." ) );
    else if ( ( nodeName.contains ( "feed", Qt::CaseInsensitive ) ) )
    {
      // Wenn es sich um ein "feed" Element handelt dann mit "ATOM" prüfen
      m_parser->parseDocument ( data, url, RaptorParser::ATOM );
    }
  }
  else
  {
    error ( QString ( "Error: %1, on Line: %2 Column: %3" ).arg ( errorMsg,
            QString::number ( errorLine ), QString::number ( errorColumn ) ) );
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

  m_MessagesList->clear();
  setDocumentSource ( data, url );
}

/**
* Nachrichten Hinweise in die Liste schreiben!
*/
void RSSParserDialog::notice ( const QString &txt )
{
  QListWidgetItem* item = new QListWidgetItem ( iconNotice, txt, m_MessagesList );
  m_MessagesList->addItem ( item );
}

/**
* Nachrichten Fehler in die Liste schreiben!
*/
void RSSParserDialog::error ( const QString &txt )
{
  QListWidgetItem* item = new QListWidgetItem ( iconWarning, txt, m_MessagesList );
  m_MessagesList->addItem ( item );
}

RSSParserDialog::~RSSParserDialog()
{
  if ( m_parser )
    delete m_parser;
}
