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

#ifndef CONFIGACCESSCONTROL_H
#define CONFIGACCESSCONTROL_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

class ConfigAccessControl : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QLineEdit* sslPublicKey;
    QLineEdit* sslPrivateKey;
    QLineEdit* sslPrivatePass;

  private Q_SLOTS:
    void getPrivKeyDialog();
    void getPupKeyDialog();
    void dataChanged ( const QString & );

  Q_SIGNALS:
    void modified ( bool b = true );

  public:
    ConfigAccessControl ( QWidget * parent = 0 );
    void setPublicKeyPath ( const QString & );
    void setPrivateKeyPath ( const QString & );
    void setPassPhrase ( const QByteArray & );
    const QString getPupKey();
    const QString getPrivKey();
    const QByteArray getPassPhrase();
    ~ConfigAccessControl();
};

#endif
