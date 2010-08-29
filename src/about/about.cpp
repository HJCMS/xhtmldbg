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

#include "about.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QResource>
#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QSizePolicy>

About::About ( QWidget * parent )
    : QDialog ( parent )
{
  Q_INIT_RESOURCE ( about );
  setObjectName ( QLatin1String ( "about" ) );

  setupUi ( this );

  connect ( comboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( htmlChanged ( int ) ) );

  connect ( buttonBox, SIGNAL ( accepted () ),
            this, SLOT ( accept() ) );

  htmlChanged ( 0 );
}

void About::loadHtml ( const QUrl &index )
{
  QTextCodec* codec = QTextCodec::codecForName ( QByteArray ( "UTF-8" ) );
  QVariant buf = textBrowser->loadResource ( QTextDocument::HtmlResource, index );
  if ( buf.isValid() )
  {
    QString xhtml = codec->toUnicode ( buf.toByteArray() );
    textBrowser->setHtml ( xhtml );
  }
}

void About::htmlChanged ( int index )
{
  switch ( index )
  {
    case 0:
      loadHtml ( QUrl ( "qrc:/about.html" ) );
      break;

    case 1:
      loadHtml ( QUrl ( "qrc:/contact.html" ) );
      break;

    case 2:
      loadHtml ( QUrl ( "qrc:/author.html" ) );
      break;

    default:
      loadHtml ( QUrl ( "qrc:/about.html" ) );
      break;
  }
}

About::~About()
{
  Q_CLEANUP_RESOURCE ( about );
}
