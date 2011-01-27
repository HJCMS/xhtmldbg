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

#ifndef ALTERNATELINKREADER_H
#define ALTERNATELINKREADER_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSignalMapper>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

/* QtWebKit */
#include <QtWebKit/QWebElement>

class AlternateLinkReader : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QString comboTitle;
    const QIcon rssIcon;
    struct LinkItem
    {
      QUrl url;
      QString mime;
    };
    QList<LinkItem> items;
    QSignalMapper* m_signalMapper;
    QToolButton* m_toolButton;
    void openParserDialog ( const QUrl &, const QString & );

  private Q_SLOTS:
    void itemClicked ( int );

  public Q_SLOTS:
    void setDomTree ( const QUrl &, const QWebElement & );

  public:
    AlternateLinkReader ( QWidget * parent = 0 );
    virtual ~AlternateLinkReader();
};

#endif
