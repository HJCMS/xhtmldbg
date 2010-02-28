/**
* This file is part of the QTidy project
*
* Copyright (C) Juergen Heinemann http://xhtmldbg.hjcms.de, (C) 2007-2010
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

#ifndef APPLICATION_H
#define APPLICATION_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>

/* QtNetwork */
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

class HistoryManager;
class NetworkAccessManager;

class Application : public QApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QLocalServer* m_server;
    QString myName() const;
    static HistoryManager* p_historyManager;
    static NetworkAccessManager* p_networkAccessManager;

  private Q_SLOTS:
    void newConnection();

  Q_SIGNALS:
    void sMessageReceived ( QLocalSocket *socket );

  public:
    Application ( int &argc, char **argv );
    bool startUniqueServer();
    bool sendMessage ( const QByteArray &mess, int rwait = 0 );
    bool isRunning() const;
    void busEventHandler ( const QString &type, const QString &str );
    static HistoryManager* historyManager();
    static NetworkAccessManager* networkAccessManager();
    ~Application();

};

#endif