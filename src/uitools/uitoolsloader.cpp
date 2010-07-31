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
#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QList>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

UiToolsLoader::UiToolsLoader ( const QString &cid, QObject * parent )
    : QUiLoader ( parent ), QScriptable()
    , classID ( cid )
    , isValid ( false )
{
  setObjectName ( QLatin1String ( "uitoolsloader" ) );

  /* Keine zusätzlichen Plugins aus QT_INSTALL_PLUGINS/designer lesen!
  * Es werden nur die Standard Plugins von Qt und meine verwendet!
  * Das hat den Hauptsächlichen Grund - zu verhindern das Fehlerhafte
  * Plugins, XHTMLBG zum absturts bringen!
  */
  clearPluginPaths();

  /* Setze meinen plugin Designer Pfad */
  QString plPath = QString ( "%1/designer" ).arg ( XHTMLDBG_PLUGIN_PATH );
  addPluginPath ( plPath );

  /* setze die Ausführung im /tmp Pfad fort */
  QDir chroot ( QDesktopServices::storageLocation ( QDesktopServices::TempLocation ) );
  setWorkingDirectory ( chroot );

  // Keine (Q)t oder (X)HTMLDBG Designer Klasse dann abbrechen!
  if ( ! classID.contains ( QRegExp ( "^(Q|X)\\w+$", Qt::CaseSensitive ) ) )
    return;

  // Nachsehen ob sie in der Liste der ladbaren Klassen ist
  isValid = availableWidgets().contains ( classID );

  // qDebug() << Q_FUNC_INFO << availableWidgets();

  // Map bereiningen
  uiConfig.clear();
}

/**
* Liest/schreibt die Prädikate und Werte in die Map @ref uiConfig
* Wenn Größe von @param params und @param values \b NICHT Identisch
* sind wird abgebrochen und keine Map erstellt!
* Es wird strickt nach XHTML gegangen!
*/
bool UiToolsLoader::setConfiguration ( const QStringList &params, const QStringList &values )
{
  if ( params.size() != values.size() )
  {
    isValid = false;
    qWarning ( "(XHTMLDBG) Accept 100%% sheer XHTML Object Elements with type application/x-qt-plugin - severance!" );
    return false;
  }

  if ( isValid )
  {
    uiConfig.clear();
    QStringListIterator it ( values );
    foreach ( QString p, params )
    {
      if ( ! p.isEmpty() && it.hasNext() )
        uiConfig[ p ] = it.next();
    }
  }
  return isValid;
}

/**
* Nachrichten Fenster bei Fehlern
*/
QWidget* UiToolsLoader::displayFailWidget ( QWidget * parent )
{
  QWidget* widget = new QWidget ( parent );
  QVBoxLayout* layout = new QVBoxLayout ( widget );
  layout->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );

  QString message ( trUtf8 ( "Sorry: I can't do it." ) );
  message.append ( "\n" );
  message.append ( trUtf8 ( "Mistrial Element initialization." ) );

  QLabel* label = new QLabel ( message, widget );
  label->setWordWrap ( true );
  layout->addWidget ( label );
  widget->setLayout ( layout );

  return widget;
}

/**
* Nehme die Ui:Komponente
**/
QWidget* UiToolsLoader::getUiComponent ( QWidget * parent )
{
  if ( isValid )
  {
    QString objName;
    // Wenn das Prädikat name existiert dann mit einfügen!
    if ( uiConfig.contains ( QLatin1String ( "name" ) ) )
      objName = uiConfig["name"];

    // TODO an dieser Stelle muss noch das setzen der Parameter erfolgen!
    return createWidget ( classID, parent, objName );
  }
  return displayFailWidget ( parent );
}

UiToolsLoader::~UiToolsLoader()
{
  isValid = false;
}
