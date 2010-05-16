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

#ifndef XDEBUGGER_H
#define XDEBUGGER_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QSplitter>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class XDebugSocket;
class XDebugMenuBar;

class XDebugger : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSettings* cfg;
    QComboBox* selectProject;
    QSpinBox* spinPortBox;
    QLineEdit* sessionIdEdit;
    QLineEdit* remoteHostEdit;
    QTextEdit* m_Messanger;
    XDebugMenuBar* m_debuggerBar;
    XDebugSocket* m_debugSocket;
    void openProjectEditDialog();

  private Q_SLOTS:
    void loadProjectsIntoComboBox();
    void findProjectItems ( int );
    void editProjects();
    void setSpinBox ( const QVariant &p = QVariant ( 9000 ) );
    void openXDebugListener();
    void closeXDebugListener();
    void setMessage ( const QString & );
    void sendCommandLine();
    void appendMessage ( const QString & );
    void setReply ( const QString & );

  public Q_SLOTS:
    void openSessionWithIdKey ( const QString & );

  public:
    XDebugger ( QWidget * parent = 0, QSettings * settings = 0 );
    virtual ~XDebugger();
};

#endif
