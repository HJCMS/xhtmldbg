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

#include "addresstoolbar.h"
#include "historyitem.h"

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QCompleter>
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QIcon>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

/** @class AddressEdit */
AddressEdit::AddressEdit ( QToolBar * parent )
    : QLineEdit ( "http://", parent )
    , defColor ( palette().text().color() )
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

void AddressEdit::setColor ( BGCOL mode )
{
  QColor warnColor ( Qt::darkRed );

  switch ( mode )
  {
    case NORMAL:
      setStyleSheet ( QString ( "QLineEdit { color: %1; }" ).arg ( defColor.name() ) );
      break;

    case WARN:
      setStyleSheet ( QString ( "QLineEdit { color: %1; }" ).arg ( warnColor.name() ) );
      break;

    case DANGER:
      setStyleSheet ( QString ( "QLineEdit { color: %1; }" ).arg ( warnColor.name() ) );
      break;

    default:
      setStyleSheet ( QString ( "QLineEdit { color: %1; }" ).arg ( defColor.name() ) );
      break;
  };
}

AddressEdit::~AddressEdit()
{}


/** @class AddressToolBar */
AddressToolBar::AddressToolBar ( QWidget * parent )
    : QToolBar ( parent )
    , schemePattern ( QRegExp ( "^(http[s]?|file)" ) )
{
  setObjectName ( QLatin1String ( "addresstoolbar" ) );
  setWindowTitle ( i18n ( "Address" ) );
  setOrientation ( Qt::Horizontal );
  setAllowedAreas ( Qt::TopToolBarArea | Qt::BottomToolBarArea );
  setMinimumWidth ( 350 );

  ac_goToIndex = addAction ( QLatin1String ( "Index" ) );
  ac_goToIndex->setIcon ( KIcon ( QLatin1String ( "go-up" ) ) );
  ac_goToIndex->setEnabled ( false );

  ac_reload = addAction ( i18n ( "Reload" ) );
  ac_reload->setIcon ( KIcon ( QLatin1String ( "view-refresh" ) ) );

  QLabel *label = new QLabel ( i18n ( "Address:" ), this );
  label->setContentsMargins ( 5, 0, 5, 0 );
  addWidget ( label );

  m_addressEdit = new AddressEdit ( this );
  addWidget ( m_addressEdit );

  QAction *cb = addAction ( i18n ( "Clear" ) );
  cb->setIcon ( KIcon ( QLatin1String ( "edit-clear-locationbar-rtl" ) ) );

  QAction *go = addAction ( QLatin1String ( "Go" ) );
  go->setIcon ( KIcon ( QLatin1String ( "go-jump-locationbar" ) ) );

  connect ( m_addressEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( validatePath ( const QString & ) ) );
  connect ( m_addressEdit, SIGNAL ( returnPressed () ), this, SLOT ( checkInput() ) );
  connect ( ac_goToIndex, SIGNAL ( triggered () ), this, SLOT ( urlToHostIndex() ) );
  connect ( ac_reload, SIGNAL ( triggered () ), this, SLOT ( urlReloadPage() ) );
  connect ( cb, SIGNAL ( triggered() ), m_addressEdit, SLOT ( clear() ) );
  connect ( go, SIGNAL ( triggered() ), this, SLOT ( checkInput() ) );
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
* Wenn nicht dann die Aktion ac_goToIndex Deaktivieren.
*/
void AddressToolBar::validatePath ( const QString &address )
{
  QUrl url ( address );
  if ( !url.isValid() )
    return;

  if ( url.isRelative() )
    url.setScheme ( "file" );

  if ( !url.scheme().contains ( schemePattern ) )
    return;

  AddressEdit::BGCOL col = url.scheme().contains ( "https" ) ? AddressEdit::WARN : AddressEdit::NORMAL;
  m_addressEdit->setColor ( col );
  ac_goToIndex->setEnabled ( ( ( url.path().length() > 1 ) ? true : false ) );
}

/**
* Entferne Pfad,Prädikat und Anker von der Adresse und setze
* Die abgeschnitte Adresse wieder in @class AddressEdit ein.
* Danach wird @ref checkInput aufgerufen.
*/
void AddressToolBar::urlToHostIndex()
{
  QUrl url ( m_addressEdit->text() );
  if ( !url.isValid() || url.isRelative() )
    return;

  if ( ! url.scheme().contains ( "http" ) )
    return;

  QUrl::FormattingOptions flags = ( QUrl::RemovePath | QUrl::RemoveQuery | QUrl::RemoveFragment );
  m_addressEdit->setText ( url.toString ( flags ) );
  checkInput();
}

/**
* Die Aktuelle Seiten URL neu Laden!
*/
void AddressToolBar::urlReloadPage()
{
  QUrl url ( m_addressEdit->text() );
  if ( !url.isValid() )
    return;

  if ( url.isRelative() )
    url.setScheme ( "file" );

  if ( url.scheme().contains ( schemePattern ) )
    emit reloadUrl ( url );
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
    emit sendMessage ( i18n ( "Sorry: FTP protocol is currently not supported" ) );

  if ( url.scheme().contains ( schemePattern ) )
    emit urlChanged ( url );
}

AddressToolBar::~AddressToolBar()
{}
