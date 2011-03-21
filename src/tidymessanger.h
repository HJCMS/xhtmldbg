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

#ifndef TIDYMESSANGER_H
#define TIDYMESSANGER_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

/* QTidy */
#include <QTidy/QTidyParser>

class TidyMessanger : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QListWidget* m_listWidget;
    const QIcon iconNotice;
    const QIcon iconWarning;
    const QIcon iconCritical;

  private Q_SLOTS:
    void pretended ( QListWidgetItem * );
    void sortAscending();
    void sortDescending();

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void marking ( int, int );
    void itemSelected();
    void itemsChanged ( bool );

  public Q_SLOTS:
    void messages ( const QTidy::QTidyDiagnosis & );
    void messages ( int, const QString & );
    void clearItems();

  public:
    TidyMessanger ( QWidget * parent = 0 );
    ~TidyMessanger();
};

#endif
