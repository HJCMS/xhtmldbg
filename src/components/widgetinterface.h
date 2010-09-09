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

#ifndef WIDGETINTERFACE_H
#define WIDGETINTERFACE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QWidget>

/* QtDesigner */
#include <QtDesigner/QDesignerExportWidget>
#include <QtDesigner/QDesignerIntegrationInterface>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtDesigner/QDesignerFormEditorInterface>

class QDESIGNER_WIDGET_EXPORT WidgetInterface
    : public QDesignerCustomWidgetInterface
{
    Q_INTERFACES ( QDesignerCustomWidgetInterface )

  private:
    const QString pluginClassName;
    const QString pluginObjectName;
    bool initialized;

  public:
    explicit WidgetInterface ( const QString &clName, const QString &objName );
    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString codeTemplate() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const = 0;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget* createWidget ( QWidget * parent ) = 0;
    void initialize ( QDesignerFormEditorInterface * formEditor );
};

#endif
