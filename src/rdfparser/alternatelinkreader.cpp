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

#include "alternatelinkreader.h"
#include "rssmodel.h"
#include "rssitem.h"
#include "rssparserdialog.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QPixmap>
#include <QtGui/QVBoxLayout>

/* QtWebKit */
#include <QtWebKit/QWebElementCollection>

AlternateLinkReader::AlternateLinkReader ( QWidget * parent )
    : QWidget ( parent )
    , comboTitle ( trUtf8 ( "RSS Feeds" ) )
{
  setObjectName ( QLatin1String ( "alternatelinkreader" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumWidth ( 160 );
  setMaximumWidth ( 300 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setObjectName ( QLatin1String ( "alternatelinkreaderlayout" ) );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  // ComboBox erstellen
  m_comboBox = new QComboBox ( this );
  m_comboBox->setObjectName ( QLatin1String ( "alternatelinkreadercombobox" ) );
  m_comboBox->setMinimumWidth ( 160 );
  m_comboBox->setMaximumWidth ( 300 );
  m_comboBox->setMinimumContentsLength ( 1 ); // sizeHint 0|1
  m_comboBox->setSizeAdjustPolicy ( QComboBox::AdjustToMinimumContentsLengthWithIcon );
  m_comboBox->setEnabled ( false );

  // Model Initialisieren
  m_model = new RSSModel ( m_comboBox );
  // setze den Standard Eintrag für die Anzeige
  m_model->setItem ( 0, new RSSItem ( comboTitle, QUrl (), comboTitle ) );
  m_comboBox->setModel ( m_model );

  layout->addWidget ( m_comboBox );

  setLayout ( layout );

  connect ( m_comboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( currentIndexChanged ( int ) ) );
}

/**
* Dialog für die Syntaxprüfung öffnen.
*/
void AlternateLinkReader::openParserDialog ( const QUrl &url )
{
  if ( ! url.isValid() )
    return;

  RSSParserDialog* dialog = new RSSParserDialog ( url, this );
  if ( dialog->exec() == QDialog::Accepted )
  {
    qDebug() << Q_FUNC_INFO << "TODO";
  }
  m_comboBox->setCurrentIndex ( 0 );
  delete dialog;
}

/**
* Wenn ein Eintrag ausgewählt wird der eine Valide URL besitzt
* dann das Signal @ref itemClicked auslösen.
*/
void AlternateLinkReader::currentIndexChanged ( int index )
{
  if ( index > 0 )
  {
    QStandardItem* item = m_model->item ( index, 0 );
    QUrl url = item->data ( Qt::UserRole ).toUrl();
    openParserDialog ( url );
  }
}

/**
* Im HEAD nach LINK mit Prädikat link[rel^=alternate] suchen und
* nachsehen ob es sich um RSS handelt.
*/
void AlternateLinkReader::setDomWebElement ( const QWebElement &dom )
{
  // ComboBox Datenkörper leeren und Aktivieren?
  m_comboBox->clear();
  m_model->setItem ( 0, new RSSItem ( comboTitle, QUrl (), comboTitle ) );

  QWebElementCollection nodeList = dom.findAll ( QString::fromUtf8 ( "link[rel^=alternate]" ) );
  m_comboBox->setEnabled ( ( ( nodeList.count() < 1 ) ? false : true ) );

  // Alle application-rss+xml durchlaufen
  foreach ( QWebElement element, nodeList )
  {
    QString type = element.attribute ( QLatin1String ( "type" ), QString::null );
    if ( type.isEmpty() || ! type.contains ( "rss+xml", Qt::CaseInsensitive ) )
      continue;

    QString source = element.attribute ( QLatin1String ( "href" ), QString::null );
    if ( source.isEmpty() )
      continue;

    QString title = element.attribute ( QLatin1String ( "title" ), comboTitle );
    m_model->appendRow ( new RSSItem ( title, QUrl ( source ), title ) );
  }
  m_comboBox->update();
}

AlternateLinkReader::~AlternateLinkReader()
{
  m_comboBox->clear();
}
