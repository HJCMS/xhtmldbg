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

#include "version.h"
#include "aboutdialog.h"

/* QtCore */
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QSizePolicy>

AboutDialog::AboutDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "aboutdialog" ) );
  QString winTitle = trUtf8 ( "About xhtmldbg (v%1) & HJCMS" ).arg( XHTMLDBG_VERSION_STRING );
  setWindowTitle ( winTitle );
  setMinimumWidth ( 450 );
  setMinimumHeight ( 350 );
  setSizeGripEnabled ( true );

  QGridLayout* layout = new QGridLayout ( this );

  QLabel* ImgLabel1 = new QLabel ( this );
  ImgLabel1->setTextInteractionFlags ( Qt::NoTextInteraction );
  ImgLabel1->setPixmap ( QPixmap( QString::fromUtf8 ( ":/icons/qtidy.png" ) ) );
  ImgLabel1->setLayoutDirection ( Qt::RightToLeft );
  ImgLabel1->setAlignment ( Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop );
  ImgLabel1->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Expanding );
  layout->addWidget ( ImgLabel1, 0, 0, 1, 1, Qt::AlignTop );

  QLabel* textLabel = new QLabel ( this );
  textLabel->setWordWrap ( true );
  textLabel->setOpenExternalLinks ( true );
  textLabel->setScaledContents ( true );
  textLabel->setTextInteractionFlags ( Qt::TextBrowserInteraction );
  textLabel->setMargin ( 5 );
  textLabel->setText ( aboutText() );
  textLabel->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
  layout->addWidget ( textLabel, 0, 1, 2, 1, Qt::AlignTop );

  QLabel* ImgLabel2 = new QLabel ( this );
  ImgLabel2->setTextInteractionFlags ( Qt::NoTextInteraction );
  ImgLabel2->setPixmap ( QPixmap( QString::fromUtf8 ( ":/icons/gplv3.png" ) ) );
  ImgLabel2->setLayoutDirection ( Qt::RightToLeft );
  ImgLabel2->setAlignment ( Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop );
  ImgLabel2->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Expanding );
  layout->addWidget ( ImgLabel2, 1, 0, 1, 1, Qt::AlignTop );

  QDialogButtonBox* box = new QDialogButtonBox ( QDialogButtonBox::Ok, Qt::Horizontal, this );
  layout->addWidget ( box, 3, 0, 1, 2, Qt::AlignHCenter );

  setLayout ( layout );

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
}

const QString AboutDialog::aboutText()
{
  return QString::fromUtf8 ( "<h3>The QTidy XHTML Debugger Application</h3><p>The goal of this Application is to Debug and Inspect Web-pages in One Application. You can Browse your Website Project and Debug it on the fly.</p>\
<dl><dt>Features:<dt><dd>Web Navigation with QWebKit</dd><dd>Source Validation for  real XHTML debugging with <a href=\"http://tidy.sourceforge.net\">Tidy</a>.</dd><dd>Dom Inspection with <a href=\"http://www.webkit.org\">Webkit's</a> Dom Inspector Tools</dd>\
<dt>Authors:</dt><dd>Heinemann Juergen (Undefined) <a href=\"http://www.hjcms.de\">HJCMS</a><dd>\
<dt>Licenses:</dt><dd>XHTMLDBG GPLv3</dd><dd>libQTidy LGPLv2</dd></dl>" );
}

AboutDialog::~AboutDialog()
{}
