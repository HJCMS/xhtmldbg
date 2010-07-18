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

#include "cookieseditortable.h"
#include "cookieaccesscombobox.h"
#include "cookiesboolcombobox.h"
#include "cookiesdatabaselocation.h"

/* QtCore */
#include <QtCore/QAbstractTableModel>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QDesktopServices>
#include <QtGui/QHeaderView>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QSizePolicy>
#include <QtGui/QTableWidgetItem>

/* QtSql */
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

CookiesEditorTable::CookiesEditorTable ( QWidget * parent, const QString &dbName )
    : QTableWidget ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", dbName ) )
    , connectionName ( dbName )
{
  setObjectName ( QLatin1String ( "editcookiestable" ) );
  setMinimumHeight ( 250 );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
  setContextMenuPolicy ( Qt::NoContextMenu );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setTabKeyNavigation ( true );
  setProperty ( "showDropIndicator", QVariant ( false ) );
  setDragDropOverwriteMode ( false );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setGridStyle ( Qt::DashLine );
  setWordWrap ( false );
  setRowCount ( 0 );

  QStringList lb;
  lb << ( trUtf8 ( "Domain" ) ) << trUtf8 ( "Arrangement" );
  lb << trUtf8 ( "Allow third party" ) << trUtf8 ( "rfc2109" );
  setColumnCount ( lb.size() );
  setHorizontalHeaderLabels ( lb );

  horizontalHeader()->setDefaultSectionSize ( 125 );
  horizontalHeader()->setMinimumSectionSize ( 125 );
  horizontalHeader()->setHighlightSections ( false );
  horizontalHeader()->setProperty ( "showSortIndicator", QVariant ( false ) );
  horizontalHeader()->setStretchLastSection ( false );
  verticalHeader()->setVisible ( false );

  connect ( this, SIGNAL ( doubleClicked ( const QModelIndex & ) ),
            this, SLOT ( cellChanged ( const QModelIndex & ) ) );
}

/**
* Die Datenbank Initialisieren
*/
bool CookiesEditorTable::initialDatabase()
{
  if ( sql.isOpen() )
    return true; // wenn offen nichts machen

  QString dbConnectionName;
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  CookiesDatabaseLocation* locator = new CookiesDatabaseLocation ( lc, this );
  if ( locator->initDatabase ( sql, "cookieshandle" ) )
    dbConnectionName = locator->databasePath ( "cookieshandle" );

  delete locator;

  if ( dbConnectionName.isEmpty() )
  {
    qWarning ( "(XHTMLDBG) Cannot find SQLite Database" );
    return false;
  }

  sql.setDatabaseName ( dbConnectionName );
  if ( ! sql.open() )
  {
    qWarning ( "(XHTMLDBG) SQLite:Error - %s", qPrintable ( sql.lastError().text() ) );
    return false;
  }
  else
    return true;
}

/**
* Wenn eine Tabellen Zelle geändert wurde das
* signal @ref modified senden!
*/
void CookiesEditorTable::cellChanged ( const QModelIndex &m )
{
  Q_UNUSED ( m )
  emit modified();
}

/**
* Sucht ab dem 3 zeichen in der Tabelle nach übereinstimmungen
* und Markiert den erst besten Eintrag der gefunden wird.
*/
void CookiesEditorTable::markCookie ( const QString &txt )
{
  if ( txt.length() < 4 )
    return;

  foreach ( QTableWidgetItem* item, findItems ( txt, Qt::MatchStartsWith ) )
  {
    setCurrentItem ( item );
    break;
  }
}

