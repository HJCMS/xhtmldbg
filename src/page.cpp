/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
**/

#include "page.h"
#include "viewer.h"
#include "xhtmldbg.h"
#include "networkaccessmanager.h"
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

Page::Page ( NetworkAccessManager * manager, QObject * parent )
    : QWebPage ( parent )
    , m_netManager ( manager )
    , xhtml ( QString::null )
{
  setObjectName ( "page" );

  setNetworkAccessManager ( m_netManager );
  // setLinkDelegationPolicy ( QWebPage::DelegateExternalLinks );

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

bool Page::prepareContent ( QNetworkReply * dev )
{
  xhtml.clear();
  QByteArray data = dev->readAll();
  if ( data.isEmpty() )
    return false;

  QTextCodec* codec = QTextCodec::codecForHtml ( data, fetchHeaderEncoding ( dev ) );
  xhtmldbg::instance()->mainWindow()->setSource ( codec->toUnicode ( data ) );
  return true;
}

void Page::replyFinished()
{
  if ( reply )
  {
    QString mimeType = reply->header ( QNetworkRequest::ContentTypeHeader ).toString();
    if ( mimeType.contains ( "text/html" ) &&  prepareContent ( reply ) )
      emit getUrl ( reply->url() );
  }
}

bool Page::acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest &request,
                                     QWebPage::NavigationType type )
{
  bool b = QWebPage::acceptNavigationRequest ( frame, request, type );
  if ( ! b )
    return b;

  // fetch Request Types for setSource View
  switch ( type )
  {
    case QWebPage::NavigationTypeLinkClicked:
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeBackOrForward:
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeReload:
      reply = m_netManager->get ( request );
      connect ( reply, SIGNAL ( finished() ), this, SLOT ( replyFinished() ) );
      return true;

    case QWebPage::NavigationTypeFormSubmitted:
      // The user activated a submit button for an HTML form.
      break;

    case QWebPage::NavigationTypeFormResubmitted:
      // An HTML form was submitted a second time.
      return false;

    case QWebPage::NavigationTypeOther:
      // typically a external reload or header location href request
      // after this a getRequest is used by WebKit
      break;

    default:
      break;
  }
  return b;
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
