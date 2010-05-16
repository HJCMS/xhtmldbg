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

#include "xdebugprojectdialog.h"
#include "xdebugprojectitem.h"
#include "xdebugprojecttable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
// #include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomProcessingInstruction>

XDebugProjectDialog::XDebugProjectDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "projectdialog" ) );
  setWindowTitle ( trUtf8 ( "Configure xhtmldbg XDebug Projects[*]" ) );
  setMinimumWidth ( 450 );
  setMinimumHeight ( 200 );
  setSizeGripEnabled ( true );
  setContentsMargins ( 5, 5, 5, 10 );
  setWindowModified ( false );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setContentsMargins ( 0, 0, 0, 0 );

  m_table = new XDebugProjectTable ( this );
  vLayout->addWidget ( m_table );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal, this );
  box->setStandardButtons ( ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ) );
  QPushButton* saveBtn = box->addButton ( trUtf8 ( "&Save" ), QDialogButtonBox::ActionRole );
  saveBtn->setIcon ( QIcon::fromTheme ( QLatin1String ( "document-save" ) ) );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  // SIGNALS
  connect ( saveBtn, SIGNAL ( clicked() ), this, SLOT ( save() ) );
  connect ( box, SIGNAL ( accepted() ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected() ), this, SLOT ( reject() ) );

  connect ( m_table, SIGNAL ( modified ( bool ) ),
            this, SLOT ( setWindowModified ( bool ) ) );

  load();
}

/**
* Such nach der Projekt Datenbank Datei xdebug.xml
* Wenn der Pfad noch nicht vorhanden ist oder die
* Datei noch nicht existiert wird sie angelegt!
*/
const QString XDebugProjectDialog::dbProjectPath()
{
  QString xml;
  QString p = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  QDir d ( p );
  if ( ! d.exists() )
    d.mkpath ( p );

  foreach ( QString f, d.entryList ( QDir::Files, QDir::Name ) )
  {
    if ( f == QString::fromUtf8 ( "xdebug.xml" ) )
      return d.absoluteFilePath ( f );
  }

  // Wenn Noch nicht vorhanden dann anlegen!
  xml = d.absoluteFilePath ( QLatin1String ( "xdebug.xml" ) );
  QFile fp ( xml );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << QString::fromUtf8 ( "<?xml version = '1.0' encoding = 'utf8' ?><xdebug projects=\"0\" />\n" );
    fp.close();
  }
  else
    qWarning ( "Permission Denied for generate xdebug.xml" );

  return xml;
}

/**
* Liest alle Vorhanden Projekte ein!
* @code
<?xml version='1.0' encoding='utf8'?>
<xdebug version="0">
  <project>
    <name>xhtmldbg</name>
    <port>9000</port>
    <remotehost>xhtmldbg.hjcms.jh</remotehost>
    <sessionid>heinemann</sessionid>
  </project>
</xdebug>
* @endcode
*/
void XDebugProjectDialog::load()
{
  QString db = dbProjectPath();
  if ( db.isEmpty() )
    return;

  QString errorMsg;
  QDomDocument dom;
  QFile fp ( db );
  if ( ! dom.setContent ( &fp, &errorMsg ) )
  {
#if defined Q_OS_LINUX && defined XHTMLDBG_DEBUG
    qWarning ( "(XHTMLDBG) Xdebug Project Reader Error:\n%s", qPrintable ( errorMsg ) );
#endif
    return;
  }

  QDomNodeList nodes = dom.elementsByTagName ( QLatin1String ( "project" ) );
  for ( int i = 0; i < nodes.size(); i++ )
  {
    if ( nodes.item ( i ).hasChildNodes() )
    {
      QDomNode node = nodes.item ( i );
      XDebugProjectItem* prj = new XDebugProjectItem;
      prj->setPort ( node.firstChildElement ( QLatin1String ( "port" ) ).firstChild().nodeValue() );
      prj->setName ( node.firstChildElement ( QLatin1String ( "name" ) ).firstChild().nodeValue() );
      prj->setRemoteHost ( node.firstChildElement ( QLatin1String ( "remotehost" ) ).firstChild().nodeValue() );
      prj->setSessionId ( node.firstChildElement ( QLatin1String ( "sessionid" ) ).firstChild().nodeValue() );
      m_table->addProject ( prj );
    }
  }
  if ( fp.isOpen() )
    fp.close();

  setWindowModified ( false );
}

/**
* Erstellt eine neue XML Datenbank Projektdatei aus dem derzeitigen Inhalt
* von @ref ProjectTableModel. Dabei wird das alte XML komplett Überschrieben.
*/
void XDebugProjectDialog::save()
{
  QString db = dbProjectPath();
  if ( db.isEmpty() )
    return;

  QDomDocument dom;
  QDomProcessingInstruction pi = dom.createProcessingInstruction ( "xml",
                                 "version = '1.0' encoding = 'utf-8'" );
  dom.appendChild ( pi );
  dom.appendChild ( dom.createComment ( QLatin1String ( " XHTMLDBG Project file for Xdebug " ) ) );

  QDomElement rootNode = dom.createElement ( "xdebug" );
  rootNode.setAttribute ( "version", "1.0" );
  dom.appendChild ( rootNode );

  for ( int row = 0; row < m_table->rowCount(); row++ )
  {
    QDomElement project = dom.createElement ( "project" );
    rootNode.appendChild ( project );

    XDebugProjectItem* items = m_table->rowProjectItem ( row );

    // Projektname
    QDomElement name = dom.createElement ( "name" );
    name.appendChild ( dom.createTextNode ( items->getName() ) );
    project.appendChild ( name );

    // Port
    QDomElement port = dom.createElement ( "port" );
    port.appendChild ( dom.createTextNode ( items->getPort() ) );
    project.appendChild ( port );

    // Remote Host
    QDomElement remotehost = dom.createElement ( "remotehost" );
    remotehost.appendChild ( dom.createTextNode ( items->getRemoteHost() ) );
    project.appendChild ( remotehost );

    // Session ID
    QDomElement sessionid = dom.createElement ( "sessionid" );
    sessionid.appendChild ( dom.createTextNode ( items->getSessionId() ) );
    project.appendChild ( sessionid );
  }

  QFile fp ( db );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << dom.toString();
    fp.close();
  }
  else
    qWarning ( "Permission Denied: %s not saved", qPrintable ( db ) );

  setWindowModified ( false );
}

XDebugProjectDialog::~XDebugProjectDialog()
{}
