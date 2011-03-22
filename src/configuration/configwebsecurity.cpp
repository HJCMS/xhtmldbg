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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "configwebsecurity.h"
#include "webdatabasehandler.h"
#include "dbmanager.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>
#include <QtGui/QVBoxLayout>

/* QtSql */
#include <QtSql/QSqlDatabase>

/* QtWebKit */
#include <QtWebKit/QWebSecurityOrigin>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KPushButton>
#include <KDE/KLocale>

/** @class ConfigWebSecurity */
ConfigWebSecurity::ConfigWebSecurity ( QWidget * parent )
    : PageWidget ( i18n ( "HTML 5 Database" ), parent )
    , dbConnectionName ( QLatin1String ( "HTML5SecurityOptions" ) )
    , defaultQuota ( ( 3 * 1024 ) )
{
  setObjectName ( QLatin1String ( "config_web_security" ) );
  setNotice ( true );
  setCheckable ( false );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( centralWidget );

  QStringList information;
  information << i18n ( "The upcoming HTML 5 standard includes support for SQL databases that web sites can create and access on a local computer through JavaScript." );
  information << i18n ( "Only Web sites with the same security options can access each other's resources for client-side scripting or databases." );
  information << i18n ( "You need to add a hostname, scheme and port to allow access to your local computer." );

  QLabel* title = new QLabel ( centralWidget );
  title->setWordWrap ( true );
  title->setTextFormat ( Qt::RichText );
  title->setText ( information.join ( "<br/>" ) );
  verticalLayout->addWidget ( title );

  m_table = new WebSecurityTable ( centralWidget );
  verticalLayout->addWidget ( m_table );

  QGroupBox* groupBox = new QGroupBox ( i18n ( "Edit" ), centralWidget );
  QGridLayout* gridLayout = new QGridLayout ( groupBox );

  gridLayout->addWidget ( new QLabel ( i18n ( "Hostname" ), groupBox ), 0, 0, 1, 1, Qt::AlignRight );
  m_hostname = new KLineEdit ( groupBox );
  gridLayout->addWidget ( m_hostname, 0, 1, 1, 1 );
  gridLayout->addWidget ( new QLabel ( i18n ( "e.g. domain.tld" ), groupBox ), 0, 2, 1, 1, Qt::AlignLeft );

  gridLayout->addWidget ( new QLabel ( i18n ( "Scheme" ), groupBox ), 1, 0, 1, 1, Qt::AlignRight );
  m_scheme = new KComboBox ( groupBox );
  m_scheme->setEditable ( false );
  m_scheme->addItem ( KIcon ( "menu-development-web" ), i18n ( "World Wide Web HTTP" ), QString::fromUtf8 ( "http" ) );
  m_scheme->addItem ( KIcon ( "menu-development-web" ), i18n ( "World Wide Web over TLS/SSL HTTPS" ), QString::fromUtf8 ( "https" ) );
  // NOTE Das FTP Protokoll wird im Moment NICHT unterstützt!
  // m_scheme->addItem ( KIcon ( "menu-development-web" ), i18n ( "File Transfer Protocol FTP" ), QString::fromUtf8 ( "ftp" ) );
  m_scheme->addItem ( KIcon ( "menu-development-web" ), i18n ( "Local File Protocol FILE" ), QString::fromUtf8 ( "file" ) );
  gridLayout->addWidget ( m_scheme, 1, 1, 1, 1, Qt::AlignLeft );
  gridLayout->addWidget ( new QLabel ( i18n ( "e.g. http://, https:// or file://" ), groupBox ), 1, 2, 1, 1, Qt::AlignLeft );

  // Optional den Port festlegen!
  gridLayout->addWidget ( new QLabel ( i18n ( "Port" ), groupBox ), 2, 0, 1, 1, Qt::AlignRight );
  m_port = new KIntNumInput ( 80, groupBox );
  m_port->setRange ( 0, 65535 );
  m_port->setEnabled ( false );
  gridLayout->addWidget ( m_port, 2, 1, 1, 1, Qt::AlignLeft );
  // CheckBox
  m_checkBox = new QCheckBox ( i18n ( "Aberrantly Port Address from Scheme" ), groupBox );
  gridLayout->addWidget ( m_checkBox, 2, 2, 1, 1, Qt::AlignLeft );

  KPushButton* reset = new KPushButton ( i18n ( "Reset" ), groupBox );
  gridLayout->addWidget ( reset, 3, 0, 1, 1, Qt::AlignLeft );

  KPushButton* submit = new KPushButton ( i18n ( "Submit" ), groupBox );
  gridLayout->addWidget ( submit, 3, 2, 1, 1, Qt::AlignRight );

  groupBox->setLayout ( gridLayout );
  verticalLayout->addWidget ( groupBox, Qt::AlignLeft );

  QHBoxLayout* horzontalLayout = new QHBoxLayout;
  horzontalLayout->addWidget ( new QLabel ( i18n ( "Sets the quota for the databases in the security origin to quota bytes." ), centralWidget ) );
  m_dbQuota = new KIntNumInput ( ( 3 * 1024 ), centralWidget );
  m_dbQuota->setRange ( 1024, ( 100 * 1024 ), 5 );
  m_dbQuota->setSliderEnabled ( true );
  m_dbQuota->setSuffix ( QLatin1String ( " Bytes" ) );
  horzontalLayout->addWidget ( m_dbQuota );
  verticalLayout->addLayout ( horzontalLayout );

  // QWebSettings::enablePersistentStorage
  m_persistentStorage = new QCheckBox ( i18n ( "Enable Persistent Storage" ), this );
  verticalLayout->addWidget ( m_persistentStorage );

  centralWidget->setLayout ( verticalLayout );

  connect ( m_table, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( itemRowChanged ( int ) ) );

  connect ( m_table, SIGNAL ( modified ( bool ) ),
            this, SLOT ( itemModified ( bool ) ) );

  connect ( m_scheme, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( schemeChanged ( int ) ) );

  connect ( m_dbQuota, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( quotaChanged ( int ) ) );

  connect ( m_checkBox, SIGNAL ( toggled ( bool ) ), m_port, SLOT ( setEnabled ( bool ) ) );
  connect ( reset, SIGNAL ( clicked () ), this, SLOT ( clearInput() ) );
  connect ( submit, SIGNAL ( clicked () ), this, SLOT ( itemSubmitted() ) );
}

