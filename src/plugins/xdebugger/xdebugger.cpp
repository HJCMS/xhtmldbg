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

#include "xdebugger.h"
#include "xdebugsocket.h"
#include "xdebugmenubar.h"
#include "xdebugprojectdialog.h"
#include "xdebugprojectitem.h"
#include "xdebugreader.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QHashIterator>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QToolButton>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>

/* QtNetwork */
#include <QtNetwork/QTcpSocket>

// http://xdebug.org/docs-dbgp.php
XDebugger::XDebugger ( QWidget * parent, QSettings * settings )
    : QDockWidget ( parent )
    , cfg ( settings )
{
  setObjectName ( QLatin1String ( "xdebugger" ) );
  setWindowTitle ( trUtf8 ( "Xdebug" ) );
  setAllowedAreas ( ( allowedAreas() & ~Qt::TopDockWidgetArea ) );
  setFeatures ( ( features() & ~QDockWidget::DockWidgetFloatable ) );
  setContentsMargins ( 0, 4, 0, 4 );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  QWidget* layerWidget = new QWidget ( this );
  QGridLayout* gLayout = new QGridLayout ( layerWidget );
  gLayout->setContentsMargins ( 0, 0, 0, 0 );
  gLayout->setSpacing ( 5 );

  // begin:Project
  QLabel* txt0 = new QLabel ( layerWidget );
  txt0->setText ( trUtf8 ( "Project:" ) );
  txt0->setAlignment ( labelAlignment );
  gLayout->addWidget ( txt0, 0, 0, 1, 1 );

  selectProject = new QComboBox ( layerWidget );
  selectProject->setObjectName ( QLatin1String ( "selectproject" ) );
  selectProject->setWhatsThis ( trUtf8 ( "select your generated projects" ) );
  gLayout->addWidget ( selectProject, 0, 1, 1, 1 );

  QToolButton* openProjectButton = new QToolButton ( layerWidget );
  openProjectButton->setIcon ( QIcon::fromTheme ( QLatin1String ( "project-open" ) ) );
  openProjectButton->setToolTip ( trUtf8 ( "Project selection" ) );
  openProjectButton->setWhatsThis ( trUtf8 ( "this button open the project editor dialog" ) );
  gLayout->addWidget ( openProjectButton, 0, 2, 1, 1 );
  // end:Project

  // begin:Port
  QLabel* txt1 = new QLabel ( layerWidget );
  txt1->setText ( trUtf8 ( "Port:" ) );
  txt1->setAlignment ( labelAlignment );
  gLayout->addWidget ( txt1, 1, 0, 1, 1 );

  spinPortBox = new QSpinBox ( layerWidget );
  spinPortBox->setObjectName ( QLatin1String ( "portnumber" ) );
  spinPortBox->setRange ( 1024, 65535 );
  spinPortBox->setValue ( 9000 );
  spinPortBox->setToolTip ( trUtf8 ( "Remote Port: (DBGP_PORT)" ) );
  spinPortBox->setWhatsThis ( trUtf8 ( "The port to which Xdebug tries to connect on the remote host.\nPort 9000 is the default for both the client and the bundled debugclient.\nAs many clients use this port number, it is best to leave this setting unchanged." ) );
  gLayout->addWidget ( spinPortBox, 1, 1, 1, 2 );
  // end:Port

  // begin:Session
  // XDEBUG_CONFIG="idekey=session_name"
  QLabel* txt2 = new QLabel ( layerWidget );
  txt2->setText ( trUtf8 ( "Session:" ) );
  txt2->setAlignment ( labelAlignment );
  gLayout->addWidget ( txt2, 2, 0, 1, 1 );

  sessionIdEdit = new QLineEdit ( layerWidget );
  sessionIdEdit->setObjectName ( QLatin1String ( "sessionkey" ) );
  sessionIdEdit->setToolTip ( QLatin1String ( "Session Key: $_COOKIE['XDEBUG_SESSION']['idekey']" ) );
  sessionIdEdit->setWhatsThis ( trUtf8 ( "a IDE key, which the debugger engine will also use in it's debugging init command." ) );
  gLayout->addWidget ( sessionIdEdit, 2, 1, 1, 2 );
  // end:Session

  // begin:hostIP
  QLabel* txt3 = new QLabel ( layerWidget );
  txt3->setText ( trUtf8 ( "Host:" ) );
  txt3->setAlignment ( labelAlignment );
  gLayout->addWidget ( txt3, 3, 0, 1, 1 );

  remoteHostEdit = new QLineEdit ( layerWidget );
  remoteHostEdit->setObjectName ( QLatin1String ( "remotehostedit" ) );
  remoteHostEdit->setToolTip ( QLatin1String ( "Remote Host: $_SERVER['HTTP_HOST']" ) );
  remoteHostEdit->setWhatsThis ( trUtf8 ( "Selects the host where the debug client is running, you can either use a host name or an IP address." ) );
  gLayout->addWidget ( remoteHostEdit, 3, 1, 1, 2 );
  // end:hostIP

  m_Messanger = new QTextEdit ( layerWidget );
  m_Messanger->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  m_Messanger->setObjectName ( QLatin1String ( "messages" ) );
  gLayout->addWidget ( m_Messanger, 4, 0, 1, 3 );

  // Debugger Klient
  m_debuggerBar = new XDebugMenuBar ( layerWidget );
  gLayout->addWidget ( m_debuggerBar, 5, 0, 1, 3 );

  // finalize layout
  layerWidget->setLayout ( gLayout );
  setWidget ( layerWidget );

  // Inhalte laden
  loadProjectsIntoComboBox();

  // Setze die Debugger Umgebung
  m_debugSocket = new XDebugSocket ( this );

  // SIGNALS
  connect ( selectProject, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( findProjectItems ( int ) ) );

  connect ( openProjectButton, SIGNAL ( clicked () ),
            this, SLOT ( editProjects () ) );

  connect ( m_debugSocket, SIGNAL ( statusMessage ( const QString & ) ),
            this, SLOT ( appendMessage ( const QString & ) ) );

  connect ( m_debugSocket, SIGNAL ( errorMessage ( const QString & ) ),
            this, SLOT ( setMessage ( const QString & ) ) );

  connect ( m_debugSocket, SIGNAL ( dataChanged ( const QString & ) ),
            this, SLOT ( setReply ( const QString & ) ) );

  connect ( m_debugSocket, SIGNAL ( connected ( bool ) ),
            m_debuggerBar, SLOT ( setListenerLed ( bool ) ) );

  connect ( m_debuggerBar, SIGNAL ( startServer() ),
            this, SLOT ( openXDebugListener() ) );

  connect ( m_debuggerBar, SIGNAL ( shutdownServer() ),
            this, SLOT ( closeXDebugListener() ) );

  connect ( m_debuggerBar, SIGNAL ( sendCommand() ),
            this, SLOT ( sendCommandLine() ) );
}

