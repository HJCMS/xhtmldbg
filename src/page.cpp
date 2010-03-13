/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"
#include "xhtmldbg.h"
#include "jsmessanger.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QClipboard>
#include <QtGui/QIcon>

/* QtNetwork */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

Page::Page ( QObject * parent )
    : QWebPage ( parent )
    , xhtml ( QString::null )
{
  setObjectName ( "page" );
  reply = 0;

  action ( QWebPage::Reload )->setShortcut ( QKeySequence::Refresh );
  action ( QWebPage::Back )->setShortcut ( QKeySequence::Back );
  action ( QWebPage::Forward )->setShortcut ( QKeySequence::Forward );
  action ( QWebPage::Copy )->setShortcut ( QKeySequence::Copy );
  action ( QWebPage::Copy )->setIcon ( QIcon::fromTheme ( "edit-copy" ) );

  connect ( this, SIGNAL ( selectionChanged() ), this, SLOT ( triggerSelections() ) );
}

void Page::javaScriptConsoleMessage ( const QString & m, int l, const QString & id )
{
  if ( m.isEmpty() )
    return;

  xhtmldbg::instance()->mainWindow()->JavaScriptMessanger()->messages ( m, l, id );
}

QTextCodec* Page::fetchHeaderEncoding ( QNetworkReply * reply )
{
  QString encoding ( "UTF-8" );
  if ( reply )
  {
    QByteArray cType = reply->rawHeader ( "Content-Type" );
    if ( ! cType.isEmpty() )
    {
      QString Charset ( cType );
      foreach ( QString param, Charset.split ( QRegExp ( "[\\s ]?;[\\s ]?" ) ) )
      {
        param.trimmed();
        if ( param.contains ( "charset", Qt::CaseInsensitive ) )
        {
          QString buf = param.split ( QRegExp ( "[\\s ]?=[\\s ]?" ) ).last();
          encoding = buf.toUpper();
          break;
        }
      }
    }
  }
  return QTextCodec::codecForName ( encoding.toAscii() );
}

void Page::headerFinished()
{}

void Page::replyFinished()
{
  if ( reply )
  {
    QString mimeType = reply->header ( QNetworkRequest::ContentTypeHeader ).toString();
    if ( mimeType.contains ( "text/html" ) )
    {
      QByteArray data = reply->readAll();
      QTextCodec* codec = QTextCodec::codecForHtml ( data, fetchHeaderEncoding ( reply ) );
      xhtml = codec->toUnicode ( data );
      // qDebug() << Q_FUNC_INFO << xhtml.length();
      // xhtmldbg::instance()->mainWindow()->setSourceView ( xhtml );
      emit getUrl ( reply->url() );
    }
  }
}

bool Page::acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest &request,
                                     QWebPage::NavigationType type )
{
  switch ( type )
  {
    case QWebPage::NavigationTypeLinkClicked:
      reply = networkAccessManager()->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      break;

    case QWebPage::NavigationTypeFormSubmitted:
      qDebug() << Q_FUNC_INFO << "NavigationTypeFormSubmitted";
      break;

    case QWebPage::NavigationTypeBackOrForward:
      reply = networkAccessManager()->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      break;

    case QWebPage::NavigationTypeReload:
      reply = networkAccessManager()->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      break;

    case QWebPage::NavigationTypeFormResubmitted:
      qDebug() << Q_FUNC_INFO << "NavigationTypeFormResubmitted";
      break;

    case QWebPage::NavigationTypeOther:
      reply = networkAccessManager()->head ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( headerFinished() ) );
      break;

    default:
      qDebug() << Q_FUNC_INFO << "UNKNOWN";
      break;
  }
  return QWebPage::acceptNavigationRequest ( frame, request, type );
}

void Page::triggerSelections()
{
  QString txt = selectedText();
  if ( txt.isEmpty() )
    return;

  QApplication::clipboard()->setText ( txt );
}

const QString Page::xhtmlSource()
{
  return xhtml;
}

Page::~Page()
{}
