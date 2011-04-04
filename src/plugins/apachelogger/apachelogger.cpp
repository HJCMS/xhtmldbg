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

#include "apachelogger.h"
#include "listmessages.h"
#include "loglistener.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

/* KDE */
#include <KDE/KFileDialog>
#include <KDE/KLocale>
#include <KDE/KIcon>

ApacheLogger::ApacheLogger ( QWidget * parent )
    : QDockWidget ( parent )
{
  setObjectName ( QLatin1String ( "ApacheLogger" ) );
  setWindowTitle ( QLatin1String ( "Logging" ) );

  QWidget* layer = new QWidget ( this );
  layer->setContentsMargins ( 1, 2, 1, 2 );

  QGridLayout* gridLayout = new QGridLayout ( layer );

  m_list = new ListMessages ( layer );
  gridLayout->addWidget ( m_list, 0, 0, 1, 3 );

  m_fileEdit = new  QLineEdit ( layer );
  m_fileEdit->setText ( QLatin1String ( "/var/log/apache/error_log" ) );
  gridLayout->addWidget ( m_fileEdit, 1, 0, 1, 1 );

  QToolButton* openbtn = new  QToolButton ( layer );
  openbtn->setIcon ( KIcon ( "document-open" ) );
  gridLayout->addWidget ( openbtn, 1, 1, 1, 1 );

  QToolButton* watchbtn = new  QToolButton ( layer );
  watchbtn->setIcon ( KIcon ( "view-refresh" ) );
  gridLayout->addWidget ( watchbtn, 1, 2, 1, 1 );

  layer->setLayout ( gridLayout );
  setWidget ( layer );

  m_listener = new LogListener ( this );

  connect ( m_fileEdit, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( createListener ( const QString & ) ) );

  connect ( openbtn, SIGNAL ( clicked() ),
            this, SLOT ( logOpenDialog() ) );

  connect ( watchbtn, SIGNAL ( clicked() ),
            m_listener, SLOT ( toggle() ) );

  connect ( m_listener, SIGNAL ( logChanged ( const QString & ) ),
            m_list, SLOT ( addLogMessage ( const QString & ) ) );

  createListener ( "dummy" );
}

void ApacheLogger::createListener ( const QString & )
{
  QFileInfo info ( m_fileEdit->text() );
  if ( ! info.exists() )
    return;

  m_listener->setLogfile ( info.absoluteFilePath() );
}

void ApacheLogger::logOpenDialog()
{
  QString f = KFileDialog::getOpenFileName ( KUrl ( "kfiledialog:///" ),
              QLatin1String ( "text/plain" ), this, i18n ( "Open Logfile" ) );
  if ( ! f.isEmpty() )
    m_fileEdit->setText ( f );
}

ApacheLogger::~ApacheLogger()
{
  m_list->clear();
  if ( m_listener )
  {
    m_listener->shutdown ();
    delete m_listener;
  }
}
