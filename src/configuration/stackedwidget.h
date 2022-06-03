/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2013
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

#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

/* xhtmldbg */
#include "settings.h"

/* Configuration */
#include "configide.h"
#include "configdomtree.h"
#include "configbrowser.h"
#include "confignetwork.h"
#include "configcookies.h"
#include "configproxy.h"
#include "configssl.h"
#include "configuseragents.h"
#include "configwebsecurity.h"
#include "configextras.h"

class StackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    Settings* cfg;
    ConfigIDE* m_configIDE;
    ConfigDomTree* m_configDomTree;
    ConfigBrowser* m_configBrowser;
    ConfigNetwork* m_configNetwork;
    ConfigCookies* m_configCookies;
    ConfigProxy* m_configProxy;
    ConfigSSL* m_configSSL;
    ConfigUserAgents* m_configUserAgents;
    ConfigWebSecurity* m_configWebSecurity;
    ConfigExtras* m_configExtras;
    enum PageIndex
    {
      ITEM_CONFIG_IDE = 0,
      // ITEM_CONFIG_TIDY =  , DEPRECATED and removed
      ITEM_CONFIG_DOMTREE =  2,
      ITEM_CONFIG_BROWSER =  3,
      ITEM_CONFIG_NETWORK =  4,
      ITEM_CONFIG_COOKIES =  5,
      ITEM_CONFIG_PROXY =  6,
      ITEM_CONFIG_SSL =  7,
      ITEM_CONFIG_USERAGENTS = 8,
      ITEM_CONFIG_WEBSECURE = 9,
      ITEM_CONFIG_EXTRAS = 10
    };

  private Q_SLOTS:
    void loadWidgetSettings ( int );

  Q_SIGNALS:
    void settingsChanged ( bool );

  public Q_SLOTS:
    void loadSettings();
    void saveSettings();

  public:
    StackedWidget ( QWidget * parent = 0, Settings * settings = 0 );
    ~StackedWidget();
};

#endif
