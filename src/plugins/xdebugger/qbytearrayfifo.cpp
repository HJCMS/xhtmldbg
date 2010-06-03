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

#include "qbytearrayfifo.h"

QByteArrayFifo::QByteArrayFifo( )
{
  m_size = 0;
  m_array.resize ( 0 );
}

const QString QByteArrayFifo::retrieve( )
{
  // See if there's a null teminator somewhere
  QString str ( m_array );
  uint size = str.length() + 1;

  // Decrease size and move bytes to the beginning of the array
  m_size -= size;
  for ( uint cnt = 0; cnt < m_size; cnt++ )
  {
    m_array[cnt] = m_array[cnt + size];
  }

  // Resize array, needed for find() to work
  m_array.resize ( m_size );

  return str;
}

bool QByteArrayFifo::append ( const char * chars, uint size )
{
  // Resize the array, fail if not possible
  m_array.resize ( m_size + size );

  // Copy the elements
  for ( uint cnt = 0; cnt < size; cnt++ )
  {
    m_array[cnt + m_size] = chars[cnt];
  }

  // Increase size var
  m_size += size;

  return true;
}

long QByteArrayFifo::find ( char character )
{
  // If size is 0, find() outputs a warning for some reason
  if ( m_size == 0 )
    return -1;

  return static_cast<long> ( m_array.count ( character ) );
}

const QByteArray QByteArrayFifo::base64Encoded()
{
  return  m_array.toBase64();
}

uint QByteArrayFifo::length()
{
  return m_size;
}

// eof