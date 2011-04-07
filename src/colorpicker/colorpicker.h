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

#ifndef COLORPICKER_H
#define COLORPICKER_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>

/* KDE */
#include <KDE/KIntNumInput>

class ColorTable;
class GrabberWindow;
class HexInput;

class ColorPicker : public QDockWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QComboBox* m_colorComboBox;
    ColorTable* m_colorTable;
    QLineEdit* m_hexEdit;
    QLabel* m_preview;
    QLineEdit* m_rgbEdit;
    GrabberWindow* m_grabberWindow;
    HexInput* m_hexInput;
    KIntNumInput* rgb_red;
    KIntNumInput* rgb_green;
    KIntNumInput* rgb_blue;
    void findPixelColor ( const QPoint & );
    void openColorEditor();

  private Q_SLOTS:
    void colorInput();
    void colorDialog();
    void colorMapChanged ( int );
    void colorChanged ( const QColor & );
    void rgbChanged ( int );
    void pointerChanged ( const QPoint & );

  protected:
    void mousePressEvent ( QMouseEvent * );

  Q_SIGNALS:
    void recording ( bool );

  public Q_SLOTS:
    Q_SCRIPTABLE void tapping ( bool );

  public:
    ColorPicker ( QWidget * parent = 0 );
    ~ColorPicker();
};

#endif
