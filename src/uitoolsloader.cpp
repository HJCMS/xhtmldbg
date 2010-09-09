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

#ifndef XHTMLDBG_VERSION_STRING
# include "version.h"
#endif
#include "uitoolsloader.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QMapIterator>
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaType>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>

/** @class UiToolsLoader */
UiToolsLoader::UiToolsLoader ( const QString &cid, const QUrl &file, QObject * parent )
    : QUiLoader ( parent ), QScriptable()
    , classID ( cid )
    , isValid ( false )
    , uiUrl ( file )
{
  setObjectName ( QLatin1String ( "UiToolsLoader" ) );
  setLanguageChangeEnabled ( true );

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

  // qDebug() << Q_FUNC_INFO << availableWidgets();

  // Nachsehen ob sie in der Liste der ladbaren Klassen ist
  isValid = availableWidgets().contains ( classID );

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
    emit message ( trUtf8 ( "(XHTMLDBG) Accept 100%% sheer XHTML Object Elements with type application/x-qt-plugin - severance!" ) );
    return false;
  }

  if ( isValid )
  {
    // Map bereiningen
    uiConfig.clear();
    QStringListIterator it ( values );
    foreach ( QString p, params )
    {
      if ( ! p.isEmpty() && it.hasNext() )
        uiConfig[ p ] = QVariant ( it.next() );
    }
  }
  return isValid;
}

/**
* Liest alle \b nich Schreibgeschütze Optionen in eine Liste.
* @see QMetaProperty::isWritable
*/
const QStringList UiToolsLoader::findProperties ( const QString &classID, QWidget * parent ) const
{
  QStringList list;
  int type = QMetaType::type ( classID.toAscii() );
  if ( QMetaType::isRegistered ( type ) )
  {
    const QMetaObject* mobj = parent->metaObject();
    for ( int i = mobj->propertyOffset(); i < mobj->propertyCount(); ++i )
    {
      const QMetaProperty prop = mobj->property ( i );
      if ( prop.isWritable() )
        list.append ( prop.name() );
    }
  }
  return list;
}

/**
* Nachrichten Fenster bei Fehlern
*/
QWidget* UiToolsLoader::displayFailWidget ( QWidget * parent, const QString &mess ) const
{
  QString message;
  QWidget* widget = new QWidget ( parent );
  QVBoxLayout* layout = new QVBoxLayout ( widget );
  layout->setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );

  if ( mess.isEmpty() )
  {
    message.append ( trUtf8 ( "Sorry: I can't do it." ) );
    message.append ( "\n" );
    message.append ( trUtf8 ( "Invalid Element initialization." ) );
  }
  else
    message = mess;

  QLabel* label = new QLabel ( message, widget );
  label->setWordWrap ( true );
  layout->addWidget ( label );
  widget->setLayout ( layout );

  return widget;
}

/**
* Nehme die Ui:Komponente
*/
QWidget* UiToolsLoader::getUiComponent ( QWidget * parent )
{
  if ( isValid )
  {
    QString objName;
    // Wenn das Prädikat name existiert dann mit einfügen!
    if ( uiConfig.contains ( QLatin1String ( "name" ) ) )
      objName = uiConfig["name"].toString();

    QWidget* widget = createWidget ( classID, parent, objName );
    // !!! Kein Klassen-Name keine Prädikate !!!
    if ( ! objName.isEmpty() )
    {
      QStringList props = findProperties ( objName, widget );
      // wenn keine Optionen zu verfügung stehen aussteigen
      if ( props.size() < 1 )
        return widget;

      QMapIterator<QString,QVariant> it ( uiConfig );
      while ( it.hasNext() )
      {
        it.next();
        if ( props.contains ( it.key() ) )
        {
          const QMetaObject* meta = widget->metaObject();
          int index = meta->indexOfProperty ( it.key().toAscii() );
          QMetaProperty prop = meta->property ( index );
          QVariant val = it.value();
          if ( val.convert ( prop.type() ) )
            widget->setProperty ( prop.name(), val );
          else
            emit message ( trUtf8 ( "(XHTMLDBG) Invalid value for property: %1" ).arg ( prop.name() ) );
        }
      }
    }

    return widget;
  }
  return displayFailWidget ( parent );
}

UiToolsLoader::~UiToolsLoader()
{
  isValid = false;
}
