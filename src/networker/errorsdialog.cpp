/**
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "errorsdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

/* KDE */
#include <KDE/KLocale>

ErrorsDialog::ErrorsDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "errorsdialog" ) );
  setWindowTitle ( i18n ( "Error" ) );
  setMinimumWidth ( 350 );
  setMinimumHeight ( 100 );
  setSizeGripEnabled ( true );
  setContentsMargins ( 2, 2, 2, 9 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  message = new QLabel ( i18n ( "Unknown Failure Message" ), this );
  message->setWordWrap ( true );
  vLayout->addWidget ( message );

  QDialogButtonBox::StandardButtons buttons = ( QDialogButtonBox::Ok );
  QDialogButtonBox* box = new QDialogButtonBox ( buttons, Qt::Horizontal, this );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
}

/**
* Hier wird die vom Netzwerk Manager generierte Nachricht eingefügt.
*/
void ErrorsDialog::setMessage ( const QString &m )
{
  message->setText ( m );
}

/**
* Eine grobe Übersetzung der Netzwerk Meldungen!
* @note Nicht alle Nachrichten werden als PopUp ausgegeben!
* Diese senden das Signal errorMessage zurück an den Manager,
* der es dann mit @ref NetworkAccessManager::netNotify über @class Window
* an @ref AppEvents::insertMessage weiter gibt.
*/
bool ErrorsDialog::setError ( QNetworkReply::NetworkError err, const QUrl &url )
{
  bool opendialog = false;
  QString reasons;
  QString beforeReasons = i18n ( "\nReasons : " );
  QString host = url.host();
  QString path = url.path();

  switch ( err )
  {
    case QNetworkReply::ConnectionRefusedError:
    {
      setWindowTitle ( i18n ( "Bad Request (400)" ) );
      message->setText ( i18n ( "the remote server \"%1\" refused the connection (the server is not accepting requests)" ).arg ( host ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::RemoteHostClosedError:
    {
      setWindowTitle ( i18n ( "Server Gone (410)" ) );
      reasons = i18n ( "the remote server \"%1\" closed the connection prematurely, before the entire reply was received and processed" ).arg ( host );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::HostNotFoundError:
    {
      setWindowTitle ( i18n ( "Invalid Hostname or Host not Found" ) );
      reasons = i18n ( "Invalid Hostname \"%1\", Server is currently down or DNS Problems." ).arg ( host );
      message->setText ( i18n ( "The Remote Hostname was not found." ) + beforeReasons + reasons );
      opendialog = true;
      emit errorMessage ( reasons );
    }
    break;

    case QNetworkReply::TimeoutError:
    {
      setWindowTitle ( i18n ( "Request Timeout (408)" ) );
      reasons = i18n ( "the connection to the remote server \"%1\" timed out" ).arg ( host );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::OperationCanceledError:
      break;

    case QNetworkReply::SslHandshakeFailedError:
    {
      emit errorMessage ( i18n ( "the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted." ) );

    }
    break;

    case QNetworkReply::ProxyConnectionRefusedError:
    {
      setWindowTitle ( i18n ( "Proxy Connection Refused Error" ) );
      reasons = i18n ( "the connection to the proxy server \"%1\" was refused (the proxy server is not accepting requests)" ).arg ( host );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyConnectionClosedError:
    {
      setWindowTitle ( i18n ( "Proxy Connection Closed Error" ) );
      message->setText ( i18n ( "the proxy server closed the connection prematurely, before the entire reply was received and processed" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyNotFoundError:
    {
      setWindowTitle ( i18n ( "Proxy Not Found Error" ) );
      message->setText ( i18n ( "the proxy host name was not found (invalid proxy hostname)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyTimeoutError:
    {
      setWindowTitle ( i18n ( "Proxy Timeout Error" ) );
      message->setText ( i18n ( "the connection to the proxy timed out or the proxy did not reply in time to the request sent" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyAuthenticationRequiredError:
    {
      setWindowTitle ( i18n ( "Proxy Authentication Required Error (407)" ) );
      message->setText ( i18n ( "the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentAccessDenied:
    {
      setWindowTitle ( i18n ( "Unauthorized (401)" ) );
      message->setText ( i18n ( "the access to the remote \"%1\" content was denied (similar to HTTP error 401)" ).arg ( path ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentOperationNotPermittedError:
    {
      emit errorMessage ( i18n ( "the operation requested on the remote \"%1\" content is not permitted" ).arg ( path ) );
    }
    break;

    case QNetworkReply::ContentNotFoundError:
    {
      emit errorMessage ( i18n ( "the remote content \"%1\" was not found at the server (similar to HTTP error 404)" ).arg ( path ) );
    }
    break;

    case QNetworkReply::AuthenticationRequiredError:
    {
      setWindowTitle ( i18n ( "Authentication Required Error" ) );
      message->setText ( i18n ( "the remote server \"%1\" requires authentication to serve the content but the credentials provided were not accepted (if any)" ).arg ( host ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentReSendError:
    {
      setWindowTitle ( i18n ( "Content Re Send Error" ) );
      message->setText ( i18n ( "the request needed to be sent again, but this failed for example because the upload data could not be read a second time." ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProtocolUnknownError:
    {
      emit errorMessage ( i18n ( "QtNetwork Access API cannot honor the request because the protocol is not known." ) );
    }
    break;

    case QNetworkReply::ProtocolInvalidOperationError:
    {
      setWindowTitle ( i18n ( "Protocol Invalid Operation Error" ) );
      message->setText ( i18n ( "the requested operation is invalid for this protocol" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::UnknownNetworkError:
    {
      emit errorMessage ( i18n ( "an unknown network-related error was detected" ) );
    }
    break;

    case QNetworkReply::UnknownProxyError:
    {
      emit errorMessage ( i18n ( "an unknown proxy-related error was detected" ) );
    }
    break;

    case QNetworkReply::UnknownContentError:
    {
      emit errorMessage ( i18n ( "an unknown error related to the remote content was detected" ) );
    }
    break;

    case QNetworkReply::ProtocolFailure:
    {
      setWindowTitle ( i18n ( "Protocol Failure" ) );
      message->setText ( i18n ( "a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)" ) );
      opendialog = true;
    }
    break;

    default:
      emit errorMessage ( i18n ( "an unknown related error was detected" ) );
      break;
  }

  return opendialog;
}

ErrorsDialog::~ErrorsDialog()
{}
