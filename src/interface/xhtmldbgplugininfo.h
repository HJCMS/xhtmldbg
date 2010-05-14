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

#ifndef XHTMLDBGPLUGININFO_H
#define XHTMLDBGPLUGININFO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace xhtmldbg
{
  /**
  * \short The Basic xhtmldbg Plugin Description Class
  * \class PluginInfo
  */
  class PluginInfo : virtual public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Description", "XHTMLDBG Plugin Info" )
      Q_ENUMS ( PluginType )
      Q_PROPERTY ( QString Name READ getName WRITE setName )
      Q_PROPERTY ( QString GenericName READ getGenericName WRITE setGenericName )
      Q_PROPERTY ( QString Version READ getVersion WRITE setVersion )
      Q_PROPERTY ( QString Description READ getDescription WRITE setDescription )
      Q_PROPERTY ( QString Author READ getAuthor WRITE setAuthor )

    public:
      enum PluginType
      {
        Dialog,   /**< \short this Plugin is a QDialog */
        Dock      /**< \short this Plugin is a QDockWidget */
      };

    private:
      const PluginType pluginType; /**< \short Plugin Type */
      QString Name;                /**< \short Plugin Name */
      QString GenericName;         /**< \short Plugin Menu Name */
      QString Version;             /**< \short Plugin Version */
      QString Description;         /**< \short Plugin Description */
      QString Author;              /**< \short Plugin Author */

    public:
      explicit PluginInfo ( QObject * parent = 0, PluginType type = PluginInfo::Dialog );

      /** \short Plugin Name */
      const QString getName();

      /** \short Plugin Menu Name */
      const QString getGenericName();

      /** \short Plugin Version */
      const QString getVersion();

      /** \short Plugin Description */
      const QString getDescription();

      /** \short Plugin Author */
      const QString getAuthor();

      /** Plugin Name
      * \param n  Full Plugin Name
      */
      void setName ( const QString &n );

      /** Plugin Name
      * \param n  Plugin Name for Menu
      */
      void setGenericName ( const QString &n );

      /** Plugin Name
      * \param n  Full Plugin Name
      */
      void setVersion ( const QString &v );

      /** Plugin Description
      * \param d Description default: Missing
      */
      void setDescription ( const QString &d = QLatin1String ( "Missing" ) );

      /** Plugin Author
      * \param a Authow default: Unknown
      */
      void setAuthor ( const QString &a = QLatin1String ( "Unknown" ) );

  }; /* eof plugininfo */

} /* eof namespace xhtmldbg */

#endif
