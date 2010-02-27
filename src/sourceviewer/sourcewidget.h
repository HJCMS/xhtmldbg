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

#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QWidget>

/* QTidy */
#include <QTidy/QTidyParser>

class SourceView;
class ListLines;

class SourceWidget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://xhtmldbg.hjcms.de" )
    Q_PROPERTY ( QString tidyrc READ getTidyrc WRITE setTidyrc )

  private:
    SourceView* m_sourceView;
    ListLines* m_listLines;
    QString tidyrc;

  Q_SIGNALS:
    void triggered ( const QTidy::QTidyDiagnosis & );

  public Q_SLOTS:
    void setSource ( const QString &source );
    void fetchBlock ( int, int );
    // void fetchColumn ( int );
    void check();
    void format();

  public:
    SourceWidget ( QWidget * parent = 0 );
    const QString getTidyrc();
    void setTidyrc ( const QString & );
    ~SourceWidget();
};

#endif
