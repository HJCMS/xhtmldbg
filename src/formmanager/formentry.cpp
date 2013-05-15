/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#include "formentry.h"
#include "formconstructor.h"
#include "formtable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QMetaObject>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

FormEntry::FormEntry ( const FormConstructor &form, const QUrl &url, KTabWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "FormEntry" ) );
  QGridLayout* gridLayout = new QGridLayout ( this );

  m_table = new FormTable ( this );
  m_table->setTableContent ( form.elements );
  gridLayout->addWidget ( m_table, 0, 0, 1, 2 );

  QGroupBox* groupBox = new QGroupBox ( i18n ( "Bookmark" ), this );
  QGridLayout* boxLayout = new QGridLayout ( groupBox );

  boxLayout->addWidget ( new QLabel ( i18n ( "Project:" ), groupBox ), 0, 0, 1, 1, Qt::AlignRight );
  m_project = new QLineEdit ( this );
  boxLayout->addWidget ( m_project, 0, 1, 1, 1 );

  boxLayout->addWidget ( new QLabel ( i18n ( "Url:" ), groupBox ), 1, 0, 1, 1, Qt::AlignRight );
  m_url = new QLineEdit ( this );
  if ( url.isValid() )
    m_url->setText ( url.toString() );
  boxLayout->addWidget ( m_url, 1, 1, 1, 1 );

  boxLayout->addWidget ( new QLabel ( i18n ( "Form ID:" ), groupBox ), 2, 0, 1, 1, Qt::AlignRight );
  m_formId = new QLineEdit ( this );
  boxLayout->addWidget ( m_formId, 2, 1, 1, 1 );

  m_autoinsert = new QCheckBox ( i18n ( "automatically insert form data" ), groupBox );
  boxLayout->addWidget ( m_autoinsert, 3, 0, 1, 2 );
  groupBox->setLayout ( boxLayout );
  gridLayout->addWidget ( groupBox, 1, 0, 1, 2 );

  btn_remove = new QPushButton ( KIcon ( "list-remove" ), i18n ( "Remove" ), this );
  btn_remove->setEnabled ( false );
  gridLayout->addWidget ( btn_remove, 2, 0, 1, 1, Qt::AlignLeft );

  btn_insert = new QPushButton ( KIcon ( "bookmark-new" ), i18n ( "Insert" ), this );
  gridLayout->addWidget ( btn_insert, 2, 1, 1, 1, Qt::AlignRight );

  setLayout ( gridLayout );

  connect ( btn_remove, SIGNAL ( clicked() ), this, SLOT ( removeBookmark() ) );
  connect ( btn_insert, SIGNAL ( clicked() ), this, SLOT ( createBookmark() ) );
}

/** TODO Lesezeichen entfernen */
void FormEntry::removeBookmark()
{
}

/** Lesezeichen erstellen */
void FormEntry::createBookmark()
{
  if ( m_project->text().isEmpty() )
  {
    m_project->setFocus();
    return;
  }
  else if ( m_url->text().isEmpty() )
  {
    m_url->setFocus();
    return;
  }
  QUrl url ( m_url->text() );
  if ( ! url.isValid() )
  {
    m_url->setFocus();
    return;
  }
  FormBookmarkItem* item = new FormBookmarkItem ( url );
  item->setAutoinsert ( m_autoinsert->isChecked() );
  emit itemChanged ( m_project->text(), item );
}

void FormEntry::setAutoInsertEnabled ( bool b )
{
  m_autoinsert->setChecked ( b );
}

const QUrl FormEntry::urlRealm()
{
  QUrl url ( m_url->text() );
  return ( url.isValid() ? url : QUrl() );
}

void FormEntry::setUniqueId ( const QString &id )
{
  m_formId->setText ( id );
}

FormEntry::~FormEntry()
{
  if ( m_table )
    delete m_table;
}
