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

#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QSizePolicy>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class PageWidget : public QScrollArea
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    QGroupBox* m_groupBox;
    QLabel* m_labelBottom;
    QSpacerItem* verticalBottomSpacer;

  protected:
    QWidget* centralWidget;
    void setNotice ( bool b = false );
    void setCheckable ( bool b = true );
    bool isChecked ();
    virtual void load ( QSettings * ) = 0;
    virtual void save ( QSettings * ) = 0;
    virtual void defaults () = 0;

  protected Q_SLOTS:
    void setChecked ( bool );

  Q_SIGNALS:
    void modified ( bool );

  public Q_SLOTS:
    void polishVerticalSpacer ( QSizePolicy::Policy );

  public:
    PageWidget ( const QString &title, QWidget * parent = 0 );
    virtual ~PageWidget();
};

#endif
