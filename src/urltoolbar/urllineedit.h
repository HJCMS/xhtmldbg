/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
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

#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QCompleter>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QFocusEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KUriFilter>
#include <KDE/KUrl>

class UrlCompleterModel;

class UrlLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    KUriFilter* m_uriFilter;
    UrlCompleterModel* m_completerModel;
    const QStringList uriFilters();
    inline bool checkInput ( const QString &inp );

  private Q_SLOTS:
    void inputValidate ( const QString &inp );

  protected:
    void focusInEvent ( QFocusEvent * event );
    void contextMenuEvent ( QContextMenuEvent * event );

  Q_SIGNALS:
    void urlChanged ( const QUrl &url );

  public Q_SLOTS:
    void startAutoEdit ();
    void urlEntered ();
    void setUrl ( const KUrl &url );

  public:
    UrlLineEdit ( QWidget * parent = 0 );
    const KUrl url();
    ~UrlLineEdit();
};

#endif
