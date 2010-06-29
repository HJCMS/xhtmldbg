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

#ifndef USERAGENT_H
#define USERAGENT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

class UserAgent : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString name READ getName WRITE setName )
    Q_PROPERTY ( QString agent READ getAgent WRITE setAgent )
    Q_PROPERTY ( QString tokens READ getTokens WRITE setTokens )

  private:
    QString name;
    QString agent;
    QString tokens;

  public:
    UserAgent ( QObject * parent = 0 );
    const QString getName();
    void setName ( const QString & );
    const QString getAgent();
    void setAgent ( const QString & );
    const QString getTokens();
    void setTokens ( const QString & );
    ~UserAgent ();
};

#endif
