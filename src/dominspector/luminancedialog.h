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

#ifndef LuminanceDialog_H
#define LuminanceDialog_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KDialog>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class LuminanceDialog : public KDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( bool valid READ isValid )

  private:
    const QWebElement htmlElement;
    bool valid;
    QLabel* m_foregroundColor;
    QLabel* m_backgroundColor;
    QLabel* m_foregroundName;
    QLabel* m_backgroundName;
    QLabel* m_foregroundContrastRatio;
    QLabel* m_backgroundContrastRatio;
    QProgressBar* m_differenceBar;
    QProgressBar* m_brightnessBar;
    QComboBox* m_messanger;
    double contrastRatio ( const QColor &col ) const;
    const QColor toColor ( const QString &str ) const;
    const QColor getStyleColor ( const QString &property ) const;
    void setRange ( const QColor &front, const QColor &back );

  public:
    LuminanceDialog ( const QWebElement &element, QWidget * parent = 0 );
    bool isValid();
    ~LuminanceDialog();
};

#endif
