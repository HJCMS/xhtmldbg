/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "keywordstoolbar.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QCompleter>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QIcon>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

KeywordsToolBar::KeywordsToolBar ( QWidget * parent )
    : QToolBar ( parent )
{
  setObjectName ( QLatin1String ( "keywordstoolbar" ) );
  setWindowTitle ( i18n ( "Keywords" ) );
  setStatusTip ( i18n ( "Show Keywords for Search Engine Optimisation (SEO)" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( ( Qt::TopToolBarArea | Qt::BottomToolBarArea ) );
  layout()->setSpacing ( 5 );

  QLabel* m_label = new QLabel ( i18n ( "SEO:" ), this );
  m_label->setToolTip ( i18n ( "Search Engine Optimisation (SEO)" ) );
  m_label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( m_label );

  m_lineEdit = new QLineEdit ( this );
  m_lineEdit->setObjectName ( QLatin1String ( "edit_search_line" ) );
  m_lineEdit->setMinimumWidth ( 100 );
  addWidget ( m_lineEdit );

  QAction *cb = addAction ( i18n ( "Clear" ) );
  cb->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );

  actionFind = addAction ( i18n ( "Keywords" ) );
  actionFind->setIcon ( KIcon ( QLatin1String ( "edit-find" ) ) );

  connect ( m_lineEdit, SIGNAL ( returnPressed() ), this, SLOT ( treating() ) );
  connect ( cb, SIGNAL ( triggered() ), m_lineEdit, SLOT ( clear() ) );
  connect ( actionFind, SIGNAL ( triggered() ), this, SLOT ( treating() ) );
}

/**
* Wenn eine Anfrage gesendet wird, an dieser Stelle
* das Historien Autovervollständigen aktualisieren.
*/
void KeywordsToolBar::modifyCompleterHistory ()
{
  completerHistory << m_lineEdit->text();
  completerHistory.removeDuplicates();
  m_lineEdit->setCompleter ( new QCompleter ( completerHistory, this ) );
}

/**
* Bei einem absenden zuerst an dieser Stelle
* den Inhalt von der Eingabe in eine Stringliste
* konvertieren und das Signal @ref changed senden.
* Ist der Inhalt leer wird auch gesendet weil die
* Suchfunktion von QWebKit damit die Hervorhebung
* wieder löscht. Gleichzeitig wird an dieser Stelle
* die Methode @ref modifyCompleterHistory aufgerufen.
*/
void KeywordsToolBar::treating()
{
  QString words = m_lineEdit->text();
  QStringList keywords;
  if ( words.isEmpty() )
  {
    emit changed ( keywords );
    return;
  }
  modifyCompleterHistory();

  if ( words.contains ( QRegExp ( "[ \\s\\t]+" ) ) )
    keywords = words.split ( QRegExp ( "[ \\s\\t]+" ) );
  else
    keywords << words;

  emit changed ( keywords );
}

KeywordsToolBar::~KeywordsToolBar()
{
  completerHistory.clear();
}
