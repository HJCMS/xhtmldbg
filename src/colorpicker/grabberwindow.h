/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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

#ifndef GRABBERWINDOW_H
#define GRABBERWINDOW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QTimerEvent>

/* QtGui */
#include <QtGui/QPaintEvent>
#include <QtGui/QPen>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

class GrabberWindow : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    int intervalId;
    int fixedSize;
    int penSize;
    int zoomFactor;
    QPen crossPen;
    QPen framePen;
    QPixmap m_bufferPixmap;
    void listener();

  protected:
    void timerEvent ( QTimerEvent * );
    void paintEvent ( QPaintEvent * );

  Q_SIGNALS:
    void recording ( bool );

  public Q_SLOTS:
    void startRecording ( bool );

  public:
    GrabberWindow ( QWidget * parent = 0 );
    virtual ~GrabberWindow();
};

#endif
