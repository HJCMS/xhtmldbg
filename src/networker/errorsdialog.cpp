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

#include "errorsdialog.h"

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

ErrorsDialog::ErrorsDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "errorsdialog" ) );
  setWindowTitle ( trUtf8 ( "Error" ) );
  setMinimumWidth ( 350 );
  setMinimumHeight ( 100 );
  setSizeGripEnabled ( true );
  setContentsMargins ( 2, 2, 2, 9 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  message = new QLabel ( trUtf8 ( "Unknown Failure Message" ), this );
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
bool ErrorsDialog::setError ( QNetworkReply::NetworkError err )
{
  bool opendialog = false;
  QString reasons;
  QString beforeReasons = trUtf8 ( "\nReasons : " );

  switch ( err )
  {
    case QNetworkReply::ConnectionRefusedError:
    {
      setWindowTitle ( trUtf8 ( "Bad Request (400)" ) );
      message->setText ( trUtf8 ( "the remote server refused the connection (the server is not accepting requests)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::RemoteHostClosedError:
    {
      setWindowTitle ( trUtf8 ( "Server Gone (410)" ) );
      reasons = trUtf8 ( "the remote server closed the connection prematurely, before the entire reply was received and processed" );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::HostNotFoundError:
    {
      setWindowTitle ( trUtf8 ( "Invalid Hostname or Host not Found" ) );
      reasons = trUtf8 ( "Invalid Hostname, Server is currently down or DNS Problems." );
      message->setText ( trUtf8 ( "The Remote Hostname was not found." ) + beforeReasons + reasons );
      opendialog = true;
      emit errorMessage ( reasons );
    }
    break;

    case QNetworkReply::TimeoutError:
    {
      setWindowTitle ( trUtf8 ( "Request Timeout (408)" ) );
      reasons = trUtf8 ( "the connection to the remote server timed out" );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::OperationCanceledError:
    break;

    case QNetworkReply::SslHandshakeFailedError:
    {
      emit errorMessage ( trUtf8 ( "the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted." ) );

    }
    break;

    case QNetworkReply::ProxyConnectionRefusedError:
    {
      setWindowTitle ( trUtf8 ( "Proxy Connection Refused Error" ) );
      reasons = trUtf8 ( "the connection to the proxy server was refused (the proxy server is not accepting requests)" );
      message->setText ( reasons );
      emit errorMessage ( reasons );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyConnectionClosedError:
    {
      setWindowTitle ( trUtf8 ( "Proxy Connection Closed Error" ) );
      message->setText ( trUtf8 ( "the proxy server closed the connection prematurely, before the entire reply was received and processed" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyNotFoundError:
    {
      setWindowTitle ( trUtf8 ( "Proxy Not Found Error" ) );
      message->setText ( trUtf8 ( "the proxy host name was not found (invalid proxy hostname)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyTimeoutError:
    {
      setWindowTitle ( trUtf8 ( "Proxy Timeout Error" ) );
      message->setText ( trUtf8 ( "the connection to the proxy timed out or the proxy did not reply in time to the request sent" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProxyAuthenticationRequiredError:
    {
      setWindowTitle ( trUtf8 ( "Proxy Authentication Required Error (407)" ) );
      message->setText ( trUtf8 ( "the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentAccessDenied:
    {
      setWindowTitle ( trUtf8 ( "Unauthorized (401)" ) );
      message->setText ( trUtf8 ( "the access to the remote content was denied (similar to HTTP error 401)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentOperationNotPermittedError:
    {
      emit errorMessage ( trUtf8 ( "the operation requested on the remote content is not permitted" ) );
    }
    break;

    case QNetworkReply::ContentNotFoundError:
    {
      emit errorMessage ( trUtf8 ( "the remote content was not found at the server (similar to HTTP error 404)" ) );
    }
    break;

    case QNetworkReply::AuthenticationRequiredError:
    {
      setWindowTitle ( trUtf8 ( "Authentication Required Error" ) );
      message->setText ( trUtf8 ( "the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ContentReSendError:
    {
      setWindowTitle ( trUtf8 ( "Content Re Send Error" ) );
      message->setText ( trUtf8 ( "the request needed to be sent again, but this failed for example because the upload data could not be read a second time." ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::ProtocolUnknownError:
    {
      emit errorMessage ( trUtf8 ( "QtNetwork Access API cannot honor the request because the protocol is not known." ) );
    }
    break;

    case QNetworkReply::ProtocolInvalidOperationError:
    {
      setWindowTitle ( trUtf8 ( "Protocol Invalid Operation Error" ) );
      message->setText ( trUtf8 ( "the requested operation is invalid for this protocol" ) );
      opendialog = true;
    }
    break;

    case QNetworkReply::UnknownNetworkError:
    {
      emit errorMessage ( trUtf8 ( "an unknown network-related error was detected" ) );
    }
    break;

    case QNetworkReply::UnknownProxyError:
    {
      emit errorMessage ( trUtf8 ( "an unknown proxy-related error was detected" ) );
    }
    break;

    case QNetworkReply::UnknownContentError:
    {
      emit errorMessage ( trUtf8 ( "an unknown error related to the remote content was detected" ) );
    }
    break;

    case QNetworkReply::ProtocolFailure:
    {
      setWindowTitle ( trUtf8 ( "Protocol Failure" ) );
      message->setText ( trUtf8 ( "a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)" ) );
      opendialog = true;
    }
    break;

    default:
      emit errorMessage ( trUtf8 ( "an unknown related error was detected" ) );
      break;
  }

  return opendialog;
}

ErrorsDialog::~ErrorsDialog()
{}
