/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#ifndef PAGE_H
#define PAGE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QByteArray>
#include <QtCore/QIODevice>
#include <QtCore/QEvent>

#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

class Page : public QWebPage
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qtidy.hjcms.de" )

  protected:
    void javaScriptConsoleMessage ( const QString &, int, const QString & );

  private Q_SLOTS:
    void triggerSelections();

  public:
    Page ( QObject* parent = 0 );
    virtual ~Page();
};


#endif
