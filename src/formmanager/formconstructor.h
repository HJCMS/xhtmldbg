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
#include <QtWebKit/QWebElementCollection>

class FormConstructor : public QWebElement
{
  private:
    QList<QWebElement> items;
    // Inputs
    enum InputType { NONE, TEXT, PASSWORD, CHECKBOX, RADIO, HIDDEN };
    FormConstructor::InputType isInputType ( const QWebElement & );
    void appendInput ( const QWebElement &, InputType );
    void findInputs();
    // TextArea
    void appendTextArea ( const QWebElement & );
    void findTextAreas();
    // Select Boxes
    void appendSelection ( const QWebElement & );
    void findSelections();

  protected:
    void rebuild();

  public:
    FormConstructor ( const QWebElement &form );

    /** TODO */
    const QList<QWebElement> content();

    virtual ~FormConstructor();
};

#endif
