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

#include "contextmenu.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

/* KDE */
#include <KDE/KLocale>
#include <KDE/KIcon>

ContextMenu::ContextMenu ( QWidget * parent )
    : QMenu ( parent )
{
  setObjectName ( QLatin1String ( "contextmenu" ) );
  setTitle ( i18n ( "Debugger" ) );

  // Debugger Actions
  act_check = addAction ( i18n ( "Check Source" ) );
  act_check->setObjectName ( "menuactiondebuggercheck" );
  act_check->setStatusTip ( i18n ( "Check Document Source" ) );
  act_check->setIcon ( KIcon ( QLatin1String ( "document-edit-verify" ) ) );
  act_check->setShortcut ( Qt::ALT + Qt::Key_C );
  connect ( act_check, SIGNAL ( triggered() ), this, SIGNAL ( sscheck() ) );

  act_format = addAction ( i18n ( "Format Source" ) );
  act_format->setObjectName ( "menuactiondebuggerformat" );
  act_format->setStatusTip ( i18n ( "Format Document Source" ) );
  act_format->setIcon ( KIcon ( QLatin1String ( "format-list-ordered" ) ) );
  act_format->setShortcut ( Qt::ALT + Qt::Key_F );
  connect ( act_format, SIGNAL ( triggered() ), this, SIGNAL ( sformat() ) );

  // Document Menu
  m_documentMenu = addMenu ( i18n ( "Document" ) );
  m_documentMenu->setIcon ( KIcon ( QLatin1String ( "documentinfo" ) ) );
  // Document Actions
  act_save = m_documentMenu->addAction ( i18n ( "Save" ) );
  act_save->setObjectName ( "menuactiondocumentsave" );
  act_save->setStatusTip ( i18n ( "Save Html Source to ..." ) );
  act_save->setIcon ( KIcon ( QLatin1String ( "document-save" ) ) );
  act_save->setShortcut ( QKeySequence::Save );
  connect ( act_save, SIGNAL ( triggered() ), this, SIGNAL ( ssave() ) );

  act_print = m_documentMenu->addAction ( i18n ( "Print" ) );
  act_print->setObjectName ( "menuactionddocumentprint" );
  act_print->setStatusTip ( i18n ( "Print current Document" ) );
  act_print->setIcon ( KIcon ( QLatin1String ( "document-print" ) ) );
  act_print->setShortcut ( QKeySequence::Print );
  connect ( act_print, SIGNAL ( triggered() ), this, SIGNAL ( sprint() ) );

  // Viewer Menu
  m_viewMenu = addMenu ( i18n ( "View" ) );
  m_viewMenu->setIcon ( KIcon ( QLatin1String ( "edit-select-all" ) ) );
  // Viewer Actions @see QTextEdit::setLineWrapMode
  act_wrap = m_viewMenu->addAction ( i18n ( "Wordwrap" ) );
  act_wrap->setObjectName ( "menuactionviewrap" );
  act_wrap->setStatusTip ( i18n ( "enable disable line wrap mode" ) );
  act_wrap->setIcon ( KIcon ( QLatin1String ( "document-properties" ) ) );
  act_wrap->setShortcut ( QKeySequence ( Qt::Key_F10 ) );
  connect ( act_wrap, SIGNAL ( triggered() ), this, SIGNAL ( swrap() ) );
}

ContextMenu::~ContextMenu()
{
}