/**
* Liest die Projekt XML ein und setzt Name und SessionID in @ref selectProject.
*/
void XDebugger::loadProjectsIntoComboBox()
{
  QString errorMsg;
  QDomDocument dom;

  QString db = XDebugProjectDialog::dbProjectPath();
  if ( db.isEmpty() )
    return;

  QFile fp ( db );
  if ( ! dom.setContent ( &fp, &errorMsg ) )
  {
#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) XDebugger XML Error:\n%s", qPrintable ( errorMsg ) );
#endif
    return;
  }

  selectProject->clear();
  selectProject->addItem ( trUtf8 ( "Select Project" ), QString() );
  QDomNodeList nodes = dom.elementsByTagName ( QLatin1String ( "project" ) );
  for ( int i = 0; i < nodes.size(); i++ )
  {
    if ( nodes.item ( i ).hasChildNodes() )
    {
      selectProject->addItem (
          nodes.item ( i ).firstChildElement ( QLatin1String ( "name" ) ).firstChild().nodeValue(),
          nodes.item ( i ).firstChildElement ( QLatin1String ( "sessionid" ) ).firstChild().nodeValue()
      );
    }
  }

  if ( fp.isOpen() )
    fp.close();
}

/**
* Sucht bei einer @ref selectProject Auswahl mit Name und SessionID
* in der Projekt XML nach einem Eintrag und befüllt die Datenfelder.
*/
void XDebugger::findProjectItems ( int index )
{
  QString name = selectProject->itemText ( index );
  QString session = selectProject->itemData ( index, Qt::UserRole ).toString();
  if ( session.isEmpty() )
  {
    sessionIdEdit->clear();
    sessionIdEdit->clearFocus();
    remoteHostEdit->clear();
    remoteHostEdit->clearFocus();
    return;
  }

  QString errorMsg;
  QDomDocument dom;
  QString db = XDebugProjectDialog::dbProjectPath();
  if ( db.isEmpty() )
    return;

  QFile fp ( db );
  if ( ! dom.setContent ( &fp, &errorMsg ) )
  {
#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) XDebugger XML Error:\n%s", qPrintable ( errorMsg ) );
#endif
    return;
  }

  QDomNodeList nodes = dom.elementsByTagName ( QLatin1String ( "project" ) );
  for ( int i = 0; i < nodes.size(); i++ )
  {
    if ( nodes.item ( i ).hasChildNodes() )
    {
      if ( ( nodes.item ( i ).firstChildElement ( QLatin1String ( "name" ) ).firstChild().nodeValue() == name )
              && ( nodes.item ( i ).firstChildElement ( QLatin1String ( "sessionid" ) ).firstChild().nodeValue() == session ) )
      {
        sessionIdEdit->setText ( session );
        setSpinBox ( nodes.item ( i ).firstChildElement ( QLatin1String ( "port" ) ).firstChild().nodeValue() );
        remoteHostEdit->setText ( nodes.item ( i ).firstChildElement ( QLatin1String ( "remotehost" ) ).firstChild().nodeValue() );
        break;
      }
    }
  }

  if ( fp.isOpen() )
    fp.close();
}

