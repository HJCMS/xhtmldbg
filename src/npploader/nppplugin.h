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

#ifndef NPPPLUGIN_H
#define NPPPLUGIN_H

/* QtCore */
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>

extern "C"
{
#include <npapi.h>
#include <npfunctions.h>
}

class NPPPlugin
{
  private:
    bool m_isEnabled;
    bool m_isLoaded;
    int m_loadCount;
    const QFileInfo m_pluginInfo;

    NPNetscapeFuncs m_browserFuncs;
    void initializeBrowserFuncs();

  public:
    NPPPlugin ( const QString &realPath );
    bool isEnabled() const;
    void setEnabled ( bool );
    bool load();
    void unload();
    virtual ~NPPPlugin();
};

#endif
