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

#include "rfcbrowser.h"
#include "selectermodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QSize>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QStandardItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

/* QtXml */
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

RFCBrowser::RFCBrowser ( QObject * parent )
    : QObject ( parent )
{
  Q_INIT_RESOURCE ( rfcbrowser );

  setObjectName ( QLatin1String ( "rfcbrowser" ) );

  dialog = new QDialog;
  dialog->setMinimumWidth ( 480 );
  dialog->setObjectName ( QLatin1String ( "rfcbrowserdialog" ) );
  dialog->setSizeGripEnabled ( true );
  dialog->setWindowTitle ( i18n ( "RFC Document Browser" ) );

  QVBoxLayout* vLayout = new QVBoxLayout ( dialog );
  dialog->setLayout ( vLayout );

  rfcselecter = new QComboBox ( dialog );
  rfcselecter->setObjectName ( QLatin1String ( "rfcselecter" ) );
  model =  new SelecterModel ( rfcselecter );
  model->setItem ( 0, new QStandardItem ( i18n ( "Change RFC Document" ) ) );
  model->item ( 0, 0 )->setEditable ( false );
  model->item ( 0, 0 )->setSelectable ( false );
  rfcselecter->setModel ( model );

  vLayout->addWidget ( rfcselecter );

  browser = new QTextEdit ( dialog );
  browser->setObjectName ( QLatin1String ( "textbrowser" ) );
  browser->setReadOnly ( true );
  browser->setAcceptRichText ( false );
  browser->setLineWrapMode ( QTextEdit::NoWrap );
  browser->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  vLayout->addWidget ( browser );

  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->setObjectName ( QLatin1String ( "horzontallayout" ) );

  search = new QLineEdit ( dialog );
  search->setObjectName ( QLatin1String ( "textsearch" ) );
  hLayout->addWidget ( search );

  QToolButton* next = new QToolButton ( dialog );
  next->setObjectName ( QLatin1String ( "button" ) );
  next->setIcon ( QIcon::fromTheme ( QLatin1String ( "edit-find" ) ) );
  hLayout->addWidget ( next );

  vLayout->addLayout ( hLayout );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, dialog );
  box->setStandardButtons ( QDialogButtonBox::Close );
  box->setCenterButtons ( true );
  vLayout->addWidget ( box );

  // D-Bus Registrierung
  QDBusConnection::sessionBus().registerObject ( "/Plugin/RFCBrowser", this,
          QDBusConnection::ExportScriptableContents );

  connect ( rfcselecter, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( currentIndexChanged ( int ) ) );
  connect ( search, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( find ( const QString & ) ) );
  connect ( next, SIGNAL ( clicked() ), this, SLOT ( find () ) );
  connect ( box, SIGNAL ( rejected() ), dialog, SLOT ( reject() ) );
}

/**
* Die RFC Auswahl mit dem XML befüllen.
*/
void RFCBrowser::fillDocumentSelecter()
{
  QDomDocument dom;
  QFile fp ( QString::fromUtf8 ( ":/rfc.xml" ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    if ( ! dom.setContent ( &fp, true ) )
    {
      qWarning ( "Can not open rfc.xml" );
      return;
    }
    QDomNodeList nodes = dom.documentElement().elementsByTagName ( QLatin1String ( "rfc" ) );
    for ( int i = 0; i < nodes.size(); i++ )
    {
      QDomElement rfc = nodes.item ( i ).toElement();
      QStandardItem* item = new QStandardItem ();
      item->setEditable ( false );
      for ( QDomNode n = rfc.firstChild(); !n.isNull(); n = n.nextSibling() )
      {
        // qDebug() << Q_FUNC_INFO << n.nodeName();
        if ( n.nodeName() == "name" )
          item->setText ( n.firstChild().nodeValue() );
        else if ( n.nodeName() == "title" )
          item->setData ( n.firstChild().nodeValue(), Qt::ToolTipRole );
        else if ( n.nodeName() == "filename" )
          item->setData ( n.firstChild().nodeValue(), Qt::UserRole );
        else
          continue;
      }
      model->appendRow ( item );
    }
    fp.close();
  }
}

/**
* Öffnen mit dem Übergebenen Dateinamen das RFC Dokument
* und schreibe in das QTextEdit
*/
void RFCBrowser::setDocument ( const QString &fileName )
{
  dialog->setCursor ( Qt::WaitCursor );
  QString buffer;
  QFile fp ( QString::fromUtf8 ( ":/rfc/%1" ).arg ( fileName ) );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QTextStream stream ( &fp );
    buffer = stream.readAll();
    fp.close();
  }
  QString doc = buffer.trimmed();
  // QTextCodec* codec = QTextCodec::codecForName ( "UTF-8" );
  browser->setPlainText ( doc );
  dialog->setCursor ( Qt::ArrowCursor );
}

/**
* Bei einem auswählen der ComboBox den DateiNamen
* für das RFC Dokument ermitteln und @ref setDocument
* aufrufen.
*/
void RFCBrowser::currentIndexChanged ( int index )
{
  if ( index > 0 )
  {
    QStandardItem* item = model->item ( index, 0 );
    setDocument ( item->data ( Qt::UserRole ).toString() );
  }
}

/**
* Verbesserte Suchhilfe
*/
void RFCBrowser::find ( const QString &word )
{
  Q_UNUSED ( word )
  browser->find ( search->text(), QTextDocument::FindWholeWords );
}

/**
* Schreibe die Inhalte für die Auswahl und
* starte den Dialog.
*/
void RFCBrowser::open()
{
  fillDocumentSelecter();
  dialog->exec();
}

RFCBrowser::~RFCBrowser()
{}
