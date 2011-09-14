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

#include "urllineedit.h"
#include "urlcompletermodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
// #include <QtGui/QStringListModel>
#include <QtGui/QAction>
#include <QtGui/QMenu>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

UrlLineEdit::UrlLineEdit ( QWidget * parent )
    : QLineEdit ( parent )
    , m_uriFilter ( KUriFilter::self() )
{
  setObjectName ( QLatin1String ( "UrlLineEdit" ) );
  setPlaceholderText ( "http://" );
  setFocusPolicy ( Qt::StrongFocus );
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  m_completerModel = new UrlCompleterModel ( this );
  QCompleter* m_completer = new QCompleter ( this );
  m_completer->setModel ( m_completerModel );
  m_completer->setCompletionPrefix ( placeholderText() );
  setCompleter ( m_completer );

  connect ( this, SIGNAL ( returnPressed() ), this, SLOT ( urlEntered() ) );
  connect ( this, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( inputValidate ( const QString & ) ) );
}

/**
* Erstellt die Filterliste und prüft ob das Plugin vorhanden ist!
*/
const QStringList UrlLineEdit::uriFilters()
{
  QStringList l;
  QStringList p = m_uriFilter->pluginNames();
  if ( p.contains ( QLatin1String ( "kshorturifilter" ) ) )
    l << QLatin1String ( "kshorturifilter" );

  if ( p.contains ( QLatin1String ( "localdomainfilter" ) ) )
    l << QLatin1String ( "localdomainfilter" );

  if ( p.contains ( QLatin1String ( "fixuphosturifilter" ) ) )
    l << QLatin1String ( "fixuphosturifilter" );

  return l;
}

/**
* Führt eine einfache Überprüfung der eingegebenen URL durch.
* Die Zeichenketten Kriterien sind:
* @li ist leer,
* @li ist länger als sie kleinste verfügbare TLD,
* @li ist gleich @ref placeholderText
* @li lässt sich mit @ref KUriFilter auflösen.
*/
bool UrlLineEdit::checkInput ( const QString &inp )
{
  if ( inp.isEmpty() || ( inp.length() <= 2 ) )
    return false;

  if ( inp == placeholderText() )
    return false;

  QString data ( inp );
  if ( ! m_uriFilter->filterUri ( data, uriFilters() ) )
    return false;

  return true;
}

/**
* Ist abhängig von Signal @em textChanged und prüft
* die Eingabe auf doppelten @ref placeholderText.
* Wenn ja wird dieser entfernt
*/
void UrlLineEdit::inputValidate ( const QString &inp )
{
  if ( ! checkInput ( inp ) )
    return;

  if ( inp.contains ( placeholderText() + placeholderText() ) )
  {
    setText ( QString ( inp ).remove ( QRegExp ( "^" + placeholderText(), Qt::CaseSensitive ) ) );
  }
  else if ( inp.contains ( placeholderText() + "https://" ) )
  {
    setText ( QString ( inp ).remove ( QRegExp ( "^" + placeholderText(), Qt::CaseSensitive ) ) );
  }
}

/**
* Eine @em Neueingabe starten
* @li Bereinigt die Eingabe
* @li Setzt den @ref placeholderText
* @li Zum schluss den Maus Cursor mit einem @ref setFocus setzen.
*/
void UrlLineEdit::startAutoEdit ()
{
  clear();
  setText ( placeholderText() );
  setFocus();
}

/**
* Dieser Slot übernimmt die Verarbeitung für die eingegebenen URL und einem Enter.
* Wird diese Erfolgreich Aufgelöst geht \e Signal: \b urlChanged raus.
*/
void UrlLineEdit::urlEntered ()
{
  if ( ! checkInput ( text() ) )
    return;

  KUrl pUrl = m_uriFilter->filteredUri ( KUrl ( text() ), uriFilters() );
  if ( pUrl.isValid() )
  {
    if ( pUrl.protocol().isEmpty() )
      pUrl.setScheme ( QLatin1String ( "http" ) );

    m_completerModel->addItem ( pUrl.url() );
    emit urlChanged ( pUrl );
  }
}

/**
* Wenn text() Leer ist dann bei einem onFocus den
* @ref placeholderText setzen und den Cursor ans Ende setzen!
*/
void UrlLineEdit::focusInEvent ( QFocusEvent * event )
{
  qDebug() << Q_FUNC_INFO << event->reason();
  if ( event->reason() == Qt::MouseFocusReason )
  {
    if ( text().isEmpty() )
      setText ( placeholderText() );
  }
  QLineEdit::focusInEvent ( event );
}

/**
* Füge zwei neue Optionen in das Kontext Menü ein.
* @li Komplettes Eingabefeld Leeren
* @li Addressn Eingabe mit Hilfe von @ref startAutoEdit
*/
void UrlLineEdit::contextMenuEvent ( QContextMenuEvent * event )
{
  QMenu* m = createStandardContextMenu();

  QAction* m_clearAction = m->addAction ( KIcon ( "edit-clear" ), i18n ( "Clear" ) );
  connect ( m_clearAction, SIGNAL ( triggered () ), this, SLOT ( clear() ) );

  QAction* m_editAction = m->addAction ( KIcon ( "edit-clear-locationbar-rtl" ), i18n ( "Start Edit" ) );
  connect ( m_editAction, SIGNAL ( triggered () ), this, SLOT ( startAutoEdit() ) );

  m->exec ( event->globalPos() );
  delete m;
}

/**
* Standard Methode zum setzen einer URL
*/
void UrlLineEdit::setUrl ( const KUrl &url )
{
  if ( url.isValid() )
  {
    m_completerModel->addItem ( url.url() );
    setText ( url.url() );
  }
}

/**
* Liest die Aktuelle URL von lineEdit
*/
const KUrl UrlLineEdit::url()
{
  KUrl u ( text() );
  if ( u.protocol().isEmpty() )
    u.setScheme ( QLatin1String ( "http" ) );

  return u;
}

UrlLineEdit::~UrlLineEdit()
{}
