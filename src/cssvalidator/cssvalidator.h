/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2011
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
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

/* xhtmldbg */
#include "settings.h"

class ValidatorMenu;
class Validator;
class SoupReader;

class CSSValidator : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    Settings* cfg;
    const QIcon iconNotice;
    const QIcon iconWarning;
    const QIcon iconCritical;
    QString usedUrl;
    QListWidget* m_listWidget;
    ValidatorMenu* m_menu;
    Validator* m_validator;
    SoupReader* m_soupReader;
    QByteArray soupData;

    bool isUnique ( const QUrl & );
    bool prepareToExecute ( const QUrl & );
    void openConfigurationDialog();
    void placeUrlItem ( const QString &, const QUrl & );

  private Q_SLOTS:
    void processTriggered();
    void markItem ( QListWidgetItem * );
    void doubleClicked ( QListWidgetItem * );
    void sortAscending();
    void sortDescending();
    void openConfig();
    void noticeItem ( const QString & );
    void warningItem ( const QString & );
    void criticalItem ( const QString & );
    void errors ( QProcess::ProcessError );
    void exited ( int, QProcess::ExitStatus );
    void readStandardReply();
    void shutdownProcess();

  protected:
    void contextMenuEvent ( QContextMenuEvent * );
    void mouseMoveEvent ( QMouseEvent * );

  Q_SIGNALS:
    void itemsChanged();

  public Q_SLOTS:
    void addForValidation ( const QUrl & );
    void forceValidation ( const QUrl & );
    void clearItems();

  public:
    CSSValidator ( QWidget * parent = 0, Settings * settings = 0 );
    ~CSSValidator();
};

#endif
