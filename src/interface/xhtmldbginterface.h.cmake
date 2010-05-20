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

#ifndef XHTMLDBGINTERFACE_H
#define XHTMLDBGINTERFACE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QMetaObject>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QWidget>
#include <QtPlugin>

/* xhtmldbg */
#include <xhtmldbgplugininfo.h>

namespace xhtmldbg
{
  /**
  * XHTMLDBG Interface Version
  */
#ifndef XHTMLDBG_VERSION
# define XHTMLDBG_VERSION    "@XHTMLDBG_VERSION@"
#endif

  /**
  * \short xhtmldbg Plugin Interface Class.
  * \class Interface
  */
  class XHTMLDBG_EXPORT Interface : virtual public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Description", "XHTMLDBG Interface" )
      Q_CLASSINFO ( "Version", "@XHTMLDBG_VERSION@" )
      Q_CLASSINFO ( "Url", "http://xhtmldbg.hjcms.de" )

    public Q_SLOTS:
      /** This SLOT ist Used from triggered Action by MenuBar */
      virtual void proccess () = 0;

    public:
      /** Plugin Initialisation */
      virtual bool create ( QWidget * parent ) = 0;

      /**
      * This is needed for implement the DockWidget into the IDE
      */
      virtual QDockWidget* dockwidget () = 0;

      /**
      * This function set the current Document Source Content
      * and will emitted when xhtmldbg has Document completed.
      * \param source Current Document Content
      */
      virtual void setContent ( const QString &source ) = 0;

      /**
      * This function set the current Document Url
      * and will emitted with xhtmldbg url load finished.
      * \param url Current Document Url
      */
      virtual void setUrl ( const QUrl &url ) = 0;

      /**
      * Information about this Plugin Type
      */
      virtual PluginInfo::PluginType type () = 0;

      /**
      * Complete Plugin Information
      */
      virtual PluginInfo* pluginInfo () = 0;
  }; /* eof interface */

} /* eof namespace xhtmldbg */

Q_DECLARE_INTERFACE ( xhtmldbg::Interface, "de.hjcms.xhtmldbg.interface/@XHTMLDBG_VERSION@" )

#endif
