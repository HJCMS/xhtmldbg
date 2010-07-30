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

#include "uitoolsloader.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QLabel>

UiToolsLoader::UiToolsLoader ( const QString &cid, QObject * parent )
    : QUiLoader ( parent )
    , classID ( cid )
{
  setObjectName ( QLatin1String ( "uitoolsloader" ) );
}

void UiToolsLoader::setConfig ( const QStringList &params, const QStringList &values )
{
  qDebug() << Q_FUNC_INFO << params << values;
}

bool UiToolsLoader::isLoadable()
{
  if ( ! classID.contains ( QRegExp ( "^Q\\w+" ) ) )
    return false;

  return availableWidgets().contains ( classID );
}

UiToolsLoader::~UiToolsLoader()
{}
