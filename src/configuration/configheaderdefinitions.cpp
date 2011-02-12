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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

ConfigHeaderDefinitions::ConfigHeaderDefinitions ( QWidget * parent )
    : QGroupBox ( i18n ( "Header Field Definitions" ), parent )
{
  setObjectName ( QLatin1String ( "configheaderdefinitions" ) );
  setFlat ( true );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( this );
  verticalLayout->setObjectName ( QLatin1String ( "configheaderdefinitions_vertical_layout" ) );

  QLabel* lr0 = new QLabel ( this );
  lr0->setText ( i18n ( "Warning: Do not edit this Table if you not now how it works! For more information see <a href=\"%1\">rfc2616</a>" )
                 .arg ( "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14" ) );
  lr0->setAlignment ( ( Qt::AlignJustify | Qt::AlignTop ) );
  lr0->setWordWrap ( true );
  lr0->setIndent ( 2 );
  lr0->setOpenExternalLinks ( true );
  lr0->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  verticalLayout->addWidget ( lr0 );

  headersTable = new QTableWidget ( this );
  headersTable->setColumnCount ( 2 );
  headersTable->setHorizontalHeaderItem ( 0, new QTableWidgetItem ( i18n ( "Parameter" ) ) );
  headersTable->setHorizontalHeaderItem ( 1, new QTableWidgetItem ( i18n ( "Values" ) ) );
  headersTable->setProperty ( "showDropIndicator", QVariant ( false ) );
  headersTable->setDragDropOverwriteMode ( false );
  headersTable->setWordWrap ( false );
  headersTable->horizontalHeader()->setDefaultSectionSize ( 150 );
  headersTable->horizontalHeader()->setStretchLastSection ( true );
  headersTable->verticalHeader()->setVisible ( false );
  verticalLayout->addWidget ( headersTable );

  QHBoxLayout* horizontalLayout = new QHBoxLayout;
  horizontalLayout->setObjectName ( QLatin1String ( "configheaderdefinitions_horizontal_layout" ) );

  QSpacerItem* spacer = new QSpacerItem ( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  horizontalLayout->addItem ( spacer );

  QToolButton* btn1 = new QToolButton ( this );
  btn1->setObjectName ( QLatin1String ( "add_header_definition_button" ) );
  btn1->setIcon ( QIcon::fromTheme ( QLatin1String ( "list-add" ) ) );
  horizontalLayout->addWidget ( btn1 );

  QToolButton* btn2 = new QToolButton ( this );
  btn2->setObjectName ( QLatin1String ( "remove_header_definition_button" ) );
  btn2->setIcon ( QIcon::fromTheme ( QLatin1String ( "list-remove" ) ) );
  horizontalLayout->addWidget ( btn2 );

  verticalLayout->addLayout ( horizontalLayout );

  setLayout ( verticalLayout );

  // Tables
  connect ( headersTable, SIGNAL ( itemChanged ( QTableWidgetItem * ) ),
            this, SLOT ( itemChanged ( QTableWidgetItem * ) ) );
  connect ( btn1, SIGNAL ( clicked() ), this, SLOT ( addHeaderItem() ) );
  connect ( btn2, SIGNAL ( clicked() ), this, SLOT ( removeHeaderItem() ) );
}

/**
* Wenn ein Eintrag verändert wurde das signal @ref modified abstoßen.
*/
void ConfigHeaderDefinitions::itemChanged ( QTableWidgetItem * item )
{
  // keine Änderungen
  if ( ! item->isSelected() )
    return;

  emit modified ( true );
}

/**
* Lade die Datenkopf Definitionen aus der Konfiguration
*/
void ConfigHeaderDefinitions::loadHeaderDefinitions ( Settings * cfg )
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
void ConfigHeaderDefinitions::saveHeaderDefinitions ( Settings * cfg )
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
  emit modified ( true );
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
    emit modified ( true );
  }
}

ConfigHeaderDefinitions::~ConfigHeaderDefinitions()
{}
