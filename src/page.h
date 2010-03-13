/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef PAGE_H
#define PAGE_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>
#include <QtCore/QByteArray>
#include <QtCore/QIODevice>
#include <QtCore/QEvent>

/* QtWebKit */
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

/* QtNetwork */
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class Page : public QWebPage
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qtidy.hjcms.de" )
    Q_PROPERTY ( QString xhtml READ xhtmlSource )

  private:
    QString xhtml;
    QNetworkReply* reply;
    QTextCodec* fetchHeaderEncoding ( QNetworkReply * );
    bool prepareContent ( QNetworkReply * dev );

  protected:
    void javaScriptConsoleMessage ( const QString &, int, const QString & );
    bool acceptNavigationRequest ( QWebFrame *, const QNetworkRequest &, QWebPage::NavigationType );

  private Q_SLOTS:
    void triggerSelections();
    void replyFinished();

  Q_SIGNALS:
    void getUrl ( const QUrl & );

  public:
    Page ( QObject* parent = 0 );
    const QString xhtmlSource();
    virtual ~Page();
};


#endif
