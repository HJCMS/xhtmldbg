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

#include "configheaderdefinitions.h"

/* QtCore */
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QVBoxLayout>

ConfigHeaderDefinitions::ConfigHeaderDefinitions ( QWidget * parent )
    : QGroupBox ( trUtf8 ( "Header Field Definitions" ), parent )
{
  setObjectName ( QLatin1String ( "configheaderdefinitions" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setFlat ( true );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "configheaderdefinitions_layout" ) );
  verticalLayout->setContentsMargins ( 0, 0, 0, 0 );
  verticalLayout->setSpacing ( 5 );

  QLabel* lr0 = new QLabel ( this );
  lr0->setText ( trUtf8 ( "Warning: Do not edit this Table if you not now how it works! For more info see <a href=\"%1\">RFC 2616</a>" )
                 .arg ( "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14" ) );
  lr0->setAlignment ( ( Qt::AlignJustify | Qt::AlignTop ) );
  lr0->setWordWrap ( true );
  lr0->setIndent ( 2 );
  lr0->setOpenExternalLinks ( true );
  lr0->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  verticalLayout->addWidget ( lr0 );

  headersTable = new QTableWidget ( this );
  headersTable->setColumnCount ( 2 );
  headersTable->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( trUtf8 ( "Parameter" ) ) );
  headersTable->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( trUtf8 ( "Values" ) ) );
  headersTable->setProperty ( "showDropIndicator", QVariant ( false ) );
  headersTable->setDragDropOverwriteMode ( false );
  headersTable->setWordWrap ( false );
  headersTable->horizontalHeader()->setDefaultSectionSize ( 150 );
  headersTable->horizontalHeader()->setStretchLastSection ( true );
  headersTable->verticalHeader()->setVisible ( false );
  verticalLayout->addWidget ( headersTable );

  setLayout ( verticalLayout );
}

/**
* Lade die Datenkopf Definitionen aus der Konfiguration
*/
void ConfigHeaderDefinitions::loadHeaderDefinitions ( QSettings * cfg )
{
  cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
  QStringList keys = cfg->allKeys();
  if ( keys.size() >= 1 )
  {
    headersTable->setRowCount ( keys.size() );
    int r = 0;
    foreach ( QString key, keys )
    {
      QString val = cfg->value ( key ).toString();
      if ( val.isEmpty() )
        continue;

      headersTable->setItem ( r, 0, new QTableWidgetItem ( key ) );
      headersTable->setItem ( r, 1, new QTableWidgetItem ( val ) );
      r++;
    }
  }
  cfg->endGroup();
}

/**
* Die vom Benutzer gesetzten Datenköpfe in die Konfiguration schreiben.
* Mit exclude werden die Header Definitionen ausgeschlossen die,
* wie z.B: User-Agent nicht hier hinein gehören.
*/
void ConfigHeaderDefinitions::saveHeaderDefinitions ( QSettings * cfg )
{
  QStringList exclude ( "user-agent" );
  int rows = headersTable->rowCount();
  cfg->remove ( QLatin1String ( "HeaderDefinitions" ) );
  if ( rows >= 1 )
  {
    cfg->beginGroup ( QLatin1String ( "HeaderDefinitions" ) );
    for ( int r = 0; r < rows; r++ )
    {
      if ( !headersTable->item ( r, 0 ) || !headersTable->item ( r, 1 ) )
        continue; // Verhindert das Leere Zeilen aufgerufen werden!

      QString key = headersTable->item ( r, 0 )->data ( Qt::EditRole ).toString();
      QString val = headersTable->item ( r, 1 )->data ( Qt::EditRole ).toString();
      if ( ! exclude.contains ( key.toLower() ) )
        cfg->setValue ( key, val );
    }
    cfg->endGroup();
  }
}

/**
* Einen Eintrag in der Datenkopf Tabelle einfügen
*/
void ConfigHeaderDefinitions::addHeaderItem()
{
  headersTable->setRowCount ( ( headersTable->rowCount() + 1 ) );
}

/**
* Den ausgewählten Eintrag aus der Datenkopf Tabelle entfernen!
*/
void ConfigHeaderDefinitions::removeHeaderItem()
{
  foreach ( QTableWidgetItem* item, headersTable->selectedItems() )
  {
    int row = item->row();
    headersTable->selectRow ( row );
    QTableWidgetItem* it1 = headersTable->item ( row, 0 );
    delete it1;
    QTableWidgetItem* it2 = headersTable->item ( row, 1 );
    delete it2;
    headersTable->removeRow ( row );
  }
}

ConfigHeaderDefinitions::~ConfigHeaderDefinitions()
{}
