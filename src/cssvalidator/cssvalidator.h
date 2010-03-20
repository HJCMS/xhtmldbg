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

#ifndef CSSVALIDATOR_H
#define CSSVALIDATOR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

class Validator;
class SoupReader;

class CSSValidator : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )

  private:
    QSettings* cfg;
    Validator* m_validator;
    SoupReader* m_soupReader;
    QListWidget* m_listWidget;
    const QIcon iconNotice;
    const QIcon iconWarning;
    const QIcon iconCritical;
    void readInputData ( const QByteArray & );

  private Q_SLOTS:
    void sortAscending();
    void sortDescending();
    void noticeItem ( const QString & );
    void warningItem ( const QString & );
    void criticalItem ( const QString & );
    void errors ( QProcess::ProcessError );
    void exited ( int, QProcess::ExitStatus );
    void readStandardReply ();

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void itemsChanged();

  public Q_SLOTS:
    void runCssCheck ( const QUrl & );
    void clearItems();

  public:
    CSSValidator ( QWidget * parent = 0, QSettings * settings = 0 );
    virtual ~CSSValidator();
};

#endif
