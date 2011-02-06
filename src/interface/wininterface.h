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

#ifndef XHTMLDBG_WININTERFACE_H
#define XHTMLDBG_WININTERFACE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>

namespace xhtmldbg
{
  /**
  * \class WinInterface
  * \brief Basic DBus Main Window Interface
  */
  class Q_DECL_EXPORT WinInterface: public QDBusInterface
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
      Q_CLASSINFO ( "D-Bus Interface", "de.hjcms.xhtmldbg" )

    private:
      inline bool isValid ( const QUrl &url ) const;
      void hasErrors();

    protected:
      const QString interface;
      const QDBusConnection bus;

    Q_SIGNALS:
      /** Send DBus Error Messages */
      void error ( const QString &name, const QString &message );

    public Q_SLOTS:
      /** Send Notification Message */
      void noticeMessage ( const QString &message );

      /** Send Error Message */
      void errorMessage ( const QString &message );

      /** Send Error Message
      * \param url     Request URL
      * \param newTab  Open a new Tab
      */
      void open ( const QUrl &url, bool addtab = true );

      /** Update Page url to new
      * \param oldUrl  Current Page URL
      * \param newUrl  Request URL
      */
      void setUrl ( const QUrl &oldUrl, const QUrl &newUrl );

      /** Send Source for Requested URL
      * \param url  Request URL
      * \param src  XHTML/HTML/XML Source
      */
      void setSource ( const QUrl &url, const QString &src );

    public:
      WinInterface ( const QDBusConnection &dbus, QObject *parent = 0 );

      virtual ~WinInterface();
  };
}

namespace de
{
  namespace hjcms
  {
    typedef ::xhtmldbg::WinInterface xhtmldbg;
  }
}

#endif