QTableWidgetItem* ConfigWebSecurity::createItem ( const QVariant &data ) const
{
  QTableWidgetItem* item = new QTableWidgetItem ( data.toString(), QTableWidgetItem::UserType );
  item->setData ( Qt::UserRole, data );
  return item;
}

void ConfigWebSecurity::fillTable ( QList<WebSecurityItem*> &list )
{
  int size = list.size();
  if ( size < 1 )
    return;

  m_table->clearContents();
  m_table->setRowCount ( size );
  for ( int r = 0; r < size; ++r )
  {
    WebSecurityItem* it = list.at ( r );
    m_table->setItem ( r, 0, it->hostItem() );
    m_table->setItem ( r, 1, it->portItem() );
    m_table->setItem ( r, 2, it->schemeItem() );
    m_table->setItem ( r, 3, it->quotaItem() );
  }
  list.clear();
}

/**  SQL Daten in Tabelle schreiben! */
void ConfigWebSecurity::loadSQLData()
{
  WebDatabaseHandler db ( QSqlDatabase::database ( QLatin1String ( XHTMLDBG_APPS_NAME ), false )
                          , dbConnectionName );
  QList<WebSecurityItem*> origins = db.getOrigins();
  fillTable ( origins );
  db.close();
  origins.clear();
  sighted = true;
}

void ConfigWebSecurity::saveSQLData()
{
  int size = m_table->rowCount();
  if ( size < 1 )
    return;

  QList<WebSecurityItem*> origins;
  for ( int r = 0; r < size; ++r )
  {
    WebSecurityItem* it = new WebSecurityItem ( "dummy", defaultQuota );
    it->setHost ( m_table->item ( r, 0 )->data ( Qt::UserRole ) );
    it->setPort ( m_table->item ( r, 1 )->data ( Qt::UserRole ) );
    it->setScheme ( m_table->item ( r, 2 )->data ( Qt::UserRole ) );
    it->setQuota ( m_table->item ( r, 3 )->data ( Qt::UserRole ) );
    origins.append ( it );
  }
  WebDatabaseHandler db ( QSqlDatabase::database ( QLatin1String ( XHTMLDBG_APPS_NAME ), false )
                          , dbConnectionName );
  db.saveOrigins ( origins );
  db.close();
}

