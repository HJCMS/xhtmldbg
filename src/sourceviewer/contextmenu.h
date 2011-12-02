/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2012
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

#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSignalMapper>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KMenu>

class ContextMenu : public KMenu
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    // Debugger Actions
    QAction* act_check;
    QAction* act_format;
    // Document Menu
    QMenu* m_documentMenu;
    QAction* act_save;
    QAction* act_print;
    // Viewer Highlight Mode Menu
    QSignalMapper* m_signalMapper;
    QMenu* m_viewMenu;
    QMenu* m_highlightMenue;
    // Editor Configuration
    QAction* act_config;

  Q_SIGNALS:
    // Debugger Signals
    void sscheck();
    void sformat();
    // Document Signals
    void ssave();
    void sprint();
    // Viewer Highlight Mode
    void updateHighlight ( const QString & );
    // Editor Configuration
    void sconfig();

  public:
    explicit ContextMenu ( QWidget * parent = 0 );
    void setHighlightModes ( const QStringList & );
    virtual ~ContextMenu();
};

#endif
