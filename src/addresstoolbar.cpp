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

#include "addresstoolbar.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QCompleter>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QIcon>

AddressEdit::AddressEdit ( QToolBar * parent )
    : QLineEdit ( "http://", parent )
{
  setObjectName ( QLatin1String ( "addressedit" ) );
  setMinimumWidth ( 400 );
}

/**
* Setze den QCompleter für das LineEdit
*/
void AddressEdit::updateCompliter ( const QStringList &history )
{
  setCompleter ( new QCompleter ( history, this ) );
}

AddressEdit::~AddressEdit()
{}

AddressToolBar::AddressToolBar ( QWidget * parent )
    : QToolBar ( parent )
    , schemePattern ( QRegExp ( "^(http|file)" ) )
{
  setObjectName ( QLatin1String ( "addresstoolbar" ) );
  setWindowTitle ( trUtf8 ( "Address" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( Qt::TopToolBarArea | Qt::BottomToolBarArea );
  setMinimumWidth ( 350 );

  QIcon icon;

  goToIndex = addAction ( QLatin1String ( "Index" ) );
  goToIndex->setIcon ( icon.fromTheme ( QLatin1String ( "go-up" ) ) );
  goToIndex->setEnabled ( false );

  QLabel *label = new QLabel ( trUtf8 ( "Address:" ), this );
  label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( label );

  m_addressEdit = new AddressEdit ( this );
  addWidget ( m_addressEdit );

  QAction *cb = addAction ( trUtf8 ( "Clear" ) );
  cb->setIcon ( icon.fromTheme ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );

  QAction *go = addAction ( QLatin1String ( "Go" ) );
  go->setIcon ( icon.fromTheme ( QLatin1String ( "go-jump-locationbar" ) ) );

  connect ( m_addressEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( validatePath ( const QString & ) ) );
  connect ( m_addressEdit, SIGNAL ( returnPressed () ), this, SLOT ( checkInput () ) );
  connect ( goToIndex, SIGNAL ( triggered () ), this, SLOT ( urlToHostIndex () ) );
  connect ( cb, SIGNAL ( triggered() ), m_addressEdit, SLOT ( clear() ) );
  connect ( go, SIGNAL ( triggered() ), this, SLOT ( checkInput () ) );
}

/**
* Die Adresse ungeprüft in die Adresszeile einfügen.
*/
void AddressToolBar::setUrl ( const QUrl &url )
{
  m_addressEdit->setText ( url.toString() );
}

/**
* Dieser SLOT wird von Signal @ref HistoryManager::updateHistoryMenu aufgerufen.
*/
void AddressToolBar::updateHistoryItems ( const QList<HistoryItem> &items )
{
  if ( items.count() >= 1 )
  {
    QStringList history;
    foreach ( HistoryItem item, items )
    {
      history << item.url;
    }
    m_addressEdit->updateCompliter ( history );
  }
}

/**
* Prüfe ob die Angegebene Addresse einen Pfad enthält.
* Wenn nicht dann die Aktion goToIndex Deaktivieren.
*/
void AddressToolBar::validatePath ( const QString &address )
{
  QUrl url ( address );
  if ( !url.isValid() || url.isRelative() )
    return;

  if ( !url.scheme().contains ( schemePattern ) )
    return;

  goToIndex->setEnabled ( ( ( url.path().length() > 1 ) ? true : false ) );
}

/**
* Entferne Pfad,Prädikat und Anker von der Adresse und setze
* Die abgeschnitte Adresse wieder in @class AddressEdit ein.
* Danach wird @ref checkInput aufgerufen.
*/
void AddressToolBar::urlToHostIndex ()
{
  QUrl url ( m_addressEdit->text() );
  if ( !url.isValid() || url.isRelative() )
    return;

  if ( !url.scheme().contains ( schemePattern ) )
    return;

  QUrl::FormattingOptions flags = ( QUrl::RemovePath | QUrl::RemoveQuery | QUrl::RemoveFragment );
  m_addressEdit->setText ( url.toString ( flags ) );
  checkInput();
}

/**
* Überprüfen ob es sich um eine Valide Adresse handelt.
* Wenn ja wird das signal @ref urlChanged angestoßen.
*/
void AddressToolBar::checkInput ()
{
  QUrl url ( m_addressEdit->text() );
  if ( !url.isValid() || url.isRelative() )
    return;

  if ( url.scheme().contains ( "ftp" ) )
    emit sendMessage ( trUtf8 ( "Sorry: the ftp protocol is currently not supported" ) );

  if ( !url.scheme().contains ( schemePattern ) )
    return;

  emit urlChanged ( url );
}

AddressToolBar::~AddressToolBar()
{
}
