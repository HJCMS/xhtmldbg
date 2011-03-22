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

#ifndef FONTWEIGHT_H
#define FONTWEIGHT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QWidget>

/**
* \class FontWeight
* The FontWeight ComboBox provides weight mappings for a font family.
* \link http://www.w3.org/TR/css3-fonts/#font-weight-the-font-weight-property
*
* \b 100 \b to \b 900
* These values form an ordered sequence, where each number indicates a weight that
* is at least as dark as its predecessor. These roughly correspond to the commonly
* used weight names below:
* \li 100 - Thin
* \li 200 - Extra Light (Ultra Light)
* \li 300 - Light
* \li 400 - Normal
* \li 500 - Medium
* \li 600 - Semi Bold (Demi Bold)
* \li 700 - Bold
* \li 800 - Extra Bold (Ultra Bold)
* \li 900 - Black (Heavy)
*
*/
class FontWeight : public QComboBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private Q_SLOTS:
    void valueChanged ( int index );

  Q_SIGNALS:
    void weightChanged ( const QVariant & );

  public Q_SLOTS:
    /** Same as ‘400’. */
    void setNormal();

    /** Same as ‘700’. */
    void setBold();

    /** Specifies the weight of the face bolder than the inherited value. */
    void setBolder();

    /** Specifies the weight of the face lighter than the inherited value. */
    void setLighter();

    /**
    * \param value Inherited: 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900
    */
    void setValue ( const QVariant &value );

  public:
    FontWeight ( QWidget * parent = 0 );

    /**
    * Valid Values:
    *   \li QFont::Light
    *   \li QFont::Normal
    *   \li QFont::DemiBold
    *   \li QFont::Bold
    *   \li QFont::Black
    */
    int qFontWeight();

    /**
    * Valid Values:
    *   \li Inherited: 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900
    */
    const QVariant value();

    ~FontWeight();
};

#endif
