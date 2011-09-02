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

/* QtGui */
// #include <QtGui/QStringListModel>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

UrlLineEdit::UrlLineEdit ( QWidget * parent )
    : QLineEdit ( parent )
    , m_uriFilter ( KUriFilter::self() )
{
  setObjectName ( QLatin1String ( "UrlLineEdit" ) );
  setPlaceholderText ( "http://" );

  m_completerModel = new UrlCompleterModel ( this );
  QCompleter* m_completer = new QCompleter ( this );
  m_completer->setModel ( m_completerModel );
  m_completer->setCompletionPrefix ( placeholderText() );
  setCompleter ( m_completer );

  connect ( this, SIGNAL ( returnPressed() ), this, SLOT ( urlEntered() ) );
  connect ( this, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( validate ( const QString & ) ) );
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
* Ist abhängig von signal:textChanged und prüft die Eingabe
*/
void UrlLineEdit::validate ( const QString &inp )
{
  if ( ! checkInput ( inp ) )
    return;

//   qDebug() << Q_FUNC_INFO << inp;
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
* @ref placeholderText() setzen und den Cursor ans Ende setzen!
*/
void UrlLineEdit::focusInEvent ( QFocusEvent * event )
{
  if ( event->gotFocus() )
  {
    if ( text().isEmpty() )
      setText ( placeholderText() );
  }
  QLineEdit::focusInEvent ( event );
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
