/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#include "cssvalidator.h"
#include "validator.h"
#include "soupreader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>

CSSValidator::CSSValidator ( QWidget * parent, QSettings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
    , iconNotice ( QString::fromUtf8 ( ":/icons/notice.png" ) )
    , iconWarning ( QString::fromUtf8 ( ":/icons/warning.png" ) )
    , iconCritical ( QString::fromUtf8 ( ":/icons/critical.png" ) )
{
  setObjectName ( QLatin1String ( "cssvalidator" ) );
  setWindowTitle ( trUtf8 ( "CSS Validation" ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );

  m_soupReader = new SoupReader ( this );

  m_listWidget = new QListWidget ( this );
  m_listWidget->setSortingEnabled ( true );
  m_listWidget->setWordWrap ( false );
  m_listWidget->sortItems ( Qt::AscendingOrder );
  m_listWidget->setSelectionMode ( QAbstractItemView::SingleSelection );
  m_listWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
  m_listWidget->setContextMenuPolicy ( Qt::ActionsContextMenu );
  m_listWidget->setBackgroundRole ( QPalette::AlternateBase );
  m_listWidget->setAlternatingRowColors ( true );

  setWidget ( m_listWidget );

  m_validator = new Validator ( this );
  m_validator->setEnviromentVariable ( cfg );

  connect ( m_validator, SIGNAL ( error ( QProcess::ProcessError ) ),
            this, SLOT ( errors ( QProcess::ProcessError ) ) );

  connect ( m_validator, SIGNAL ( finished ( int, QProcess::ExitStatus ) ),
            this, SLOT ( exited ( int, QProcess::ExitStatus ) ) );

  connect ( m_validator, SIGNAL ( readyReadStandardOutput() ),
            this, SLOT ( readStandardReply() ) );

  connect ( m_validator, SIGNAL ( started() ),
            this, SLOT ( clearItems() ) );

  connect ( m_soupReader, SIGNAL ( parserError ( const QString & ) ),
            this, SLOT ( criticalItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( warnings ( const QString & ) ),
            this, SLOT ( warningItem ( const QString & ) ) );

  connect ( m_soupReader, SIGNAL ( congratulation ( const QString & ) ),
            this, SLOT ( noticeItem ( const QString & ) ) );
}

void CSSValidator::sortAscending()
{
  m_listWidget->sortItems ( Qt::AscendingOrder );
}

void CSSValidator::sortDescending()
{
  m_listWidget->sortItems ( Qt::DescendingOrder );
}

void CSSValidator::criticalItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconCritical, message ) );
}

void CSSValidator::warningItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconWarning, message ) );
}

void CSSValidator::noticeItem ( const QString &message )
{
  m_listWidget->addItem ( new QListWidgetItem ( iconNotice, message ) );
}

void CSSValidator::contextMenuEvent ( QContextMenuEvent *e )
{
  QMenu* menu = new QMenu ( this );

  QAction* ac_Ascending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-ascending" ) ),
                          trUtf8 ( "Ascending" ) );
  connect ( ac_Ascending, SIGNAL ( triggered() ), this, SLOT ( sortAscending() ) );

  QAction* ac_Descending = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "view-sort-descending" ) ),
                           trUtf8 ( "Descending" ) );
  connect ( ac_Descending, SIGNAL ( triggered() ), this, SLOT ( sortDescending() ) );

  QAction* ac_clear = menu->addAction ( QIcon::fromTheme ( QLatin1String ( "edit-clear" ) ),
                                        trUtf8 ( "Clear" ) );
  connect ( ac_clear, SIGNAL ( triggered() ), m_listWidget, SLOT ( clear() ) );

  menu->exec ( e->globalPos() );
}

void CSSValidator::runCssCheck ( const QUrl &url )
{
  if ( m_validator->isRunning() )
  {
    warningItem ( trUtf8 ( "Rejected - CSS Validator is busy ..." ) );
    return;
  }
  QString remote = url.toString ( ( QUrl::RemovePassword | QUrl::RemoveFragment ) );
  if ( ! remote.isEmpty() )
    m_validator->validate ( remote );
}

void CSSValidator::clearItems()
{
  m_listWidget->clear();
}

void CSSValidator::errors ( QProcess::ProcessError err )
{
  QString errtxt = m_validator->errorString();
  switch ( err )
  {
    case QProcess::FailedToStart:
      criticalItem ( trUtf8 ( "FailedToStart (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Crashed:
      criticalItem ( trUtf8 ( "Crashed (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::Timedout:
      warningItem ( trUtf8 ( "Timedout (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::WriteError:
      criticalItem ( trUtf8 ( "WriteError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::ReadError:
      criticalItem ( trUtf8 ( "ReadError (%1) ..." ).arg ( errtxt ) );
      break;

    case QProcess::UnknownError:
      criticalItem ( trUtf8 ( "UnknownError (%1) ..." ).arg ( errtxt ) );
      break;
  }
  m_validator->close();
}

void CSSValidator::exited ( int exitCode, QProcess::ExitStatus stat )
{
  Q_UNUSED ( exitCode )
  switch ( stat )
  {
    case QProcess::NormalExit:
      // noticeItem ( trUtf8 ( "CSS Validation finished." ) );
      m_validator->waitForFinished ( 50 );
      break;

    case QProcess::CrashExit:
      criticalItem ( trUtf8 ( "Crashed see logfiles" ) );
      break;

    default:
      return;
  }
  m_validator->close();
}

void CSSValidator::readInputData ( const QByteArray &data )
{
  QString soup ( data );
  if ( soup.isEmpty() || soup.contains ( QRegExp ( "^[\\s]?\\{.+\\}[\\s\\n\\r]" ) ) )
    return;

  if ( m_soupReader->readReceivedXML ( soup.trimmed() ) )
    m_validator->closeReadChannel ( QProcess::StandardOutput );
}

/**
* Der css-validator
*/
void CSSValidator::readStandardReply ()
{
  QByteArray data = m_validator->readAllStandardOutput();
  if ( data.startsWith ( QByteArray ( "{" ) ) || data.size() < 50 )
    return;

  readInputData ( data );
}

CSSValidator::~CSSValidator()
{
  if ( m_validator->isRunning() )
    m_validator->kill();
}
