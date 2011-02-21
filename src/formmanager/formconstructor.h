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

#ifndef FORMCONSTRUCTOR_H
#define FORMCONSTRUCTOR_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QString>

/* QtWebKit */
#include <QtWebKit/QWebElement>

/**
* This Class read all given QWebElement::FORM Elements
* and remove not needet predicates from it
*/
class FormConstructor : public QWebElement
{
  private:
    bool isValidInputElement ( const QWebElement & );
    void appendElement ( const QWebElement & );
    void appendInputElement ( const QWebElement & );
    void findInputs();
    void findTextAreas();
    void findSelections();

  public:
    /** Create a new QWebElement with given FORM Element */
    FormConstructor ( const QWebElement &form );

    /** List with stripped WebElements */
    QList<QWebElement> elements;

    /** rewrite the \ref elements list */
    void rebuild();

    virtual ~FormConstructor();
};

#endif
