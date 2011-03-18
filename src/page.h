/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://hjcms.de, (C) 2007-2011
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

#ifndef PAGE_H
#define PAGE_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QEvent>
#include <QtCore/QIODevice>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

/* QtWebKit */
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebSecurityOrigin>

/* KDE */
#include <KDE/KWebPage>
#include <KDE/KWebWallet>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class NetworkAccessManager;

class Page : public KWebPage
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://hjcms.de" )

  private:
    NetworkAccessManager* m_netManager;
    QNetworkReply* reply;
    QTextCodec* fetchHeaderEncoding ( QNetworkReply * );
    bool prepareContent ( QNetworkReply * );

  private Q_SLOTS:
    void initWebDatabaseAccess ( bool );
    void readPostResponse ( const QUrl &, const QString & );
    void unsupportedContentRequest ( QNetworkReply * );
    void downloadContentRequest ( const QNetworkRequest & );
    void internalMessanger ( const QString & );
    void triggerSelections();
    void replyFinished();

  protected:
    virtual void javaScriptConsoleMessage ( const QString &, int, const QString & );
    virtual bool javaScriptPrompt ( QWebFrame *, const QString &, const QString &, QString * );
    virtual void javaScriptAlert ( QWebFrame *, const QString & );
    virtual bool acceptNavigationRequest ( QWebFrame *, const QNetworkRequest &, QWebPage::NavigationType );
#ifdef HAVE_QTUITOOLS
    virtual QObject* createPlugin ( const QString &, const QUrl &, const QStringList &, const QStringList & );
#endif

  public:
    Page ( NetworkAccessManager * manager, QObject* parent = 0 );
    const QStringList keywordMetaTagItems();
    virtual ~Page();
};

#endif
