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

#ifndef CONFIGPROXY_H
#define CONFIGPROXY_H

/* QtCore */
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkProxy>

#include "pagewidget.h"

class ConfigProxy : public PageWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QString HostName, User, Password;
    int Port;
    QComboBox* proxyType;
    QLineEdit* proxyHostName;
    QSpinBox*  proxyPort;
    QLineEdit* proxyUser;
    QLineEdit* proxyPassword;

  private Q_SLOTS:
    void itemClicked ( const QVariant &d = QVariant() );

  public Q_SLOTS:
    void setType ( QNetworkProxy::ProxyType type );
    void setHostName ( const QString & );
    void setPort ( int );
    void setUser ( const QString & );
    void setPassword ( const QString & );

  public:
    ConfigProxy ( QWidget * parent = 0 );
    QNetworkProxy::ProxyType getType ();
    const QString getHostName ();
    int getPort ();
    const QString  getUser ();
    const QString  getPassword ();
    void load ( Settings * );
    void save ( Settings * );
    bool isModified ();
    bool isSighted ();
    virtual ~ConfigProxy();
};

#endif
