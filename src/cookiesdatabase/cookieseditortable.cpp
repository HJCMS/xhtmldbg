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
#include "cookieseditormodel.h"
#include "cookiesdatabaselocation.h"
#include "cookieseditortabledelegation.h"

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

CookiesEditorTable::CookiesEditorTable ( QWidget * parent )
    : QTableView ( parent )
    , sql ( QSqlDatabase::addDatabase ( "QSQLITE", QString::fromUtf8 ( "CookiesHandle" ) ) )
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
  setCornerButtonEnabled ( false );

  horizontalHeader()->setDefaultSectionSize ( 125 );
  horizontalHeader()->setHighlightSections ( false );
  horizontalHeader()->setMinimumSectionSize ( 125 );
  horizontalHeader()->setProperty ( "showSortIndicator", QVariant ( true ) );
  horizontalHeader()->setStretchLastSection ( true );
  verticalHeader()->setVisible ( false );

  // CookiesEditorModel
  sql.setConnectOptions ( QString::fromUtf8 ( "QSQLITE_ENABLE_SHARED_CACHE=1" ) );
  QString lc = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  CookiesDatabaseLocation* locator = new CookiesDatabaseLocation ( lc, this );
  if ( locator->initDatabase ( sql, "cookieshandle" ) )
  {
    sql.setDatabaseName ( locator->databasePath ( "cookieshandle" ) );
    if ( ! sql.open() )
      qWarning ( "(XHTMLDBG) Database: %s", qPrintable ( sql.lastError().text() ) );
  }
  delete locator;

  m_model = new CookiesEditorModel ( sql, this );
  setModel ( m_model );

  setItemDelegate ( new CookiesEditorTableDelegation ( this ) );

  horizontalHeader()->setResizeMode ( QHeaderView::ResizeToContents );

  connect ( this, SIGNAL ( doubleClicked ( const QModelIndex & ) ),
            this, SLOT ( cellChanged ( const QModelIndex & ) ) );
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

//   foreach ( QTableWidgetItem* item, findItems ( txt, Qt::MatchContains ) )
//   {
//     setCurrentItem ( item );
//     break;
//   }
}

/**
* Lädt die Cookie Regelungen aus der xhtmldbg.conf
*/
void CookiesEditorTable::loadCookieAccess ()
{
  m_model->select();
}

/**
* Alle Cookies Speichern
*/
void CookiesEditorTable::saveCookieAccess ()
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

/**
* Die Ausgewählten einträge entfernen!
*/
void CookiesEditorTable::removeItem()
{
  if ( currentIndex ().isValid() )
  {
    int row = currentIndex ().row();
    if ( m_model->index ( row, 0 ).isValid() )
    {
//       if ( m_model->editStrategy() !=  QSqlTableModel::OnRowChange )
//         m_model->setEditStrategy ( QSqlTableModel::OnRowChange );

      if ( ! m_model->removeRow ( row ) )
        return;

      if ( ! m_model->submit() )
        return;
    }
    emit modified();
  }
}

/**
* Tabelle Leeren
*/
void CookiesEditorTable::removeAll()
{
//   clearContents();
//   setRowCount ( 0 );
//   emit modified();
}

/**
* Einen Neuen Eintrag einfügen
*/
bool CookiesEditorTable::addCookie ( int t, const QString &h )
{
//   if ( h.isEmpty() || t < 0 )
//     return false;
//
//   if ( rowCount() >= 1 )
//   {
//     for ( int r = 0; r < rowCount(); r++ )
//     {
//       QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
//       if ( key == h )
//       {
//         setCurrentCell ( r, 0 );
//         return false;
//       }
//     }
//   }
//   setRowCount ( rowCount() + 1 );
//   int row = ( rowCount() - 1 );
//   setItem ( row, 0, new QTableWidgetItem ( h ) );
//   setCellWidget ( row, 1, new CookieAccessComboBox ( h, t, this ) );
//   emit modified();
  return false;
}

CookiesEditorTable::~CookiesEditorTable()
{
  if ( sql.isOpen() )
    sql.close();
}

