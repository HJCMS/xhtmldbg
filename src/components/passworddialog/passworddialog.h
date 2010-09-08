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

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

/* QtScript */
#include <QtScript/QScriptValue>

class Q_DECL_EXPORT PasswordDialog : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString user READ ownerData WRITE setOwnerData )
    Q_PROPERTY ( QString pass READ passwordData WRITE setPasswordData )

  private:
    QString owner;
    QString password;

  private Q_SLOTS:
    void restore();
    void submit();

  protected:
    QLineEdit* m_onwerLineEdit;
    QLineEdit* m_passLineEdit;

  Q_SIGNALS:
    void accept ( const QString &owner, const QString &password );

  public:
    explicit PasswordDialog ( QWidget * parent = 0 );

    /**
    * QScript Value from @ref m_onwerLineEdit Input Field
    * @see http://www.ecma-international.org/publications/standards/Ecma-262.htm
    **/
    QScriptValue scOwnerField;

    /**
    * QScript Value from @ref m_passLineEdit Input Field
    * @see http://www.ecma-international.org/publications/standards/Ecma-262.htm
    **/
    QScriptValue scPasswordField;

    /**
    * Set Meta-Object Property for @ref m_onwerLineEdit
    **/
    void setOwnerData ( const QString &u );

    /**
    * Current @ref m_passLineEdit Plain Text Data
    **/
    QString ownerData() const;

    /**
    * Set Meta-Object Property for @ref m_passLineEdit
    **/
    void setPasswordData ( const QString &p );

    /**
    * Current @ref m_passLineEdit Plain Text Data
    **/
    QString passwordData() const;

    virtual ~PasswordDialog();
};

#endif
