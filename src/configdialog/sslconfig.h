/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://qtidy.hjcms.de, (C) 2007-2010
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

#ifndef SSLCONFIG_H
#define SSLCONFIG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QScrollArea>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslConfiguration>

class SSLConfig : public QScrollArea
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSslConfiguration ssl;
    QWidget* centeredWidget;
    QLineEdit* sslPeerCertificate;
    QLineEdit* sslPrivateKey;
    QLineEdit* sslCaCertsDatabase;
    QTableWidget* sslIssuers;
    void fillCaCertIssuerTable();

  private Q_SLOTS:
    void setCaCertDatabase ( const QString &p = QString() );
    void getPeerCertDialog();
    void getCaCertDatabaseDialog();

  Q_SIGNALS:
    void modified ();

  public Q_SLOTS:
    void load ( QSettings * cfg );

  public:
    SSLConfig ( QWidget * parent = 0 );
    virtual ~SSLConfig();
};

#endif
