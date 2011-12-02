/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "keywordeditorplugin.h"
#include "keywordeditor.h"


/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

/* KDE */
#include <KDE/KLocale>

bool KeywordEditorPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_editor = new KeywordEditor ( parent );
    m_editor->setObjectName ( QLatin1String ( "KeywordEditor" ) );
    return true;
  }
  return false;
}

QDockWidget* KeywordEditorPlugin::dockwidget()
{
  return new QDockWidget;
}

void KeywordEditorPlugin::setContent ( const QString &source )
{
  Q_UNUSED ( source )
}

void KeywordEditorPlugin::setUrl ( const QUrl &url )
{
  Q_UNUSED ( url )
}

void KeywordEditorPlugin::proccess ()
{
  m_editor->open();
}

xhtmldbg::PluginInfo::PluginType KeywordEditorPlugin::type ()
{
  return xhtmldbg::PluginInfo::PopUp;
}

xhtmldbg::PluginInfo* KeywordEditorPlugin::pluginInfo ()
{
  xhtmldbg::PluginInfo* inf = new xhtmldbg::PluginInfo ( this, type() );
  inf->setName ( QLatin1String ( "Keyword Editor" ) );
  inf->setGenericName ( i18n ( "SEO Keyword Editor" ) );
  inf->setVersion ( XHTMLDBG_VERSION );
  inf->setDescription ( i18n ( "Editor for creating SEO Keywords" ) );
  inf->setAuthor ( i18n ( "Jürgen Heinemann (Undefined)" ) );
  return inf;
}

Q_EXPORT_PLUGIN2 ( KeywordEditor, KeywordEditorPlugin )
