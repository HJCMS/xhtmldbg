/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2012
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
#include <QtCore/QStringList>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QPrinter>
#include <QtGui/QWidget>

/* QTidy */
#include <QTidy/QTidyParser>

/* KDE */
#ifndef KDE_NO_DEBUG_OUTPUT
# define KDE_NO_DEBUG_OUTPUT 1
#endif
#include <KDE/KTextEditor/Document>
#include <KDE/KTextEditor/View>

class ContextMenu;
class SourceCache;

class SourceWidget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )
    Q_PROPERTY ( QString tidyrc READ getTidyrc WRITE setTidyrc )

  private:
    QString tidyrc;
    KTextEditor::Document* m_document;
    KTextEditor::View* m_view;
    QTidy::QTidyParser* m_parser;
    SourceCache* m_sourceCache;
    ContextMenu* m_menu;

  private Q_SLOTS:
    void editorMenuEvent ( KTextEditor::View *, QMenu * );
    void saveSource();
    void openConfig();
    void printSource();
    void updatePrintPreview ( QPrinter * );
    void switchHighlight ( const QString & );

  Q_SIGNALS:
    void clearMessages ();
    void triggered ( const QTidy::QTidyDiagnosis & );

  public Q_SLOTS:
    void fetchBlock ( int, int );
    void check();
    void format();
    void restore();

  public:
    SourceWidget ( QWidget * parent = 0 );
    const QString getTidyrc();
    void setTidyrc ( const QString & );
    void setSource ( const QUrl &, const QString &source );
    ~SourceWidget();
};

#endif
