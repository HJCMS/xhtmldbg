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

#ifndef XHTMLDBG_PLUGGER_H
#define XHTMLDBG_PLUGGER_H

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QGlobalStatic>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaObject>

/* QtGui */
#include <QtGui/QWidget>

#ifndef XHTMLDBG_VERSION
# include <plugininfo.h>
#endif

namespace xhtmldbg
{
  class Interface;

  class Q_DECL_EXPORT Plugger : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

    private:
      QDir p_dir;
      QStringList filters;
      const QStringList findPlugins ();

    public:
      explicit Plugger ( QObject * parent = 0 );
      const QString findPlugin ( const QString &find );
      const QList<xhtmldbg::Interface*> pluginsByType ( QWidget * parent,
              xhtmldbg::PluginInfo::PluginType type = xhtmldbg::PluginInfo::PopUp );
      virtual ~Plugger();
  };
}

#endif
