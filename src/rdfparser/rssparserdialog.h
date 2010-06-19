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

#ifndef RSSPARSERDIALOG_H
#define RSSPARSERDIALOG_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QListWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

/* QtNetwork */
#include <QtNetwork/QNetworkReply>

class RaptorParser;
class XsdParser;
class RSSTreeView;
class RSSViewer;

class RSSParserDialog : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

  private:
    const QUrl rssUrl;
    const QString mimeType;
    const QIcon iconWarning;
    const QIcon iconNotice;
    RaptorParser* m_parser;
    XsdParser* m_xsdParser;
    QToolBox* toolBox;
    RSSTreeView* m_treeViewer;
    QListWidget* m_MessagesList;
    RSSViewer* m_sourceViewer;
    QNetworkReply* reply;
    void setDocumentSource ( const QByteArray &, const QUrl & );

  private Q_SLOTS:
    void requestFinished();
    void notice ( const QString & );
    void error ( const QString & );

  public:
    RSSParserDialog ( const QUrl &url, const QString &type, QWidget * parent = 0 );
    virtual ~RSSParserDialog();
};

#endif
