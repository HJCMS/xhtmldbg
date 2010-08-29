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
#include <QtCore/QDebug>
#include <QtCore/QResource>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

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

void About::loadHtml ( const QString &index )
{
  QVariant data;
  data = textBrowser->loadResource ( QTextDocument::HtmlResource, index );
  if ( data.isValid() )
  {
    textBrowser->setHtml ( data.toString() );
  }
}

void About::htmlChanged ( int index )
{
  switch ( index )
  {
    case 0:
      loadHtml ( QLatin1String( "qrc:/about.html" ) );
      break;

    case 1:
      loadHtml ( QLatin1String( "qrc:/author.html" ) );
      break;

    default:
      loadHtml ( QLatin1String( "qrc:/about.html" ) );
      break;
  }
}

About::~About()
{
  Q_CLEANUP_RESOURCE ( about );
}
