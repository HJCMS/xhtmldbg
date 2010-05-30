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

#ifndef SELFHTMLSIDEBARPLUGIN_H
#define SELFHTMLSIDEBARPLUGIN_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QWidget>

/* xhtmldbg */
#include <xhtmldbgplugininfo.h>
#include <xhtmldbginterface.h>

class SelfHtmlSidebar;

class XHTMLDBG_EXPORT SelfHtmlSidebarPlugin : public xhtmldbg::Interface
{
    Q_OBJECT
    Q_INTERFACES ( xhtmldbg::Interface )

  public:
    SelfHtmlSidebar* m_sideBar;

  public Q_SLOTS:
    void proccess ();

  public:
    bool create ( QWidget * parent );
    QDockWidget* dockwidget();
    void setContent ( const QString &source );
    void setUrl ( const QUrl &url );
    xhtmldbg::PluginInfo::PluginType type ();
    xhtmldbg::PluginInfo* pluginInfo ();
};

#endif
