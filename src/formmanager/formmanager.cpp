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
#include "formdblist.h"

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

/* QtXml */
// #include <QtXml/>

/* QtWebKit */
#include <QtWebKit/QWebFrame>

/* KDE */
#include <KDE/KLocale>

/**
* TODO Der FormManager soll vollgendes machen!
* \li Anzahl der Formulare einer Seite ermitteln
* \li Einen Unique MD5 Hash aus URL und den Element Namen des Formulares für die Datenbank erstellen!\n
*     Das komplette einlesen des Forms wird wegen verschiedener Inhalte nicht gehen :-/
* \li Das Formular in einen Byte String Konvertieren und in die Datenbank Speichern
*/
FormManager::FormManager ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "FormManager" ) );
  setWindowTitle ( i18n ( "Forms" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 1, 1, 1, 1 );

  m_splitter = new QSplitter ( Qt::Vertical, this );
  m_splitter->setObjectName ( QLatin1String ( "FormManager/Splitter" ) );

  m_pageWidget = new KPageWidget ( m_splitter );
  m_pageWidget->setObjectName ( QLatin1String ( "FormManager/Splitter/KPageWidget" ) );
  m_splitter->insertWidget ( 0, m_pageWidget );

  m_pageWidget->addPage ( new QTreeWidget ( this ), i18n ( "Forms TEST Widget" ) );

  m_formDBList = new FormDBList ( m_splitter );
  m_splitter->insertWidget ( 1, m_formDBList );

  setWidget ( m_splitter );
  forms.clear();
}

/**
* Suche nach Formularen und erstelle die Suchschlüssel für die Datenbank.
*/
void FormManager::setPageContent ( const QUrl &url, const QWebElement &element )
{
  if ( ! toggleViewAction()->isChecked() || ! isVisible() )
    return;

  QWebElementCollection collection = element.findAll ( "FORM" );
  if ( collection.count() > 0 )
  {
    qDebug() << Q_FUNC_INFO << "Todo db search" << url;
    foreach ( QWebElement e, collection )
    {
      if ( e.hasAttribute ( QLatin1String ( "action" ) ) && e.hasAttribute ( QLatin1String ( "method" ) ) )
      {
        if ( e.attribute ( "action", QString::null ).isEmpty() )
          continue;

        forms.append ( FormConstructor ( e ) );
      }
    }
  }
}

FormManager::~FormManager()
{
  forms.clear();
}