void ConfigWebSecurity::itemRowChanged ( int r )
{
  QString scheme = m_table->item ( r, 2 )->data ( Qt::UserRole ).toString();
  int port = m_table->item ( r, 1 )->data ( Qt::UserRole ).toUInt();
  if ( scheme == QString::fromUtf8 ( "file" ) || port == 0 )
    m_scheme->setCurrentIndex ( 2 );
  else if ( scheme == QString::fromUtf8 ( "https" ) )
    m_scheme->setCurrentIndex ( 1 );
  else
    m_scheme->setCurrentIndex ( 0 );

  m_dbQuota->setValue ( m_table->item ( r, 3 )->data ( Qt::UserRole ).toUInt() );
  m_port->setValue ( port );
  m_hostname->setText ( m_table->item ( r, 0 )->data ( Qt::UserRole ).toString() );
}

void ConfigWebSecurity::schemeChanged ( int index )
{
  QList<int> po;
  po.append ( 80 ); // http://
  po.append ( 443 ); // https://
  po.append ( 0 ); // file://
  if ( index >= 0 || index <= 3 )
    m_port->setValue ( po[index] );
}

void ConfigWebSecurity::quotaChanged ( int )
{
  itemModified ( true );
}

void ConfigWebSecurity::clearInput()
{
  m_hostname->clear();
  m_port->setValue ( 80 );
  m_scheme->setCurrentIndex ( 0 );
  m_checkBox->setChecked ( false );
}

void ConfigWebSecurity::itemSubmitted()
{
  QString hostname = m_hostname->text();
  if ( hostname.isEmpty() )
  {
    m_hostname->setFocus();
    return;
  }

  int port  = m_port->value();
  int quota  = m_dbQuota->value();
  QString scheme = m_scheme->itemData ( m_scheme->currentIndex () ).toString();
  // Wenn ein Datensatz ausgewählt, die Hostnamen Identisch sind, dann Updaten!
  int row = m_table->currentRow();
  if ( m_table->item ( row, 0 ) )
  {
    QString tmpHost = m_table->item ( row, 0 )->data ( Qt::UserRole ).toString();
    if ( tmpHost == hostname )
    {
      m_table->item ( row, 1 )->setData ( Qt::DisplayRole, QString::number ( port ) );
      m_table->item ( row, 1 )->setData ( Qt::UserRole, port );
      m_table->item ( row, 2 )->setData ( Qt::DisplayRole, scheme );
      m_table->item ( row, 2 )->setData ( Qt::UserRole, scheme );
      m_table->item ( row, 3 )->setData ( Qt::DisplayRole, quota );
      m_table->item ( row, 3 )->setData ( Qt::UserRole, quota );
      m_table->update();
      itemModified ( true );
      return;
    }
  }
  // Einen Neuen Eintrag erzeugen!
  m_table->setRowCount ( ( m_table->rowCount() + 1 ) );
  row = m_table->rowCount() - 1;
  m_table->setItem ( row, 0, createItem ( hostname ) );
  m_table->setItem ( row, 1, createItem ( port ) );
  m_table->setItem ( row, 2, createItem ( scheme ) );
  m_table->setItem ( row, 3, createItem ( m_dbQuota->value() ) );
  itemModified ( true );
}

void ConfigWebSecurity::itemModified ( bool b )
{
  mod = b;
  emit modified ( b );
}

void ConfigWebSecurity::load ( Settings * cfg )
{
  qint64 quota = cfg->value ( "WebDatabase/DefaultQuota", defaultQuota ).toUInt();
  m_dbQuota->setValue ( quota );
  m_persistentStorage->setChecked ( cfg->value ( "WebDatabase/EnablePersistentStorage", false ).toBool() );
  loadSQLData();
}

/**
* Aktuelle in QWebSettings::offlineStoragePath ()+"/Databases.db" speichern!
*/
void ConfigWebSecurity::save ( Settings * cfg )
{
  cfg->setValue ( "WebDatabase/DefaultQuota", m_dbQuota->value() );
  cfg->setValue ( "WebDatabase/EnablePersistentStorage", m_persistentStorage->isChecked() );
  saveSQLData();
}

bool ConfigWebSecurity::isModified ()
{
  return mod;
}

bool ConfigWebSecurity::isSighted ()
{
  return sighted;
}

ConfigWebSecurity::~ConfigWebSecurity()
{
  m_table->clearContents();
  QSqlDatabase::removeDatabase ( dbConnectionName );
}
