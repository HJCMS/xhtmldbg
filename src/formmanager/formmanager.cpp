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

#include "formmanager.h"
#include "formconstructor.h"
#include "formbookmarks.h"
#include "formentry.h"

/* QtCore */
#include <QtCore/QDebug>
// #include <QtCore/QFile>
// #include <QtCore/QRect>
// #include <QtCore/QPoint>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QTreeWidget>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

/* KDE */
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KWebWallet>
#include <KDE/KWebPage>

/* QtSql */
#include <QtSql/QSqlError>

/**
* TODO Der FormManager soll vollgendes machen!
* \li Anzahl der Formulare einer Seite ermitteln
* \li Einen Unique MD5 Hash aus URL und den Element Namen des Formulares für die Datenbank erstellen!\n
*     Das komplette einlesen des Forms wird wegen verschiedener Inhalte nicht gehen :-/
* \li Das Formular in einen Byte String Konvertieren und in die Datenbank Speichern
* \code
* CREATE TABLE formmanager (
*   url TEXT UNIQUE NOT NULL ON CONFLICT FAIL,
*   project TEXT NOT NULL DEFAULT 'Form 0',
*   autoinsert INTEGER default 0
* );
* CREATE TABLE forms (
*   ident TEXT UNIQUE NOT NULL ON CONFLICT FAIL,
*   formdata TEXT
* );
* \endcode
*/
FormManager::FormManager ( DBManager* db, QWidget * parent )
    : QDockWidget ( parent )
    , m_dbManager ( db )
{
  setObjectName ( QLatin1String ( "FormManager" ) );
  setWindowTitle ( i18n ( "Form Manager" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  m_scrollArea = new QScrollArea ( this );
  m_scrollArea->setWidgetResizable ( true );

  m_splitter = new QSplitter ( Qt::Horizontal, m_scrollArea );
  m_splitter->setObjectName ( QLatin1String ( "FormManager/Splitter" ) );
  m_scrollArea->setWidget ( m_splitter );

  m_formBookmarks = new FormBookmarks ( m_splitter );
  m_splitter->insertWidget ( 0, m_formBookmarks );

  m_tabWidget = new KTabWidget ( m_splitter );
  m_tabWidget->setObjectName ( QLatin1String ( "FormManager/Splitter/KTabWidget" ) );
  m_tabWidget->setMinimumWidth ( 350 );
  m_splitter->insertWidget ( 1, m_tabWidget );

  setWidget ( m_scrollArea );

  // Alle bestehenden Lesezeichen einfügen
  createBookmarkItems();
}

/** Lesezeichen Einträge erstellen  */
void FormManager::createBookmarkItems()
{
  QSqlQuery q = m_dbManager->select ( "SELECT url,project,autoinsert FROM formmanager WHERE (url IS NOT NULL) GROUP BY project;" );
  QSqlRecord r = q.record();
  if ( r.count() > 0 )
  {
    while ( q.next() )
    {
      QUrl url = q.value ( r.indexOf ( "url" ) ).toUrl();
      if ( url.isValid() )
      {
        QString prj = q.value ( r.indexOf ( "project" ) ).toString();
        FormBookmarkItem* item = new FormBookmarkItem ( url );
        item->setAutoinsert ( q.value ( r.indexOf ( "autoinsert" ) ).toBool() );
        m_formBookmarks->addItem ( ( prj.isEmpty() ? i18n ( "Default" ) : prj ), item );
      }
    }
    q.finish();
  }
}

void FormManager::addFormTable ( const QUrl &url, const FormConstructor &form )
{
  if ( form.elements.size() > 0 )
  {
    FormEntry* entry = new FormEntry ( form, url, m_tabWidget );
    entry->setUniqueId ( form.uniqueId() );
    entry->setObjectName ( form.uniqueId() );
    m_tabWidget->insertTab ( m_tabWidget->count(), entry, KIcon ( "xhtmldbg" ), form.uniqueId() );
    return;
  }
#ifdef DEBUG_VERBOSE
  qDebug() << Q_FUNC_INFO << url << "form.elements.size() = " << form.elements.size() << form.uniqueId();
#endif
}

/**
* Suche nach Formularen und erstelle die Suchschlüssel für die Datenbank.
*/
void FormManager::setPageContent ( const QUrl &url, const QWebElement &element )
{
  if ( ! toggleViewAction()->isChecked() || ! isVisible() )
    return;

  m_tabWidget->clear();
  QWebElementCollection collection = element.findAll ( "FORM" );
  if ( collection.count() > 0 )
  {
    foreach ( QWebElement e, collection )
    {
      if ( e.hasAttribute ( QLatin1String ( "action" ) ) && e.hasAttribute ( QLatin1String ( "method" ) ) )
      {
        if ( e.attribute ( "action", QString::null ).isEmpty() )
          continue;

        addFormTable ( url, FormConstructor ( e ) );
      }
    }
  }
}

FormManager::~FormManager()
{
  m_tabWidget->clear();
}
