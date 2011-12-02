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

    /**
    * Returns true if the custom widget is intended to
    * be used as a container; otherwise returns false.
    * @return false by default
    */
    bool isContainer() const;

    /**
    * Returns true if the widget has been initialized; otherwise returns false.
    */
    bool isInitialized() const;

    /**
    * Returns the icon used to represent the custom widget in Qt Designer's widget box.
    * Default is xhtmldbg Icon from Icon Theme
    */
    QIcon icon() const;

    /**
    * This function is reserved for future use by Qt Designer. 
    */
    QString codeTemplate() const;

    /**
    * Returns the XML that is used to describe the custom widget's properties to Qt-Designer.
    */
    QString domXml() const;

    /**
    * Returns the name of the group to which the custom widget belongs.
    * @return "xhtmldbg" Default
    */
    QString group() const;

    /**
    * Returns the path to the include file that uic useswhen creating code for the custom widget.
    * @note The Path must be relative from "xhtmldbg" Include Path!
    */
    QString includeFile() const = 0;

    /**
    * Returns the class name of the custom widget supplied by the interface.
    * The name returned must be identical to the class name used for the custom widget.
    */
    QString name() const;

    /**
    * Returns a short description of the widget that
    * can be used by Qt Designer in a tool tip.
    */
    QString toolTip() const;

    /**
    * Returns a description of the widget that can be used
    * by Qt Designer in "What's This?" help for the widget.
    */
    QString whatsThis() const = 0;

    /**
    * Returns a new instance of the custom widget, with the given parent.
    */
    QWidget* createWidget ( QWidget * parent ) = 0;

    /**
    * Initializes the widget for use with the specified formEditor interface.
    */
    void initialize ( QDesignerFormEditorInterface * formEditor );
};

#endif
