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

#include "passworddialog.h"
#include "passworddialogplugin.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

PasswordDialogPlugin::PasswordDialogPlugin ( QObject * parent )
    : QObject ( parent )
    , initialized ( false )
{
  setObjectName ( QLatin1String ( "PasswordDialogPlugin" ) );
}

bool PasswordDialogPlugin::isContainer() const
{
  return false;
}

bool PasswordDialogPlugin::isInitialized() const
{
  return initialized;
}

QIcon PasswordDialogPlugin::icon() const
{
  return  QIcon::fromTheme ( QLatin1String ( "xhtmldbg" ) );
}

QString PasswordDialogPlugin::domXml() const
{
  return "<widget class=\"XPasswordDialog\" name=\"xPasswordDialog\">\n"
         " <property name=\"geometry\">\n"
         "  <rect>\n"
         "   <x>0</x>\n"
         "   <y>0</y>\n"
         "   <width>250</width>\n"
         "   <height>250</height>\n"
         "  </rect>\n"
         " </property>\n"
         " <property name=\"toolTip\" >\n"
         "  <string>PasswordDialog Plugin</string>\n"
         " </property>\n"
         " <property name=\"whatsThis\" >\n"
         "  <string>Password Login Widget for Web Integration</string>\n"
         " </property>\n"
         "</widget>\n";
}

QString PasswordDialogPlugin::group() const
{
  return "xhtmldbg";
}

QString PasswordDialogPlugin::includeFile() const
{
  return "passworddialog.h";
}

QString PasswordDialogPlugin::name() const
{
  return "XPasswordDialog";
}

QString PasswordDialogPlugin::toolTip() const
{
  return QString::null;
}

QString PasswordDialogPlugin::whatsThis() const
{
  return "Password Login Widget for Web Integration";
}

QWidget* PasswordDialogPlugin::createWidget ( QWidget * parent )
{
  return new PasswordDialog ( parent );
}

void PasswordDialogPlugin::initialize ( QDesignerFormEditorInterface * formEditor )
{
  Q_UNUSED ( formEditor )
  if ( initialized )
    return;

  initialized = true;
}

Q_EXPORT_PLUGIN2 ( XPasswordDialog, PasswordDialogPlugin )
