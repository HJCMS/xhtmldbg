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

#include "contextmenu.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QKeySequence>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

static inline const QStringList supportedHighLights()
{
  QStringList l;
  // Markup Languages
  l << "qml" << "xml" << "html" << "dtd" << "xsl" << "sgml";
  // Script Languages
  l << "php" << "perl" << "javascript" << "css" << "yaml" << "json" << "python" << "ruby" << "actionscript";
  return l;
}

ContextMenu::ContextMenu ( QWidget * parent )
    : KMenu ( parent )
{
  setObjectName ( "contextmenu" );
  setTitle ( i18n ( "Debugger" ) );
  setIcon ( KIcon ( "xhtmldbg" ) );

  // Debugger Actions
  act_check = addAction ( i18n ( "Check Source" ) );
  act_check->setStatusTip ( i18n ( "Check Document Source" ) );
  act_check->setIcon ( KIcon ( "document-edit-verify" ) );
  act_check->setShortcut ( Qt::ALT + Qt::Key_C );
  connect ( act_check, SIGNAL ( triggered() ), this, SIGNAL ( sscheck() ) );

  act_format = addAction ( i18n ( "Format Source" ) );
  act_format->setStatusTip ( i18n ( "Format Document Source" ) );
  act_format->setIcon ( KIcon ( "format-list-ordered" ) );
  act_format->setShortcut ( Qt::ALT + Qt::Key_F );
  connect ( act_format, SIGNAL ( triggered() ), this, SIGNAL ( sformat() ) );

  // Document Menu
  m_documentMenu = addMenu ( i18n ( "Document" ) );
  m_documentMenu->setIcon ( KIcon ( "documentinfo" ) );
  // Document Actions
  act_save = m_documentMenu->addAction ( i18n ( "Save" ) );
  act_save->setStatusTip ( i18n ( "Save Html Source to ..." ) );
  act_save->setIcon ( KIcon ( "document-save" ) );
  act_save->setShortcut ( QKeySequence::Save );
  connect ( act_save, SIGNAL ( triggered() ), this, SIGNAL ( ssave() ) );

  act_print = m_documentMenu->addAction ( i18n ( "Print" ) );
  act_print->setStatusTip ( i18n ( "Print current Document" ) );
  act_print->setIcon ( KIcon ( "document-print" ) );
  act_print->setShortcut ( QKeySequence::Print );
  connect ( act_print, SIGNAL ( triggered() ), this, SIGNAL ( sprint() ) );

  // Viewer Menu
  m_viewMenu = addMenu ( i18n ( "Editor" ) );
  m_viewMenu->setIcon ( KIcon ( QLatin1String ( "edit-select-all" ) ) );

  m_signalMapper = new QSignalMapper ( this );

  // Open Configuration
  act_config = m_viewMenu->addAction ( i18n ( "Configuration" ) );
  act_config->setStatusTip ( i18n ( "Show the editor's configuration dialog" ) );
  act_config->setIcon ( KIcon ( "configure" ) );
  connect ( act_config, SIGNAL ( triggered() ), this, SIGNAL ( sconfig() ) );

  m_highlightMenue = m_viewMenu->addMenu ( i18n ( "Highlight" ) );
  m_highlightMenue->setIcon ( KIcon ( QLatin1String ( "code-context" ) ) );

  connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
            this, SIGNAL ( updateHighlight ( const QString & ) ) );
}

void ContextMenu::setHighlightModes ( const QStringList &list )
{
  if ( list.size() < 1 )
    return;

  QStringList hl = supportedHighLights();
  QStringList highlights;
  foreach ( QString name, list )
  {
    if ( hl.contains ( name.toLower() ) )
      highlights.append ( name );
  }
  if ( highlights.size() < 1 )
    return;

  highlights.sort();
  m_highlightMenue->clear();
  foreach ( QString name, highlights )
  {
    QAction* ac = m_highlightMenue->addAction ( name );
    ac->setIcon ( KIcon ( QLatin1String ( "code-context" ) ) );
    connect ( ac, SIGNAL ( triggered() ), m_signalMapper, SLOT ( map() ) );
    m_signalMapper->setMapping ( ac, name );
  }
}

ContextMenu::~ContextMenu()
{
  m_highlightMenue->clear();
}
