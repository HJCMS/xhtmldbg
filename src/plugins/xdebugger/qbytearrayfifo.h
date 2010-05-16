/**
 * Copyright (C) 2005-2010 by Linus McCabe, <Linus@McCabe.nu>
 * Copyright (C) 2010-2011 by Juergen Heinemann http://www.hjcms.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef QBYTEARRAYFIFO_H
#define QBYTEARRAYFIFO_H

/* QtCore */
#include <stddef.h>
#include <QtCore/QByteArray>
#include <QtCore/QString>

/**
* \class QByteArrayFifo
* \version 0.1.2
* \short read byte array from tcp socket streams
* \author Linus McCabe, <Linus@McCabe.nu>
* \author Juergen Heinemann http://www.hjcms.de
*
* The engine responds with XML data.
* The XML data is prepended with a stringified integer representing
* the number of bytes in the XML data packet.
* The length and XML data are separated by a NULL byte.
* The XML data is ended with a NULL byte.
* Neither the IDE or debugger engine packets may contain NULL bytes
* within the packet since it is used as a separator.
* Data must be encoded using base64.
*
* [NUMBER] [NULL] XML(data) [NULL]
*
* \code
bool example ( QTcpSocket * socket )
{
  if ( ! socket )
    return false;

  int bytes;
  long m_datalen = -1;
  QString data;
  QByteArrayFifo m_fifo;
  if ( socket->waitForReadyRead () )
  {
    while ( socket->bytesAvailable() > 0 || m_fifo.length() >= ( unsigned long ) m_datalen )
    {
      if ( socket->bytesAvailable() > 0 )
      {
        // Read all available bytes from socket and append them to the buffer
        bytes = socket->bytesAvailable();
        char* buffer = new char[bytes];
        socket->read ( buffer, bytes );

        // Put it in the fifo buffer
        m_fifo.append ( buffer, bytes );

        delete[] buffer;
      }

      while ( 1 )
      {
        // If datalen == -1, we didnt read the size yet, otherwise we're reading data.
        if ( m_datalen == -1 )
        {
          bytes = m_fifo.find ( '\0' );
          if ( bytes < 0 )
            break;

          data = m_fifo.retrieve();
          m_datalen = data.toLong();

        }
        if ( m_datalen != -1 && ( long ) m_fifo.length() >= m_datalen + 1 )
        {
          data = m_fifo.retrieve();
          m_datalen = -1;
          qDebug() << Q_FUNC_INFO << data;
        }
        else
          break;
      }
    }
  }
  return true;
}
* \endcode
*/

class QByteArrayFifo
{
  private:
    QByteArray m_array;
    size_t m_size;

  public:
    QByteArrayFifo();

    /**
    * Resize the array and copy the elements
    */
    bool append ( const char * chars, size_t size );

    /**
    * See if there's a null teminator somewhere and retrive the Array Data.
    * Decrease also \ref m_array size and move bytes to the beginning of the array.
    */
    const QString retrieve();

    /**
    * Returns a copy of the byte array \ref m_array, encoded as Base64.
    */
    const QByteArray base64Encoded();

    /**
    * Returns the number of (potentially overlapping) occurrences
    * of byte array ba in this byte array.
    */
    long find ( char character );

    /**
    * Returns current Size \ref m_size
    */
    size_t length();

};

#endif
