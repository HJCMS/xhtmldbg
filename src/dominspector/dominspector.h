/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2010
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

#ifndef DOMINSPECTOR_H
#define DOMINSPECTOR_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QDockWidget>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class DomTree;
class DomToolBar;
class ListStyleSheet;

class DomInspector : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    QSettings* cfg;
    DomToolBar* m_domToolBar;
    QSplitter* m_domSplitter;
    DomTree* m_domTree;
    ListStyleSheet* m_listStyleSheet;
    bool hasBorderStyleSheet ( const QWebElement & ) const;
    const QStringList foundStylesheetReferences ( const QWebElement & ) const;

  private Q_SLOTS:
    void setElementVisible ( const QWebElement & );

  protected:
    struct SelectedItem
    {
      bool border;
      bool background;
      QWebElement element;
    };
    QList<SelectedItem> lastSelections;

  Q_SIGNALS:
    void cascadedStylesHref ( const QStringList & );

  public Q_SLOTS:
    void setDomTree ( const QWebElement & );
    void findItem ( const QWebElement & );

  public:
    DomInspector ( QWidget * parent = 0, QSettings * settings = 0 );
    virtual ~DomInspector ();
};

#endif