/**
* Lädt die Cookie Regelungen aus der xhtmldbg.conf
*/
void CookiesEditorTable::loadCookieAccess ()
{
  if ( ! initialDatabase() )
    return;

  QString queryString ( "SELECT Hostname,AccessType,AllowThirdParty,RFC2109 " );
  queryString.append ( "FROM cookieshandle WHERE ( Hostname != '' ) ORDER BY Hostname ASC;" );
  QSqlQuery query = sql.exec ( queryString );
  if ( query.lastError().type() == QSqlError::NoError )
  {
    QSqlRecord rec = query.record();
    if ( rec.count() > 0 )
    {
      clearContents();
      int row = 0;

      while ( query.next() )
      {
        setRowCount ( ( row + 1 ) );
        // Hostname
        setItem ( row, 0, new QTableWidgetItem ( query.value ( rec.indexOf ( "Hostname" ) ).toString() ) );
        // AccessType
        CookieAccessComboBox* item0 = new CookieAccessComboBox ( this, query.value ( rec.indexOf ( "AccessType" ) ).toUInt() );
        connect ( item0, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
        setCellWidget ( row, 1, item0 );
        // AllowThirdParty
        CookiesBoolComboBox* item2 = new CookiesBoolComboBox ( this, query.value ( rec.indexOf ( "AllowThirdParty" ) ).toUInt() );
        connect ( item2, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
        setCellWidget ( row, 2, item2 );
        // RFC2109
        CookiesBoolComboBox* item3 = new CookiesBoolComboBox ( this, query.value ( rec.indexOf ( "RFC2109" ) ).toUInt() );
        connect ( item3, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
        setCellWidget ( row, 3, item3 );
        row++;
      }
    }
    query.finish();
  }
  horizontalHeader()->resizeSections ( QHeaderView::ResizeToContents );
}

/**
* Alle Cookies Speichern
* @code
* PRAGMA foreign_keys=OFF;
* BEGIN TRANSACTION;
* CREATE TABLE "cookieshandle" (
*   "Hostname" TEXT PRIMARY KEY ASC default '',
*   "AccessType" INTEGER default 0,
*   "AllowThirdParty" INTEGER default 0,
*   "RFC2109" INTEGER default 1
* );
* INSERT INTO "cookieshandle" VALUES('klettern-ettringen.de',0,1,1);
* INSERT INTO "cookieshandle" VALUES('hjcms.de',2,0,1);
* COMMIT;
* @endcode
*/
void CookiesEditorTable::saveCookieAccess ()
{
  if ( ! initialDatabase() )
    return;

  QString sqlPragma ( "INSERT INTO cookieshandle (Hostname,AccessType,AllowThirdParty,RFC2109) VALUES ('" );
  QStringList queryStrings;
  int rows = rowCount();
  if ( rows >= 1 )
  {
    for ( int r = 0; r < rows; r++ )
    {
      QString values ( sqlPragma );
      // Hostname
      values.append ( item ( r, 0 )->data ( Qt::EditRole ).toString() );
      values.append ( "'," );

      // AccessType
      int item1Value = qobject_cast<CookieAccessComboBox*> ( cellWidget ( r, 1 ) )->value();
      values.append ( QString::number ( item1Value ) );
      values.append ( "," );

      // AllowThirdParty
      int item2Value = qobject_cast<CookiesBoolComboBox*> ( cellWidget ( r, 2 ) )->value();
      values.append ( QString::number ( item2Value ) );
      values.append ( "," );

      // RFC2109
      int item3Value = qobject_cast<CookiesBoolComboBox*> ( cellWidget ( r, 3 ) )->value();
      values.append ( QString::number ( item3Value ) );
      values.append ( "); " );

      queryStrings.append ( values );
    }
  }

  if ( queryStrings.size() > 0 )
  {
    QSqlQuery query;
    if ( sql.transaction () )
    {
      query = sql.exec ( QString::fromUtf8 ( "DELETE FROM cookieshandle WHERE (Hostname!='');" ) );
      if ( query.lastError().type() != QSqlError::NoError )
        return;

      foreach ( QString sqlQuery, queryStrings )
      {
        sql.exec ( sqlQuery );
      }
      sql.commit ();
      query.finish();
    }
  }
}

/**
* Die Ausgewählten einträge entfernen!
*/
void CookiesEditorTable::removeItem()
{
  QList<QTableWidgetItem*> items = selectedItems();
  if ( items.size() < 1 )
    return;

  int r = row ( items.at ( 0 ) );
  if ( r >= 0 )
  {
    removeCellWidget ( r, 1 );
    removeCellWidget ( r, 2 );
    removeCellWidget ( r, 3 );
    removeRow ( r );
    horizontalHeader()->resizeSections ( QHeaderView::ResizeToContents );
    emit modified();
  }
}

/**
* Tabelle Leeren
*/
void CookiesEditorTable::removeAll()
{
  clearContents();
  setRowCount ( 0 );
  emit modified();
}

/**
* Einen Neuen Eintrag einfügen
*/
bool CookiesEditorTable::addCookie ( int t, const QString &h, int rfc )
{
  if ( h.isEmpty() || t < 0 )
    return false;

  if ( rowCount() >= 1 )
  {
    for ( int r = 0; r < rowCount(); r++ )
    {
      QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
      if ( key == h )
      {
        setCurrentCell ( r, 0 );
        return false;
      }
    }
  }

  setRowCount ( rowCount() + 1 );
  int row = ( rowCount() - 1 );

  // Hostname
  setItem ( row, 0, new QTableWidgetItem ( h ) );
  // AccessType
  CookieAccessComboBox* item0 = new CookieAccessComboBox ( this, t );
  connect ( item0, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
  setCellWidget ( row, 1, item0 );
  // AllowThirdParty
  CookiesBoolComboBox* item2 = new CookiesBoolComboBox ( this, 0 );
  connect ( item2, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
  setCellWidget ( row, 2, item2 );
  // RFC2109
  CookiesBoolComboBox* item3 = new CookiesBoolComboBox ( this, rfc );
  connect ( item3, SIGNAL ( itemChanged() ), this, SIGNAL ( modified() ) );
  setCellWidget ( row, 3, item3 );

  horizontalHeader()->resizeSections ( QHeaderView::ResizeToContents );
  emit modified();
  return true;
}

/**
* Lädt die Cookie Regelungen aus der xhtmldbg.conf
*/
void CookiesEditorTable::addCookiesFromOldConfig ( QSettings * cfg )
{
  Q_ASSERT ( cfg );

  if ( !cfg->contains ( QLatin1String ( "CookieArrangement" ) ) )
    return;

  cfg->beginGroup ( QLatin1String ( "CookieArrangement" ) );
  QStringList keys = cfg->allKeys();
  if ( keys.size() >= 1 )
  {
    foreach ( QString key, keys )
    {
      int val = cfg->value ( key ).toUInt();
      if ( val < 0 )
        continue;

      addCookie ( val, key, 1 );
      cfg->remove ( key );
    }
  }
  cfg->endGroup();
  cfg->remove ( QLatin1String ( "CookieArrangement" ) );
}

CookiesEditorTable::~CookiesEditorTable()
{
  if ( QSqlDatabase::database ( connectionName, false ).isOpen() )
    QSqlDatabase::database ( connectionName, false ).close();
}