/**
* Private Methode zum öffnen von @ref ProjectDialog
*/
void XDebugger::openProjectEditDialog()
{
  XDebugProjectDialog* dialog = new XDebugProjectDialog ( this );
  dialog->exec();
}

/**
* SLOT für das öffnen des Projekt Editieren
* Dialoges @ref openProjectEditDialog
*/
void XDebugger::editProjects()
{
  openProjectEditDialog();
}

void XDebugger::setSpinBox ( const QVariant &p )
{
  spinPortBox->setValue ( p.toUInt() );
}

/**
* Startet den Server an dem gehorcht wird
**/
void XDebugger::openXDebugListener ()
{
  XDebugProjectItem* proj = new XDebugProjectItem;
  proj->setPort ( QString::number ( spinPortBox->value() ) );
  proj->setName ( selectProject->currentText() );
  proj->setSessionId ( sessionIdEdit->text() );
  proj->setRemoteHost ( remoteHostEdit->text() );
  m_debugSocket->start ( proj );
}

void XDebugger::closeXDebugListener()
{
  if ( m_debugSocket )
    m_debugSocket->stop();
}

/**
* Nachrichten Anfügen
*/
void XDebugger::setMessage ( const QString &m )
{
  m_Messanger->setPlainText ( m );
}

/**
* TEST
*/
void XDebugger::sendCommandLine()
{
  QString cmd = QString ( "typemap_get -i %1 -- " ).arg( sessionIdEdit->text() );
  QByteArray arg = cmd.toAscii();
  m_debugSocket->sendMessage ( arg + arg.toBase64() );
}

/**
* Nachrichten Anzeigen.
*/
void XDebugger::appendMessage ( const QString &m )
{
  if ( m.isEmpty() )
    return;

  QString buff = m_Messanger->toPlainText();
  if ( ! buff.isEmpty() )
    buff.append ( "\n" );
  buff.append ( m );

  m_Messanger->setPlainText ( buff );
}

/**
* TODO XML Antwort Verarbeiten
*/
void XDebugger::setReply ( const QString &xml )
{
  if ( xml.isEmpty() )
    return;

  QStringList attributes;
  XDebugReader dom ( xml );
  QHashIterator<QString,QString> it = dom.attributeList ( dom.documentElement() );

#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG_VERBOSE
  qWarning ( "(XHTMLDBG) XDebugReader:\n%s", qPrintable ( dom.toString() ) );
#endif

  while ( it.hasNext() )
  {
    it.next();
    attributes << QString ( "%1 : %2" ).arg ( it.key(), it.value() );
  }
  m_Messanger->setPlainText ( attributes.join ( "\n" ) );
}

/**
* Wird von außerhalb aufgerufen und soll mit der SessionID
* bei @ref selectProject einen Eintrag finden.
* Wenn gefunden dann den ComboBox eintrag auswählen.
* Siehe auch @ref findProjectItems das hierdurch ausgelöst wird.
*/
void XDebugger::openSessionWithIdKey ( const QString &idekey )
{
  int index = selectProject->findData ( idekey, Qt::UserRole, Qt::MatchExactly );
  if ( index >= 0 )
    selectProject->setCurrentIndex ( index );
}

XDebugger::~XDebugger()
{}
