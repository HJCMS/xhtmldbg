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

#ifndef STYLESHEETED_H
#define STYLESHEETED_H

/* QtCore */
#include <QtCore/QObject>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KColorDialog>
#include <KDE/KDialog>
#include <KDE/KTabWidget>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class ColorChooserWidget;
class ActionToolBar;
class Predicates;
class FontStyle;

class StyleSheeted : public KDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QWebElement origElement;
    QWebElement htmlElement;
    KTabWidget* m_tabWidget;
    ColorChooserWidget* m_colorChooserWidget;
    FontStyle* m_fontStyle;
    ActionToolBar* m_actionToolBar;
    Predicates* m_predicates;

  private Q_SLOTS:
    void colorChanged ( const QColor & );
    void fontChanged ( const QFont & );
    void fontSizeAdjust ( double );
    void fontWeight ( const QVariant & );
    void reset();

  public:
    StyleSheeted ( const QWebElement &element, QWidget * parent = 0 );
    ~StyleSheeted();
};

#endif
