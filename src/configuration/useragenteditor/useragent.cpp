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

#include "useragent.h"

/* QtCore */
#include <QtCore/QString>

/* KDE */
#include <KDE/KLocale>

UserAgent::UserAgent ( QObject * parent )
    : QObject ( parent )
{
  setObjectName ( QLatin1String ( "UserAgent" ) );
}

const QString UserAgent::getName()
{
  if ( name.isEmpty() )
    return i18n ( "XHTMLDBG (Default)" );
  else
    return name;
}

void UserAgent::setName ( const QString &p )
{
  if ( p.isEmpty() )
    return;

  name = p;
}

const QString UserAgent::getAgent()
{
  if ( agent.isEmpty() )
    return QString::fromUtf8 ( "Mozilla/5.0" );
  else
    return agent;
}

void UserAgent::setAgent ( const QString &p )
{
  if ( p.isEmpty() )
    return;

  agent = p;
}

const QString UserAgent::getTokens()
{
  if ( tokens.isEmpty() )
    return QString::fromUtf8 ( "(compatible; XHTMLDBG; en, en_US; X11) AppleWebKit (KHTML, like Gecko)" );
  else
    return tokens;
}

void UserAgent::setTokens ( const QString &p )
{
  if ( p.isEmpty() )
    return;

  tokens = p;
}

UserAgent::~UserAgent()
{
}
